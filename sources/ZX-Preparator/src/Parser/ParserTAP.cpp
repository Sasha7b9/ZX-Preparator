// 2026/11/02 09:55:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/ParserTAP.h"
#include "Parser/KeyCodes.h"
#include "Emulator/Emulator.h"
#include "Emulator/Memory.h"
#include "Emulator/DataBase.h"
#pragma warning (push, 0)
    #include <wx/statline.h>
    #include <wx/file.h>
    #include <wx/wfstream.h>
    #include <wx/datstrm.h>
#pragma warning (pop)


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
        wxFileOffset posBefore = stream.TellI();

        BlockTAP_Header *header = new BlockTAP_Header(data);

        if (header->IsValid())
        {
            file->AppendBlock(std::shared_ptr<BlockTAP>(header));

            BlockTAP *block = BlockTAP::ReadDataBlock(data, header);

            if (block)
            {
                file->AppendBlock(std::shared_ptr<BlockTAP>(block));
            }
        }
        else
        {
            stream.SeekI(posBefore);

            BlockTAP_PureBytes *pure_bytes = new BlockTAP_PureBytes(data);

            if (pure_bytes->IsValid())
            {
                file->AppendBlock(std::shared_ptr<BlockTAP>(pure_bytes));
            }
            else
            {
                stream.SeekI(posBefore);

                BlockTAP_Undefined *undef = new BlockTAP_Undefined(data);

                if (undef->IsValid())
                {
                    file->AppendBlock(std::shared_ptr<BlockTAP>(undef));
                }
            }
        }
    }

    file->is_valid = (file->NumberBlocks() != 0);

    return file->is_valid;
}


pchar BlockTAP::NameASCII() const
{
    return "Generic block TAP";
}


pchar BlockTAP_Header::NameASCII() const
{
    return "Header";
}

pchar BlockTAP_BASIC::NameASCII() const
{
    return "BASIC PROGRAM";
}

pchar BlockTAP_ArrayCharacter::NameASCII() const
{
    return "ARRAY CHARACTER";
}

pchar BlockTAP_ArrayNumber::NameASCII() const
{
    return "ARRAY NUMBER";
}

pchar BlockTAP_Code::NameASCII() const
{
    if (ToCode() && GetSize() == 6912 && ToCode()->StartAddressCode() == 16384)
    {
        return "DATA SCREEN";
    }

    return "MACHINE CODE";
}

pchar BlockTAP_PureBytes::NameASCII() const
{
    return "Pure bytes (bytes without header)";
}


pchar BlockTAP_Undefined::NameASCII() const
{
    return "Undefined data";
}


BlockTAP_Header *BlockTAP::ToHeader() const
{
    return dynamic_cast<BlockTAP_Header *>(const_cast<BlockTAP *>(this));
}


BlockTAP_BASIC *BlockTAP::ToBASIC() const
{
    return dynamic_cast<BlockTAP_BASIC *>(const_cast<BlockTAP *>(this));
}


BlockTAP_ArrayNumber *BlockTAP::ToArrayNumber() const
{
    return dynamic_cast<BlockTAP_ArrayNumber *>(const_cast<BlockTAP *>(this));
}


BlockTAP_ArrayCharacter *BlockTAP::ToArrayCharacter() const
{
    return dynamic_cast<BlockTAP_ArrayCharacter *>(const_cast<BlockTAP *>(this));
}


BlockTAP_Code *BlockTAP::ToCode() const
{
    return dynamic_cast<BlockTAP_Code *>(const_cast<BlockTAP *>(this));
}


BlockTAP_Undefined *BlockTAP::ToUndefined() const
{
    return dynamic_cast<BlockTAP_Undefined *>(const_cast<BlockTAP *>(this));
}


BlockTAP_PureBytes *BlockTAP::ToPureBytes() const
{
    return dynamic_cast<BlockTAP_PureBytes *>(const_cast<BlockTAP *>(this));
}


BlockTAP_Header::BlockTAP_Header(wxDataInputStream &stream)
{
    uint16 size = stream.Read16();

    if (size == 19)                 // У заголовка размер всегда 19
    {
        data.Push16(size);

        data.AppendBytes(stream, size);

        data.SetPointer(2);         // Перемещаемся к первому байту содержимого

        if (Read8() == 0)           // У заголовка здесь всегда 0
        {
            type = (Type::E)Read8();

            if (type < Type::PURE_BYTES)
            {
                ReadBytes((uint8 *)file_name, 10);

                size_data = Read16();

                param1 = Read16();

                param2 = Read16();

                if (data.Read8() == crc)          // Проверка CRC
                {
                    is_valid = true;
                }
            }
        }
    }
}


