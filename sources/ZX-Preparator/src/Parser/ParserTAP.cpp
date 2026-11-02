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


BlockTAP_Header *BlockTAP::ReadHeaderBlock(wxDataInputStream &stream)
{
    BlockTAP_Header *block = new BlockTAP_Header(stream);

    uint16 size = stream.Read16();

    if (size == 19)                 // У заголовка размер всегда 19
    {
        block->data.Resize(size);

        block->type = (Type::E)block->Read8();

        if (block->type < Type::PURE_BYTES)
        {
            block->Read8((uint8 *)block->file_name, 10);

            block->file_name[10] = '\0';

            block->size_data = block->Read16();

            block->param1 = block->Read16();

            block->param2 = block->Read16();

            block->valid = (block->Read8() == 0);
        }

        return block;
    }

    return nullptr;
}


BlockTAP *BlockTAP::ReadDataBlock(wxDataInputStream &stream, BlockTAP_Header *header)
{
    BlockTAP *block = nullptr;

    if (header->type == BlockTAP::Type::BASIC_PROGRAM)
    {
        block = new BlockTAP_BASIC(stream);
    }
    else if (header->type == BlockTAP::Type::ARRAY_NUMBER)
    {
        block = new BlockTAP_ArrayNumber(stream);
    }
    else if (header->type == BlockTAP::Type::ARRAY_CHARACTER)
    {
        block = new BlockTAP_ArrayCharacter(stream);
    }
    else if (header->type == BlockTAP::Type::CODE)
    {
        block = new BlockTAP_Code(stream);
    }

    block->data.Resize(header->size_data);

    block->Read8(block->data.BeginData(), header->size_data);

    return block;
}


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
        BlockTAP_Header *header = BlockTAP::ReadHeaderBlock(data);

        if (header)
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
            while (data.IsOk())
            {
                data.Read8();
                file->number_unreaded_bytes++;
            }

            break;
        }
    }

    file->is_valid = (file->NumberBlocks() != 0);

    return file->is_valid;
}


bool BlockTAP_BASIC::ParseBASIC(ProgramBASIC &program)
{
    program.Clear();

    while (data.GetSize())
    {
        program.lines.push_back(ParseLineProgram());
    }

    return true;
}


LineBASIC BlockTAP_BASIC::ParseLineProgram()
{
    LineBASIC line;

    line.number = data.GetData16Reverse();
    line.size = data.GetData16();

    for (int i = 0; (i < line.size - 1) && (data.GetSize() != 0); i++)
    {
        uint8 code = data.GetData8();

        const char *string = KeyCodes::Get(code);

        line.symbols.push_back(string);
    }

    if (!data.IsEmpty())
    {
        data.GetChar();
    }

    if (!data.IsEmpty())
    {
        data.GetChar();
    }

    return line;
}


uint16 BlockTAP::Data::GetData16Reverse()
{
    if (data.size() >= 2)
    {
        auto it = data.begin();
        uint8 first = *it++;
        uint8 second = *it++;
        data.erase(data.begin(), it);

        return (first << 8) + second;
    }

    LOG_ERROR("Not enough elements in the array");

    return 0;
}


uint16 BlockTAP::Data::GetData16()
{
    if (data.size() >= 2)
    {
        auto it = data.begin();
        uint8 first = *it++;
        uint8 second = *it++;
        data.erase(data.begin(), it);

        return (first) + (second << 8);
    }

    LOG_ERROR("Not enough elements in the array");

    return 0;
}


uint8 BlockTAP::Data::GetData8()
{
    if (data.size())
    {
        auto it = data.begin();
        uint8 result = *it++;;
        data.erase(data.begin(), it);
        return result;
    }

    LOG_ERROR("Not enough elements in the array");

    return 0;
}


char BlockTAP::Data::GetChar()
{
    return (char)GetData8();
}


bool BlockTAP::Data::IsEmpty() const
{
    return GetSize() == 0;
}


bool BlockTAP_Code::ParseASM(ProgramASM &program)
{
    program.Clear();

    InfoStruct params;

    std::memcpy(params.MEMORY, Memory::_48, 64 * 1024);

    std::memcpy(params.MEMORY + StartAddressCode(), data.BeginData(), data.GetSize());

    StorageInstructions storage(StartAddressCode(), StartAddressCode() + data.GetSize());

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
