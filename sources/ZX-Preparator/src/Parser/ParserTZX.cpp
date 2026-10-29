// 2026/10/29 09:12:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/ParserTZX.h"
#include "Notebook/PageInfo.h"
#pragma warning (push, 0)
    #include <wx/statline.h>
    #include <wx/statline.h>
    #include <wx/file.h>
    #include <wx/wfstream.h>
    #include <wx/datstrm.h>
#pragma warning (pop)


/*
    Описание : https://worldofspectrum.net/TZXformat.html
*/


bool ParserTZX::Execute(pchar fileName)
{
    file = std::make_shared<FileTZX>();

    if (!wxFile::Exists(fileName))
    {
        return false;
    }

    file->dump.FillFrom(fileName);

    wxFileInputStream file_input(fileName);

    wxBufferedInputStream stream(file_input);

    wxDataInputStream data(stream);

    HeaderTZX header;
    header.Read(data);

    if (!header.IsValid())
    {
    }
    else
    {
        while (data.IsOk())
        {
            BlockTZX *bl = BlockTZX::ReadBlock(data);

            if (bl)
            {
                file->AppendBlock(std::shared_ptr<BlockTZX>(bl));
            }
            else
            {
                break;
            }
        }

        file->is_valid = (file->NumberBlocks() != 0);
    }

    return file->is_valid;
}


BlockTZX *BlockTZX::ReadBlock(wxDataInputStream &stream)
{
    Type::ID id = (Type::ID)stream.Read8();

    std::unique_ptr<BlockTZX> block;

    if (id == Type::ID::_10_Standard_speed_data_block)
    {
        block = std::make_unique<BlockTZX_10>(stream);
    }
    else if (id == Type::ID::_30_Text_description)
    {
        block = std::make_unique<BlockTZX_30>(stream);
    }
    else
    {
        return nullptr;
    }

    BlockTZX *result = (BlockTZX *)block->Read();
    if (result)
    {
        block.release();  // владение передаётся вызывающему коду
    }

    return result;
}



Block *BlockTZX_10::Read()
{
    pause_after_block = stream.Read16();
    length_data = stream.Read16();

    for (uint16 i = 0; i < length_data; i++)
    {
        data.push_back(stream.Read8());
    }

    return this;
}


Block *BlockTZX_30::Read()
{
    int num_sybmols = stream.Read8();

    for (int i = 0; i < num_sybmols; i++)
    {
        text.Append((char)stream.Read8());
    }

    return this;
}


void HeaderTZX::Read(wxDataInputStream &stream)
{
    uint8 *pointer = (uint8 *)this;

    for (uint i = 0; i < sizeof(*this); i++)
    {
        *pointer++ = stream.Read8();
    }
}


bool HeaderTZX::IsValid() const
{
    pchar key = "ZXTape!";

    if (std::memcmp(signature, key, 7) != 0)
    {
        return false;
    }

    if (marker_end_of_text != 0x1A)
    {
        return false;
    }

    return true;
}


void BlockTZX::WriteType(Writer &writer, const wxString &type, int dx)
{
    writer.DrawNextString(wxString::Format("Type : %s", type.c_str().AsChar()), dx);
}


void BlockTZX_30::Write(Writer &writer, int dx)
{
    WriteType(writer, "Text description", dx);

    writer.DrawNextString(wxString::Format("Description : %s", text), dx);
}


void BlockTZX_10::Write(Writer &writer, int dx)
{
    WriteType(writer, "Standard Speed Data Block", dx);

    writer.DrawNextString(wxString::Format("Pause after this block : %u ms", pause_after_block), dx);

    writer.DrawNextString(wxString::Format("Length of data : %u", length_data), dx);
}