BlockTAP_BASIC::BlockTAP_BASIC(wxDataInputStream &stream, BlockTAP_Header *header)
{
    data.AppendBytes(stream, header->size_data);

    if (ParseBASIC())
    {
        is_valid = true;
    }
}


uint8 BlockTAP_Header::Read8()
{
    uint8 result = data.Read8();

    crc ^= result;

    return result;
}


uint16 BlockTAP_Header::Read16()
{
    return Read8() + (Read8() << 8);
}


void BlockTAP_Header::ReadBytes(uint8 *buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        *buffer++ = Read8();
    }
}


bool BlockTAP_BASIC::ParseBASIC()
{
    program.Clear();

    int last_bytes = data.BytesLast();

    while (last_bytes)
    {
        program.lines.push_back(ParseLineProgram());
        last_bytes = data.BytesLast();
    }

    return true;
}


LineBASIC BlockTAP_BASIC::ParseLineProgram()
{
    LineBASIC line;

    line.number = data.Read16Reverse();
    line.size = data.Read16();

    for (int i = 0; (i < line.size - 1) && (data.BytesLast() != 0); i++)
    {
        uint8 code = data.Read8();

        const char *string = KeyCodes::Get(code);

        line.symbols.push_back(string);
    }

    if (!data.IsEmpty())
    {
        data.ReadChar();
    }

    if (!data.IsEmpty())
    {
        data.ReadChar();
    }

    return line;
}


BlockTAP_ArrayNumber::BlockTAP_ArrayNumber(wxDataInputStream &, BlockTAP_Header *)
{

}


BlockTAP_ArrayCharacter::BlockTAP_ArrayCharacter(wxDataInputStream &, BlockTAP_Header *)
{

}


BlockTAP_Code::BlockTAP_Code(wxDataInputStream &, BlockTAP_Header *)
{

}


BlockTAP_PureBytes::BlockTAP_PureBytes(wxDataInputStream &stream)
{
    uint16 size = stream.Read16();

    data.AppendBytes(stream, size);

    is_valid = true;
}


BlockTAP_Undefined::BlockTAP_Undefined(wxDataInputStream &)
{

}


BlockTAP *BlockTAP::ReadDataBlock(wxDataInputStream &stream, BlockTAP_Header *header)
{
    BlockTAP *block = nullptr;

    if (header->type == BlockTAP::Type::BASIC_PROGRAM)
    {
        block = new BlockTAP_BASIC(stream, header);
    }
    else if (header->type == BlockTAP::Type::ARRAY_NUMBER)
    {
        block = new BlockTAP_ArrayNumber(stream, header);
    }
    else if (header->type == BlockTAP::Type::ARRAY_CHARACTER)
    {
        block = new BlockTAP_ArrayCharacter(stream, header);
    }
    else if (header->type == BlockTAP::Type::CODE)
    {
        block = new BlockTAP_Code(stream, header);
    }

    return block;
}


void BlockTAP::Data::Push16(uint16 value)
{
    data.push_back((uint8)value);
    data.push_back((uint8)(value >> 8));
}


void BlockTAP::Data::AppendBytes(wxDataInputStream &stream, int size)
{
    int offset =(int)data.size();

    data.resize(data.size() + size);

    stream.Read8(data.data() + offset, size);

    pointer = 0;
}


uint16 BlockTAP::Data::Read16Reverse()
{
    if (BytesLast() >= 2)
    {
        uint8 first = data.data()[pointer++];
        uint8 second = data.data()[pointer++];

        return (first << 8) + second;
    }

    LOG_ERROR("Not enough elements in the array");

    return 0;
}


uint16 BlockTAP::Data::Read16()
{
    if (BytesLast() >= 2)
    {
        uint8 first = data.data()[pointer++];
        uint8 second = data.data()[pointer++];

        return (first) + (second << 8);
    }

    LOG_ERROR("Not enough elements in the array");

    return 0;
}


void BlockTAP::Data::ReadBytes(uint8 *buffer, int size)
{
    std::memcpy(buffer, data.data() + pointer, size);

    pointer += size;
}


uint8 BlockTAP::Data::Read8()
{
    if (BytesLast())
    {
        return data.data()[pointer++];
    }

    LOG_ERROR("Not enough elements in the array");

    return 0;
}


char BlockTAP::Data::ReadChar()
{
    return (char)Read8();
}


bool BlockTAP::Data::IsEmpty() const
{
    return BytesLast() == 0;
}


bool BlockTAP_Code::ParseASM(ProgramASM &program)
{
    program.Clear();

    InfoStruct params;

    std::memcpy(params.MEMORY, Memory::_48, 64 * 1024);

    std::memcpy(params.MEMORY + StartAddressCode(), data.BeginData(), data.AllBytes());

    StorageInstructions storage(StartAddressCode(), StartAddressCode() + data.AllBytes());

    storage.AppendEntryPoint(StartAddressCode());

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
