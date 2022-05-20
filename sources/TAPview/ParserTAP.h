// 2022/05/20 15:47:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <vector>


class wxBufferedInputStream;


struct BlockTAP
{

};


class ParserTAP
{
public:

    void Run(wxBufferedInputStream &);

    std::vector<BlockTAP> blocks;

private:

    BlockTAP ParseBlock(wxBufferedInputStream &);
};
