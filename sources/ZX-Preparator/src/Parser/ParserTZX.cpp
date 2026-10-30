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
                while (data.IsOk())
                {
                    data.Read8();
                    file->number_unreaded_bytes++;
                }

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
    else if (id == Type::ID::_20_Pause_or_Stop_The_Tape_command)
    {
        block = std::make_unique<BlockTZX_20>(stream);
    }
    else if (id == Type::ID::_21_Group_start)
    {
        block = std::make_unique<BlockTZX_21>(stream);
    }
    else if (id == Type::ID::_30_Text_description)
    {
        block = std::make_unique<BlockTZX_30>(stream);
    }
    else if (id == Type::ID::_32_Archive_info)
    {
        block = std::make_unique<BlockTZX_32>(stream);
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


Block *BlockTZX_20::Read()
{
    pause_duration = stream.Read16();

    return this;
}


Block *BlockTZX_21::Read()
{
    uint8 length = stream.Read8();

    for (size_t i = 0; i < length; i++)
    {
        group_name.Append((char)stream.Read8());
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


Block *BlockTZX_32::Read()
{
    length_whole_block = stream.Read16();
    number_text_strings = stream.Read8();

    for (uint8 i = 0; i < number_text_strings; i++)
    {
        Text text;
        text.type = (Text::Type::E)stream.Read8();

        uint8 size = stream.Read8();

        for (int s = 0; s < size; s++)
        {
            text.string.Append((char)stream.Read8());
        }

        strings.push_back(text);
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
    writer.DrawNextString(wxString::Format("Type ID %X : %s", id, type.c_str().AsChar()), dx);
}


void BlockTZX_10::Write(Writer &writer, int dx)
{
    WriteType(writer, "Standard Speed Data Block", dx);

    writer.DrawNextString(wxString::Format("Pause after this block : %u ms", pause_after_block), dx);

    writer.DrawNextString(wxString::Format("Length of data : %u", length_data), dx);
}


void BlockTZX_20::Write(Writer &writer, int dx)
{
    WriteType(writer, "Pause (silence) or 'Stop the Tape' command", dx);

    writer.DrawNextString(wxString::Format("duration : %u ms", pause_duration), dx);
}


void BlockTZX_21::Write(Writer &writer, int dx)
{
    WriteType(writer, "Group start", dx);

    writer.DrawNextString(wxString::Format("Group name : %s", group_name), dx);
}


void BlockTZX_30::Write(Writer &writer, int dx)
{
    WriteType(writer, "Text description", dx);

    writer.DrawNextString(wxString::Format("Description : %s", text), dx);
}


void BlockTZX_32::Write(Writer &writer, int dx)
{
    WriteType(writer, "Archive info", dx);

    writer.DrawNextString(wxString::Format("Length of the whole block : %u bytes", length_whole_block), dx);

    for (size_t i = 0; i < strings.size(); i++)
    {
        writer.DrawNextString(strings[i].GetString(), dx + 1);
    }
}


wxString BlockTZX_32::Text::GetString() const
{
    wxString _id{ "Comment(s)" };

    static const wxString ids[] =
    {
        "Full title",
        "Software house/publisher",
        "Author(s)",
        "Year of publication",
        "Language",
        "Game/utility type",
        "Price",
        "Protection scheme/loader",
        "Origin"
    };

    if (type < std::size(ids))
    {
        _id = ids[type];
    }
    else
    {
        _id = "!!! Warning !!! Unknown ID text";
    }

    return wxString::Format("%s : %s", _id, string);
}
