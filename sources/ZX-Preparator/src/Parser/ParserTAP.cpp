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
            0,              // 00
            0,              // 01
            0,              // 02
            0,              // 03
            0,              // 04
            0,              // 05
            0,              // 06
            0,              // 07
            0,              // 08
            0,              // 09
            0,              // 0A
            0,              // 0B
            0,              // 0C
            0,              // 0D
            0,              // 0E
            0,              // 0F
            0,              // 10
            0,              // 11
            0,              // 12
            0,              // 13
            0,              // 14
            0,              // 15
            0,              // 16
            0,              // 17
            0,              // 18
            0,              // 19
            0,              // 1A
            0,              // 1B
            0,              // 1C
            0,              // 1D
            0,              // 1E
            0,              // 1F
            " ",            // 20
            "!",            // 21
            "\"",           // 22
            "#",            // 23
            "$",            // 24
            "%%",           // 25
            "&",            // 26
            "`",            // 27
            "(",            // 28
            ")",            // 29
            "*",            // 2A
            "+",            // 2B
            ",",            // 2C
            "-",            // 2D
            ".",            // 2E
            "/",            // 2F
            "0",            // 30
            "1",            // 31
            "2",            // 32
            "3",            // 33
            "4",            // 34
            "5",            // 35
            "6",            // 36
            "7",            // 37
            "8",            // 38
            "9",            // 39
            ":",            // 3A
            ";",            // 3B
            "<",            // 3C
            "=",            // 3D
            ">",            // 3E
            "?",            // 3F
            "@",            // 40
            "A",            // 41
            "B",            // 42
            "C",            // 43
            "D",            // 44
            "E",            // 45
            "F",            // 46
            "G",            // 47
            "H",            // 48
            "I",            // 49
            "J",            // 4A
            "K",            // 4B
            "L",            // 4C
            "M",            // 4D
            "N",            // 4E
            "O",            // 4F
            "P",            // 50
            "Q",            // 51
            "R",            // 52
            "S",            // 53
            "T",            // 54
            "U",            // 55
            "V",            // 56
            "W",            // 57
            "X",            // 58
            "Y",            // 59
            "Z",            // 5A
            "[",            // 5B
            "/",            // 5C
            "]",            // 5D
            0,              // 5E
            "_",            // 5F
            0,              // 60
            "a",            // 61
            "b",            // 62
            "c",            // 63
            "d",            // 64
            "e",            // 65
            "f",            // 66
            "g",            // 67
            "h",            // 68
            "i",            // 69
            "j",            // 6A
            "k",            // 6B
            "l",            // 6C
            "m",            // 6D
            "n",            // 6E
            "o",            // 6F
            "p",            // 70
            "q",            // 71
            "r",            // 72
            "s",            // 73
            "t",            // 74
            "u",            // 75
            "v",            // 76
            "w",            // 77
            "x",            // 78
            "y",            // 79
            "z",            // 7A
            "{",            // 7B
            "|",            // 7C
            "}",            // 7D
            "~",            // 7E
            0,              // 7F
            0,              // 80
            0,              // 81
            0,              // 82
            0,              // 83
            0,              // 84
            0,              // 85
            0,              // 86
            0,              // 87
            0,              // 88
            0,              // 89
            0,              // 8A
            0,              // 8B
            0,              // 8C
            0,              // 8D
            0,              // 8E
            0,              // 8F
            0,              // 90
            0,              // 91
            0,              // 92
            0,              // 93
            0,              // 94
            0,              // 95
            0,              // 96
            0,              // 97
            0,              // 98
            0,              // 99
            0,              // 9A
            0,              // 9B
            0,              // 9C
            0,              // 9D
            0,              // 9E
            0,              // 9F
            0,              // A0
            0,              // A1
            0,              // A2
            0,              // A3
            0,              // A4
            "RND ",         // A5
            "INKEY$ ",      // A6
            "PI ",          // A7
            "FN ",          // A8
            "POINT ",       // A9
            "SCREEN$ ",     // AA
            "ATTR ",        // AB
            "AT ",          // AC
            "TAB ",         // AD
            "VAL$ ",        // AE
            "CODE ",        // AF
            "VAL ",         // B0
            "LEN ",         // B1
            "SIN ",         // B2
            "COS ",         // B3
            "TAN ",         // B4
            "ASN ",         // B5
            "ACS ",         // B6
            "ATN ",         // B7
            "LN ",          // B8
            "EXP ",         // B9
            "INT ",         // BA
            "SQR ",         // BB
            "SGN ",         // BC
            "ABS ",         // BD
            "PEEK ",        // BE
            "IN ",          // BF
            "USR ",         // C0
            "STR# ",        // C1
            "CHR# ",        // C2
            "NOT ",         // C3
            "BIN ",         // C4
            "OR ",          // C5
            "AND ",         // C6
            "<=",           // C7
            ">=",           // C8
            "<>",           // C9
            "LINE ",        // CA
            "THEN ",        // CB
            "TO ",          // CC
            "STEP ",        // CD
            "DEF FN ",      // CE
            "CAT ",         // CF
            "FORMAT ",      // D0
            "MOVE ",        // D1
            "ERASE ",       // D2
            "OPEN# ",       // D3
            "CLOSE# ",      // D4
            "MERGE ",       // D5
            "VERIFY ",      // D6
            "BEEP ",        // D7
            "CIRCLE ",      // D8
            "INK ",         // D9
            "PAPER ",       // DA
            "FLASH ",       // DB
            "BRIGHT ",      // DC
            "INVERSE ",     // DD
            "OVER ",        // DE
            "OUT ",         // DF
            "LPRINT ",      // E0
            "LLIST ",       // E1
            "STOP ",        // E2
            "READ ",        // E3
            "DATA ",        // E4
            "RESTORE ",     // E5
            "NEW ",         // E6
            "BORDER ",      // E7
            "CONTINUE ",    // E8
            "DIM ",         // E9
            "REM ",         // EA
            "FOR ",         // EB
            "GOTO ",        // EC
            "GOSUB ",       // ED
            "INPUT ",       // EE
            "LOAD ",        // EF
            "LIST ",        // F0
            "LET ",         // F1
            "PAUSE ",       // F2
            "NEXT ",        // F3
            "POKE ",        // F4
            "PRINT ",       // F5
            "PLOT ",        // F6
            "RUN ",         // F7
            "SAVE ",        // F8
            "RANDOMIZE ",   // F9
            "IF ",          // FA
            "CLS ",         // FB
            "DRAW ",        // FC
            "CLEAR ",       // FD
            "RETURN ",      // FE
            "COPY "         // FF
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


