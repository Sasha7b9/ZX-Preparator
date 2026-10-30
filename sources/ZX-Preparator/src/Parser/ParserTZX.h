// 2026/10/28 22:08:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Parser/Parser.h"


struct Writer;


struct HeaderTZX
{
    char signature[7];          // TZX signature = "ZXTape!"
    uint8 marker_end_of_text;   // 0x1A
    uint8 revision_major;       // 1    // Последняя версия - 1.20
    uint8 revision_minor;       // 20   //

    void Read(wxDataInputStream &);

    bool IsValid() const;
};


struct BlockTZX : public Block
{
    BlockTZX(wxDataInputStream &stream) : Block(stream)
    {
    }

    virtual Block *Read() override
    {
        return nullptr;
    }

    static BlockTZX *ReadBlock(wxDataInputStream &);

    virtual void Write(Writer &, int /*dx*/) override  { }

protected:

    void WriteType(Writer &, const wxString &type, int dx);

    struct Type
    {
        enum ID
        {
            _10_Standard_speed_data_block = 0x10,
            _11_Turbo_speed_data_block = 0x11,
            _12_Pure_tone = 0x12,
            _13_Sequence_of_pulses_of_various_lengths = 0x13,
            _14_Pure_data_block = 0x14,
            _15_Direct_recording_block = 0x15,
            _18_CSW_recording_block = 0x18,
            _19_Generalized_data_block = 0x19,
            _20_Pause_or_Stop_The_Tape_command = 0x20,
            _21_Group_start = 0x21,
            _22_Group_end = 0x22,
            _23_Jump_to_block = 0x23,
            _24_Loop_start = 0x24,
            _25_Loop_end = 0x25,
            _26_Call_sequence = 0x26,
            _27_Return_from_sequence = 0x27,
            _28_Select_block = 0x28,
            _2A_Stop_the_tape_if_in_48K_mode = 0x2A,
            _2B_Set_signal_level = 0x2B,
            _30_Text_description = 0x30,
            _31_Message_block = 0x31,
            _32_Archive_info = 0x32,
            _33_Hardware_type = 0x33,
            _35_Custom_info_block = 0x35,
            _5A_Glue_block_90_dec, _ASCII_Letter_Z = 0x5A
        };
    };

    BlockTZX(wxDataInputStream &stream, Type::ID _id) : Block(stream), id(_id)
    {}

    Type::ID id;
};


struct BlockTZX_10 : public BlockTZX
{
    BlockTZX_10(wxDataInputStream &stream) : BlockTZX(stream, BlockTZX::Type::ID::_10_Standard_speed_data_block) { }

    virtual Block *Read() override;

    virtual void Write(Writer &, int dx) override;

private:

    uint16 pause_after_block;
    uint16 length_data;
    std::vector<uint8> data;
};


struct BlockTZX_15 : public BlockTZX
{
    BlockTZX_15(wxDataInputStream &stream) : BlockTZX(stream, BlockTZX::Type::ID::_15_Direct_recording_block)
    {}

    virtual Block *Read() override;

    virtual void Write(Writer &, int dx) override;

private:

    uint16 number_t_states;                 // Number of T-states per sample (bit of data)
    uint16 pause_after_this_block;          // Pause after this block in milliseconds (ms.)
    uint8  used_bits_in_last_byte_of_data;  // Used bits (samples) in last byte of data (1-8) (e.g. if this is 2, only first two samples of the last byte will be played)
    uint  length_of_samples_data;           // 3 byte Length of samples' data
    std::vector<uint8> samples_data;        // Samples data. Each bit represents a state on the EAR port (i.e. one sample). MSb is played first.
};


struct BlockTZX_20 : public BlockTZX
{
    BlockTZX_20(wxDataInputStream &stream) : BlockTZX(stream, BlockTZX::Type::ID::_20_Pause_or_Stop_The_Tape_command) { }

    virtual Block *Read() override;

    virtual void Write(Writer &, int dx) override;

private:

    uint16 pause_duration;
};


struct BlockTZX_21 : public BlockTZX
{
    BlockTZX_21(wxDataInputStream &stream) : BlockTZX(stream, BlockTZX::Type::ID::_21_Group_start)
    {}

    virtual Block *Read() override;

    virtual void Write(Writer &, int dx) override;

private:

    wxString group_name;    // Group name in ASCII format (please keep it under 30 characters long)
};


struct BlockTZX_30 : public BlockTZX
{
    BlockTZX_30(wxDataInputStream &stream) : BlockTZX(stream, BlockTZX::Type::ID::_30_Text_description) { }

    virtual Block *Read() override;

    virtual void Write(Writer &, int dx) override;

private:

    wxString text;
};


struct BlockTZX_32 : public BlockTZX
{
    BlockTZX_32(wxDataInputStream &stream) : BlockTZX(stream, BlockTZX_32::Type::ID::_32_Archive_info) { }

    virtual Block *Read() override;

    virtual void Write(Writer &, int dx) override;

private:

    uint16 length_whole_block = 0;      // Length of the whole block (without these two bytes)
    uint8 number_text_strings = 0;      // Number of text strings

    struct Text
    {
        struct Type
        {
            enum E
            {
                _00_Full_title,
                _01_Software_house_publisher,
                _02_Author,
                _03_Year_of_publication,
                _04_Language,
                _05_Game_utility_type,
                _06_Price,
                _07_Protection_scheme_loader,
                _08_Origin,
                _FF_Comment = 0xFF
            };
        };

        Type::E type;

        wxString string;

        wxString GetString() const;
    };

    std::vector<Text> strings;  // List of text strings
};


struct BlockTZX_33 : public BlockTZX
{
    BlockTZX_33(wxDataInputStream &stream) : BlockTZX(stream, BlockTZX::Type::ID::_33_Hardware_type)
    {}

    virtual Block *Read() override;

    virtual void Write(Writer &, int dx) override;

private:

    struct HWINFO
    {
        uint8 type;
        uint8 id;
        uint8 info;
    };

    std::vector<HWINFO> hwinfo;
};


struct FileTZX : public File
{
    HeaderTZX header;
};


class ParserTZX
{
public:

    bool Execute(pchar fileName);

    std::shared_ptr<FileTZX> file;
};
