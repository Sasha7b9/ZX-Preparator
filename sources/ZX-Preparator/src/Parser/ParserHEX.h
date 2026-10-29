// 2026/10/29 11:09:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Parser/Parser.h"


struct DescriptionHEX : public Description
{
};


struct FileHEX : public File
{

};


class ParserHEX
{
public:

    bool Execute(pchar fileName);

    FileHEX file;

    std::shared_ptr<DescriptionHEX> desc;
};
