// 2026/10/29 09:12:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/ParserTZX.h"
#include "Notebook/PageInfo.h"
#pragma warning (push, 0)
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
    else if (id == Type::ID::_11_Turbo_speed_data_block)
    {
        block = std::make_unique<BlockTZX_11>(stream);
    }
    else if (id == Type::ID::_12_Pure_tone)
    {
        block = std::make_unique<BlockTZX_12>(stream);
    }
    else if (id == Type::ID::_13_Sequence_of_pulses_of_various_lengths)
    {
        block = std::make_unique<BlockTZX_13>(stream);
    }
    else if (id == Type::ID::_14_Pure_data_block)
    {
        block = std::make_unique<BlockTZX_14>(stream);
    }
    else if (id == Type::ID::_15_Direct_recording_block)
    {
        block = std::make_unique<BlockTZX_15>(stream);
    }
    else if (id == Type::ID::_18_CSW_recording_block)
    {
        block = std::make_unique<BlockTZX_18>(stream);
    }
    else if (id == Type::ID::_20_Pause_or_Stop_The_Tape_command)
    {
        block = std::make_unique<BlockTZX_20>(stream);
    }
    else if (id == Type::ID::_21_Group_start)
    {
        block = std::make_unique<BlockTZX_21>(stream);
    }
    else if (id == Type::ID::_22_Group_end)
    {
        block = std::make_unique<BlockTZX_22>(stream);
    }
    else if (id == Type::ID::_23_Jump_to_block)
    {
        block = std::make_unique<BlockTZX_23>(stream);
    }
    else if (id == Type::ID::_24_Loop_start)
    {
        block = std::make_unique<BlockTZX_24>(stream);
    }
    else if (id == Type::ID::_25_Loop_end)
    {
        block = std::make_unique<BlockTZX_25>(stream);
    }
    else if (id == Type::ID::_28_Select_block)
    {
        block = std::make_unique<BlockTZX_28>(stream);
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
        if (id != 0)
        {
            id = id;
        }
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
    pause_after_block = Read16();

    length_data = Read16();
    data.resize(length_data);
    Read8(data.data(), length_data);

    return this;
}


Block *BlockTZX_11::Read()
{
    length_PILOT_pulse = Read16();
    length_SYNC_first_pulse = Read16();
    length_SYNC_second_pulse = Read16();
    length_ZERO_bit_pulse = Read16();
    length_ONE_bit_pulse = Read16();
    length_PILOT_tone = Read16();
    used_bits_in_last_byte = Read8();

    uint length = 0;
    Read8((uint8 *)&length, 3);

    data.resize(length);
    Read8(data.data(), length);

    return this;
}


Block *BlockTZX_12::Read()
{
    length_one_pulse = Read16();
    number_pulses = Read16();

    return this;
}


Block *BlockTZX_13::Read()
{
    uint8 number = Read8();

    pulses_lengths.resize(number);

    Read16(pulses_lengths.data(), number);

    return this;
}


Block *BlockTZX_14::Read()
{
    length_ZERO_bit_pulse = Read16();
    length_ONE_bit_pulse = Read16();
    used_bits_in_last_byte = Read8();
    pause_after_this_block = Read16();

    uint length = 0;
    Read8((uint8 *)&length, 3);

    data.resize(length);
    Read8(data.data(), length);

    return this;
}


Block *BlockTZX_15::Read()
{
    number_t_states = Read16();
    pause_after_this_block = Read16();
    used_bits_in_last_byte_of_data = Read8();

    {
        uint length = 0;
        Read8((uint8 *)&length, 3);

        samples_data.resize(length);
        Read8(samples_data.data(), length);
    }

    return this;
}


Block *BlockTZX_18::Read()
{
    block_length = Read32();
    pause_after_this_block = Read16();
    Read8((uint8 *)&sampling_rate, 3);
    compression_type = Read8();

    uint size = Read32();
    csw_data.resize(size);
    Read8(csw_data.data(), size);

    return this;
}


Block *BlockTZX_20::Read()
{
    pause_duration = Read16();

    return this;
}


Block *BlockTZX_21::Read()
{
    uint8 length = Read8();

    for (size_t i = 0; i < length; i++)
    {
        group_name.Append((char)Read8());
    }

    return this;
}


Block *BlockTZX_22::Read()
{
    return this;
}


Block *BlockTZX_23::Read()
{
    relative_jump_value = Read16();

    return this;
}


Block *BlockTZX_24::Read()
{
    number_repetitions = Read16();

    return this;
}


Block *BlockTZX_25::Read()
{
    return this;
}


Block *BlockTZX_28::Read()
{
    length_block = Read16();

    uint8 number = Read8();

    for (uint8 i = 0; i < number; i++)
    {
        SELECT select;

        select.relative_offset = Read16();

        uint8 length = Read8();

        for (uint8 l = 0; l < length; l++)
        {
            select.description.Append((char)Read8());
        }

        selects.push_back(select);
    }

    return this;
}


Block *BlockTZX_30::Read()
{
    int num_sybmols = Read8();

    for (int i = 0; i < num_sybmols; i++)
    {
        text.Append((char)Read8());
    }

    return this;
}


