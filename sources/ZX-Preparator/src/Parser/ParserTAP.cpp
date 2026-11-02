// 2026/11/02 09:55:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/ParserTAP.h"
#pragma warning (push, 0)
    #include <wx/statline.h>
    #include <wx/file.h>
    #include <wx/wfstream.h>
    #include <wx/datstrm.h>
#pragma warning (pop)


BlockTAP_Header *BlockTAP::ToHeader() const
{
    return dynamic_cast<BlockTAP_Header *>(const_cast<BlockTAP *>(this));
}


BlockTAP_BASIC *BlockTAP::ToBASIC() const
{
    return dynamic_cast<BlockTAP_BASIC *>(const_cast<BlockTAP *>(this));
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
