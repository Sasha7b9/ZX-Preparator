// 2022/05/20 15:48:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Parser/ParserTAP.h"
#include "Emulator/Emulator.h"
#include "Emulator/Memory.h"
#include "Emulator/DataBase.h"
#include "Notebook/Notebook.h"
#include "Parser/KeyCodes.h"
#pragma warning (push, 0)
    #include <wx/statline.h>
    #include <wx/file.h>
    #include <wx/wfstream.h>
    #include <wx/datstrm.h>
#pragma warning (pop)
#include <sstream>


/*
    Описание : https://sinclair.wiki.zxnet.co.uk/wiki/TAP_format

    Then raw tape data follows, including the flag andchecksum bytes.The checksum is the bitwise XOR of all bytes including the flag byte.
    For example, when you execute the line SAVE "ROM" CODE 0, 2 this will result :

          |------ Spectrum generated data -------|       |---------|
    13 00 00 03 52 4f 4d 7x20 02 00 00 00 00 80 f1 04 00 ff f3 af a3
    
    ^^^^^...... first block is 19 bytes(17 bytes + flag + checksum)
          ^^... flag byte(A reg, 00 for headers, ff for data blocks)
             ^^ first byte of header, indicating a code block
    
    file name .. ^^^^^^^^^^^^
    header info ..............^^^^^^^^^^^^^^^^
    checksum of header .........................^^
    length of second block ........................^^^^^
    flag byte ...........................................^^
    first two bytes of rom .................................^^^^^
    checksum(checkbittoggle would be a better name!)............. ^^
*/


bool ParserTAP::Execute(pchar fileName)
{
    file = std::make_shared<FileTAP>();

    if (!wxFile::Exists(fileName))
    {
        return false;
    }

    file->dump.FillFrom(fileName);

    wxFileInputStream file_input(fileName);

    wxBufferedInputStream stream(file_input);

    wxDataInputStream data(stream);

    while (data.IsOk())
    {
        BlockTAP block(data);
        if (block.Read())
        {
            file->AppendBlock(std::make_shared<BlockTAP>(std::move(block)));
        }
        else
        {
            break;
        }
    }

    file->is_valid = (file->NumberBlocks() != 0);

    return file->is_valid;
}


Block *BlockTAP::Read()
{
    Clear();

    header.Read();

    if (header.size == 19)
    {
        data.Read(header);
    }
    else if (header.size != 0)
    {
        data.Read(header);
    }

    return (header.IsValid() && data.IsValid()) ? this : nullptr;
}


bool BlockTAP::IsValid() const
{
    return header.IsValid() && data.IsValid();
}


void BlockTAP::Clear()
{
    header.Clear();

    data.Clear();
}


bool BlockTAP::Header::Read()
{
    valid = false;

    crc = 0;

    size = GetStream().Read16();

    if (size == 19)             // Для заголовка здесь всегда 19
    {
        if (Read8() == 0)       // Чтение флага - для заголовка 0
        {
            type = (BlockTAP::Header::TypeData::E)Read8();

            if (type < BlockTAP::Header::TypeData::PURE_BYTES)
            {
                for (int i = 0; i < 10; i++)
                {
                    file_name[i] = (char)Read8();
                }

                file_name[10] = 0;

                size_data = Read16();

                param1 = Read16();

                param2 = Read16();

                valid = GetStream().IsOk();

                if (GetStream().Read8() != crc)
                {
                    valid = false;
                }
            }
        }
    }
    else if (size != 0)
    {
        type = BlockTAP::Header::TypeData::PURE_BYTES;
        size_data = (uint16)(size - 2);
        valid = true;
    }

    return valid;
}


bool BlockTAP::Data::Read(const Header &header)
{
    valid = false;

    crc = 0;

    data.clear();

    if (header.type == BlockTAP::Header::TypeData::PURE_BYTES)
    {
        size = header.size_data;

        for (int i = 0; i < header.size_data + 1; i++)
        {
            data.push_back(Read8());
        }

        valid = GetStream().IsOk();

        if (GetStream().Read8() != crc)
        {
            valid = false;
        }
    }
    else
    {
        size = GetStream().Read16();

        if (size == header.size_data + 2)
        {
            if (Read8() == 0xff)        // Для данных должно быть 255
            {
                for (int i = 0; i < header.size_data; i++)
                {
                    data.push_back(Read8());
                }

                valid = GetStream().IsOk();

                if (GetStream().Read8() != crc)
                {
                    valid = false;
                }
            }
        }
    }

    return valid;
}


uint8 BlockTAP::CommonStruct::Read8()
{
    uint8 byte = GetStream().Read8();

    crc ^= byte;

    return byte;
}


uint16 BlockTAP::CommonStruct::Read16()
{
    uint16 result = Read8();

    result |= Read8() << 8;

    return result;
}


bool BlockTAP::ParseBASIC(ProgramBASIC &program)
{
    program.Clear();

    if (header.type == BlockTAP::Header::TypeData::BASIC_PROGRAM)
    {
        while (data.data.size())
        {
            program.lines.push_back(ParseLineProgram());
        }

        return true;
    }

    return false;
}


bool BlockTAP::ParseASM(ProgramASM &program)
{
    program.Clear();

    if (header.type == BlockTAP::Header::TypeData::CODE)
    {
        InfoStruct params;

        std::memcpy(params.MEMORY, Memory::_48, 64 * 1024);

        std::memcpy(params.MEMORY + header.param1, data.data.data(), header.size_data);

        StorageInstructions storage(header.param1, header.param1 + header.size_data);

        storage.AppendEntryPoint(header.param1);

        Emulator::Init(&params);

        int address = storage.NextAddress();

        while (address > 0)
        {
            bool res = Emulator::Decode((uint16)address) != 0;

            storage.AddNewData(res, address, &params);

            address = storage.NextAddress();
        }

        storage.CreateProgram(program);

        return true;
    }

    return false;
}


LineBASIC BlockTAP::ParseLineProgram()
{
    LineBASIC line;

    line.number = data.GetData16Reverse();
    line.size = data.GetData16();

    for (int i = 0; i < line.size - 1 && !data.data.empty(); i++)
    {
        uint8 code = data.GetData8();

        const char *string = KeyCodes::Get(code);

        line.symbols.push_back(string);
    }

    if (!data.data.empty())
    {
        data.GetChar();
    }

    return line;
}


uint16 BlockTAP::Data::GetData16Reverse()
{
    uint16 result = (uint16)(GetData8() << 8);
    result |= GetData8();
    return result;
}


uint16 BlockTAP::Data::GetData16()
{
    uint16 result = GetData8();
    result |= GetData8() << 8;
    return result;
}


uint8 BlockTAP::Data::GetData8()
{
    uint8 result = data[0];

    data.erase(data.begin());

    return result;
}


char BlockTAP::Data::GetChar()
{
    return (char)GetData8();
}
