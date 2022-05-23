// 2022/5/22 10:29:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Parser/ParserStructures.h"
#include <istream>
#include <fstream>


void DescriptionTAP::Clear()
{
    valid = false;

    numberBlocks = 0;
}


void DumpHEX::FillFrom(pchar name_file)
{
    std::ifstream file(name_file, std::ifstream::in | std::ifstream::binary);

    file.seekg(0, std::ios::end);

    size_t size = (size_t)file.tellg();

    file.seekg(0, std::ios::beg);

    bytes.resize(size);

    file.read((char *)bytes.data(), bytes.size());
}
