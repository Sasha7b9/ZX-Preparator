// 2022/05/20 15:48:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "ParserTAP.h"
#pragma warning (push, 0)
#include <wx/statline.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>
#pragma warning (pop)
#include <sstream>


/*
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


namespace KeyCodes
{
    const char *Get(uint8 code)
    {
        static const char * const strings[256] =
        {
            0,          // 00
            0,          // 01
            0,          // 02
            0,          // 03
            0,          // 04
            0,          // 05
            0,          // 06
            0,          // 07
            0,          // 08
            0,          // 09
            0,          // 0A
            0,          // 0B
            0,          // 0C
            0,          // 0D
            0,          // 0E
            0,          // 0F
            0,          // 10
            0,          // 11
            0,          // 12
            0,          // 13
            0,          // 14
            0,          // 15
            0,          // 16
            0,          // 17
            0,          // 18
            0,          // 19
            0,          // 1A
            0,          // 1B
            0,          // 1C
            0,          // 1D
            0,          // 1E
            0,          // 1F
            " ",        // 20
            0,          // 21
            0,          // 22
            0,          // 23
            0,          // 24
            0,          // 25
            0,          // 26
            0,          // 27
            0,          // 28
            0,          // 29
            "*",        // 2A
            "+",        // 2B
            ",",        // 2C
            0,          // 2D
            0,          // 2E
            0,          // 2F
            "0",        // 30
            "1",        // 31
            "2",        // 32
            "3",        // 33
            "4",        // 34
            "5",        // 35
            "6",        // 36
            "7",        // 37
            "8",        // 38
            "9",        // 39
            ":",        // 3A
            ";",        // 3B
            "<",        // 3C
            "=",        // 3D
            ">",        // 3E
            "?",        // 3F
            0,          // 40
            0,          // 41
            0,          // 42
            0,          // 43
            0,          // 44
            0,          // 45
            0,          // 46
            0,          // 47
            0,          // 48
            0,          // 49
            0,          // 4A
            0,          // 4B
            0,          // 4C
            0,          // 4D
            0,          // 4E
            0,          // 4F
            0,          // 50
            0,          // 51
            0,          // 52
            0,          // 53
            0,          // 54
            0,          // 55
            0,          // 56
            0,          // 57
            0,          // 58
            0,          // 59
            0,          // 5A
            0,          // 5B
            0,          // 5C
            0,          // 5D
            0,          // 5E
            0,          // 5F
            0,          // 60
            0,          // 61
            0,          // 62
            0,          // 63
            0,          // 64
            0,          // 65
            0,          // 66
            0,          // 67
            0,          // 68
            0,          // 69
            0,          // 6A
            0,          // 6B
            0,          // 6C
            0,          // 6D
            0,          // 6E
            0,          // 6F
            0,          // 70
            0,          // 71
            0,          // 72
            0,          // 73
            0,          // 74
            0,          // 75
            0,          // 76
            0,          // 77
            0,          // 78
            0,          // 79
            0,          // 7A
            0,          // 7B
            0,          // 7C
            0,          // 7D
            0,          // 7E
            0,          // 7F
            0,          // 80
            0,          // 81
            0,          // 82
            0,          // 83
            0,          // 84
            0,          // 85
            0,          // 86
            0,          // 87
            0,          // 88
            0,          // 89
            0,          // 8A
            0,          // 8B
            0,          // 8C
            0,          // 8D
            0,          // 8E
            0,          // 8F
            0,          // 90
            0,          // 91
            0,          // 92
            0,          // 93
            0,          // 94
            0,          // 95
            0,          // 96
            0,          // 97
            0,          // 98
            0,          // 99
            0,          // 9A
            0,          // 9B
            0,          // 9C
            0,          // 9D
            0,          // 9E
            0,          // 9F
            0,          // A0
            0,          // A1
            0,          // A2
            0,          // A3
            0,          // A4
            0,          // A5
            0,          // A6
            0,          // A7
            0,          // A8
            0,          // A9
            0,          // AA
            0,          // AB
            0,          // AC
            0,          // AD
            0,          // AE
            0,          // AF
            0,          // B0
            0,          // B1
            0,          // B2
            0,          // B3
            0,          // B4
            0,          // B5
            0,          // B6
            0,          // B7
            0,          // B8
            0,          // B9
            0,          // BA
            0,          // BB
            0,          // BC
            0,          // BD
            0,          // BE
            0,          // BF
            0,          // C0
            0,          // C1
            0,          // C2
            0,          // C3
            "BIN ",     // C4
            0,          // C5
            0,          // C6
            0,          // C7
            0,          // C8
            0,          // C9
            0,          // CA
            0,          // CB
            0,          // CC
            0,          // CD
            0,          // CE
            0,          // CF
            0,          // D0
            0,          // D1
            0,          // D2
            0,          // D3
            0,          // D4
            0,          // D5
            0,          // D6
            0,          // D7
            0,          // D8
            0,          // D9
            "PAPER ",   // DA
            0,          // DB
            0,          // DC
            0,          // DD
            0,          // DE
            0,          // DF
            "LPRINT ",  // E0
            0,          // E1
            0,          // E2
            "READ ",    // E3
            0,          // E4
            0,          // E5
            0,          // E6
            0,          // E7
            0,          // E8
            0,          // E9
            0,          // EA
            0,          // EB
            0,          // EC
            0,          // ED
            0,          // EE
            0,          // EF
            0,          // F0
            0,          // F1
            0,          // F2
            0,          // F3
            0,          // F4
            0,          // F5
            0,          // F6
            0,          // F7
            0,          // F8
            0,          // F9
            0,          // FA
            0,          // FB
            0,          // FC
            0,          // FD
            0,          // FE
            0           // FF
        };

        if (strings[code] == 0)
        {
            static char buffer[10] = { 0, 0 };

            sprintf_s(buffer, 10, "`%02X", code);

            return buffer;
        }

        return strings[code];
    }
}


void ParserTAP::Run(wxBufferedInputStream &stream)
{
    blocks.clear();

    wxDataInputStream data(stream);

    BlockTAP block(data);

    while (data.IsOk())
    {
        if (block.Read())
        {
            blocks.push_back(block);
        }
        else
        {
            if (data.IsOk())
            {
                blocks.clear();
            }
        }
    }
}


bool BlockTAP::Read()
{
    Clear();

    if (header.Read())
    {
        data.Read(header);
    }

    return header.IsValid() && data.IsValid();
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

    size = stream.Read16();

    if (size == 19)             // Для заголовка здесь всегда 19
    {
        if (Read8() == 0)       // Чтение флага - для заголовка 0
        {
            type_data = Read8();

            if (type_data < 4)
            {
                for (int i = 0; i < 10; i++)
                {
                    file_name[i] = (char)Read8();
                }

                file_name[10] = 0;

                size_data = Read16();

                param1 = Read16();

                param2 = Read16();

                valid = stream.IsOk();

                if (stream.Read8() != crc)
                {
                    valid = false;
                }
            }
        }
    }

    return valid;
}


bool BlockTAP::Data::Read(const Header &header)
{
    valid = false;

    crc = 0;

    data.clear();

    size = stream.Read16();

    if (size == header.size_data + 2)
    {
        if (Read8() == 0xff)        // Для данных должно быть 255
        {
            for (int i = 0; i < header.size_data; i++)
            {
                data.push_back(Read8());
            }

            valid = stream.IsOk();

            if (stream.Read8() != crc)
            {
                valid = false;
            }
        }
    }

    return valid;
}


uint8 BlockTAP::CommonStruct::Read8()
{
    uint8 byte = stream.Read8();

    crc ^= byte;

    return byte;
}


uint16 BlockTAP::CommonStruct::Read16()
{
    uint16 result = Read8();

    result |= Read8() << 8;

    return result;
}


bool BlockTAP::Parse(std::vector<Line> &lines)
{
    lines.clear();

    if (header.type_data == 0)
    {
        return ParseProgram(lines);
    }

    return false;
}


bool BlockTAP::ParseProgram(std::vector<Line> &lines)
{
    while (data.data.size())
    {
        lines.push_back(ParseLineProgram());
    }

    return true;
}


Line BlockTAP::ParseLineProgram()
{
    Line line;

    line.number = data.GetData16Reverse();
    line.size = data.GetData16();

    for (int i = 0; i < line.size - 1; i++)
    {
        uint8 code = data.GetData8();

        const char *string = KeyCodes::Get(code);

        line.symbols.push_back(string);
    }

    data.GetChar();

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
