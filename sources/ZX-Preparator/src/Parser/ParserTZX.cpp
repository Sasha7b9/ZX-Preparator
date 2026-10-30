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

    file->header.Read(data);

    if (!file->header.IsValid())
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
    else if (id == Type::ID::_15_Direct_recording_block)
    {
        block = std::make_unique<BlockTZX_15>(stream);
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
    else if (id == Type::ID::_33_Hardware_type)
    {
        block = std::make_unique<BlockTZX_33>(stream);
    }
    else if (id == Type::ID::_35_Custom_info_block)
    {
        block = std::make_unique<BlockTZX_35>(stream);
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


Block *BlockTZX_15::Read()
{
    number_t_states = stream.Read16();
    pause_after_this_block = stream.Read16();
    used_bits_in_last_byte_of_data = stream.Read8();

    {
        // Читаем три байта значения

        length_of_samples_data = 0;

        uint8 *pointer = (uint8 *)&length_of_samples_data;

        for (int i = 0; i < 3; i++)
        {
            *pointer++ = stream.Read8();
        }

        for (uint i = 0; i < length_of_samples_data; i++)
        {
            samples_data.push_back(stream.Read8());
        }
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


Block *BlockTZX_33::Read()
{
    uint8 num = stream.Read8();

    for (uint8 i = 0; i < num; i++)
    {
        HWINFO info;

        info.type = stream.Read8();
        info.id = stream.Read8();
        info.info = stream.Read8();

        hwinfo.push_back(info);
    }

    return this;
}


Block *BlockTZX_35::Read()
{
    for (int i = 0; i < 16; i++)
    {
        id_string.Append((char)stream.Read8());
    }

    size_t length_custom_info = stream.Read32();

    for (size_t i = 0; i < length_custom_info; i++)
    {
        custom_info.Append((char)stream.Read8());
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


void BlockTZX_15::Write(Writer &writer, int dx)
{
    WriteType(writer, "Direct Recording", dx);

    writer.DrawNextString(wxString::Format("Number of T-states per sample (bit of data) : %u", number_t_states), dx);

    writer.DrawNextString(wxString::Format("Pause after this block : %u ms", pause_after_this_block), dx);

    writer.DrawNextString(wxString::Format("Used bits (samples) in last byte of data (1-8) : %u", used_bits_in_last_byte_of_data), dx);

    writer.DrawNextString(wxString::Format("Length of samples' data : %u bytes", length_of_samples_data), dx);
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
    else if(type != 0xFF)
    {
        _id = wxString::Format("!!! Warning !!! Unknown ID text %d", (int)type);
    }

    return wxString::Format("%s : %s", _id, string);
}


void BlockTZX_33::Write(Writer &writer, int dx)
{
    WriteType(writer, "Hardware type", dx);

    size_t size = hwinfo.size();

    for (size_t i = 0; i < size; i++)
    {
        writer.DrawNextString(wxString::Format("HWINFO %d", (int)i), dx + 1);

        HWINFO &hw = hwinfo[i];

        writer.DrawNextString(wxString::Format("Hardware type : %u", hw.type), dx + 2);
        writer.DrawNextString(wxString::Format("Hardware ID : %u", hw.id), dx + 2);

        wxString info{ "Unknown" };

        if (hw.info == 0)
        {
            info = "The tape RUNS on this machine or with this hardware, but may or may not use the hardware or special features of the machine";
        }
        else if (hw.info == 1)
        {
            info = "The tape USES the hardware or special features of the machine, such as extra memory or a sound chip";
        }
        else if (hw.info == 2)
        {
            info = "The tape RUNS but it DOESN'T use the hardware or special features of the machine";
        }
        else if (hw.info == 3)
        {
            info = "The tape DOESN'T RUN on this machine or with this hardware";
        }

        writer.DrawNextString(wxString::Format("Hardware info : %s", info), dx + 2);
    }
}


void BlockTZX_35::Write(Writer &writer, int dx)
{
    WriteType(writer, "Custom info block", dx);

    writer.DrawNextString(wxString::Format("Identification string : %s", id_string), dx + 1);
    writer.DrawNextString(wxString::Format("Custom info : %s", custom_info), dx + 1);
}
