// 2026/10/29 09:07:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Parser/StructuresZX.h"
#include <istream>
#include <fstream>


void DumpHEX::FillFrom(pchar name_file)
{
    std::ifstream file(name_file, std::ifstream::in | std::ifstream::binary);

    file.seekg(0, std::ios::end);

    size_t size = (size_t)file.tellg();

    file.seekg(0, std::ios::beg);

    bytes.resize(size);

    file.read((char *)bytes.data(), bytes.size());
}