Block *BlockTZX_32::Read()
{
    length_whole_block = Read16();
    number_text_strings = Read8();

    for (uint8 i = 0; i < number_text_strings; i++)
    {
        Text text;
        text.type = (Text::Type::E)Read8();

        uint8 size = Read8();

        for (int s = 0; s < size; s++)
        {
            text.string.Append((char)Read8());
        }

        strings.push_back(text);
    }

    return this;
}


Block *BlockTZX_33::Read()
{
    uint8 num = Read8();

    for (uint8 i = 0; i < num; i++)
    {
        HWINFO info;

        info.type = Read8();
        info.id = Read8();
        info.info = Read8();

        hwinfo.push_back(info);
    }

    return this;
}


Block *BlockTZX_35::Read()
{
    for (int i = 0; i < 16; i++)
    {
        id_string.Append((char)Read8());
    }

    size_t length_custom_info = Read32();

    for (size_t i = 0; i < length_custom_info; i++)
    {
        custom_info.Append((char)Read8());
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


void BlockTZX_11::Write(Writer &writer, int dx)
{
    WriteType(writer, "Turbo Speed Data Block", dx);

    writer.DrawNextString(wxString::Format("Length of PILOT pulse : %u", length_PILOT_pulse), dx);
}


void BlockTZX_12::Write(Writer &writer, int dx)
{
    WriteType(writer, "Pure Tone", dx);

    writer.DrawNextString(wxString::Format("Length of one pulse : %u T-states", length_one_pulse), dx);

    writer.DrawNextString(wxString::Format("Number of pulses", number_pulses), dx);
}


void BlockTZX_13::Write(Writer &writer, int dx)
{
    WriteType(writer, "Pulse sequence", dx);

    writer.DrawNextString(wxString::Format("Number of pulses : %zu", pulses_lengths.size()), dx);
}


void BlockTZX_14::Write(Writer &writer, int dx)
{
    WriteType(writer, "Pure Data Block", dx);

    writer.DrawNextString(wxString::Format("Length of ZERO bit pulse : %u", length_ZERO_bit_pulse), dx);
    writer.DrawNextString(wxString::Format("Length of ONE bit pulse : %u", length_ONE_bit_pulse), dx);
    writer.DrawNextString(wxString::Format("Used bits in last byte : %u", used_bits_in_last_byte), dx);
    writer.DrawNextString(wxString::Format("Pause after this block : %u ms", pause_after_this_block), dx);
    writer.DrawNextString(wxString::Format("Size data tap file : %zu bytes", data.size()), dx);
}


void BlockTZX_15::Write(Writer &writer, int dx)
{
    WriteType(writer, "Direct Recording", dx);

    writer.DrawNextString(wxString::Format("Number of T-states per sample (bit of data) : %u", number_t_states), dx);

    writer.DrawNextString(wxString::Format("Pause after this block : %u ms", pause_after_this_block), dx);

    writer.DrawNextString(wxString::Format("Used bits (samples) in last byte of data (1-8) : %u", used_bits_in_last_byte_of_data), dx);

    writer.DrawNextString(wxString::Format("Length of samples' data : %d bytes", (int)samples_data.size()), dx);
}


void BlockTZX_18::Write(Writer &writer, int dx)
{
    WriteType(writer, "CSW Recording", dx);

    writer.DrawNextString(wxString::Format("Block length : %u bytes", block_length), dx);
    writer.DrawNextString(wxString::Format("Block length : %u ms", pause_after_this_block), dx);
    writer.DrawNextString(wxString::Format("Sampling rate : %u", sampling_rate), dx);

    wxString compression{ wxString::Format("Unknown value : %u", compression_type) };

    if (compression_type == 1)
    {
        compression = "RLE";
    }
    else if (compression_type == 2)
    {
        compression = "Z-RLE";
    }
    writer.DrawNextString(wxString::Format("Compression type : %s", compression), dx);

    writer.DrawNextString(wxString::Format("Number of stored pulses(after decompression, for validation purposes) : %u", number_stored_pulses), dx);

    writer.DrawNextString(wxString::Format("Length CSW data : %u bytes", csw_data.size()), dx);
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


void BlockTZX_22::Write(Writer &writer, int dx)
{
    WriteType(writer, "Group end", dx);
}


void BlockTZX_23::Write(Writer &writer, int dx)
{
    WriteType(writer, "Jump to block", dx);

    writer.DrawNextString(wxString::Format("Relative jump value : %u", relative_jump_value), dx);
}


void BlockTZX_24::Write(Writer &writer, int dx)
{
    WriteType(writer, "Loop start", dx);

    writer.DrawNextString(wxString::Format("Number of repetitions : %u", number_repetitions), dx);
}


void BlockTZX_25::Write(Writer &writer, int dx)
{
    WriteType(writer, "Loop end", dx);
}


void BlockTZX_28::Write(Writer &writer, int dx)
{
    WriteType(writer, "Select block", dx);

    writer.DrawNextString(wxString::Format("Length block : %u", length_block), dx);

    for (size_t i = 0; i < selects.size(); i++)
    {
        writer.DrawNextString(wxString::Format("SELECT %zu", i), dx);
        writer.DrawNextString(wxString::Format("Relative offset : %u", selects[i].relative_offset), dx + 1);
        writer.DrawNextString(wxString::Format("Description : %s", selects[i].description), dx + 1);
    }
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