bool ParserTAP::Execute(pchar fileName)
{
    desc.Clear();

    file.Clear();

    if (!wxFile::Exists(fileName))
    {
        return false;
    }

    file.dump.FillFrom(fileName);

    wxFileInputStream file_input(fileName);

    wxBufferedInputStream stream(file_input);

    wxDataInputStream data(stream);

    BlockTAP block(data);

    while (data.IsOk())
    {
        if (block.Read())
        {
            file.AppendBlock(block);
            DescriptionTAP::Block descBlock(block.header.file_name, block.header.type_data, block.header.size_data, block.header.param1, block.header.param2);
            desc.numberBlocks++;
            desc.blocks.push_back(descBlock);
        }
        else
        {
            break;
        }
    }

    desc.valid = (file.NumberBlocks() != 0);

    desc.unused_memory = 0;

    while (data.IsOk())
    {
        data.Read8();
        desc.unused_memory++;
    }

    return desc.valid;
}


bool BlockTAP::Read()
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
    else if (size != 0)
    {
        type_data = 4;
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

    if (header.type_data == 4)
    {
        size = header.size_data;

        for (int i = 0; i < header.size_data + 1; i++)
        {
            data.push_back(Read8());
        }

        valid = stream.IsOk();

        if (stream.Read8() != crc)
        {
            valid = false;
        }
    }
    else
    {
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


bool BlockTAP::ParseBASIC(ProgramBASIC &program)
{
    program.Clear();

    if (header.type_data == 0)
    {
        return ParseProgram(program);
    }

    return false;
}


bool BlockTAP::ParseProgram(ProgramBASIC &program)
{
    while (data.data.size())
    {
        program.lines.push_back(ParseLineProgram());
    }

    return true;
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


void FileTAP::Clear()
{
    blocks.clear();
    dump.Clear();
};
