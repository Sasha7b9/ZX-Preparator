// 2022/05/20 15:48:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "ParserTAP.h"
#pragma warning (push, 0)
#include <wx/statline.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/datstrm.h>
#pragma warning (pop)


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


bool BlockTAP::Parse(std::vector<std::string> &lines)
{
    return false;
}
