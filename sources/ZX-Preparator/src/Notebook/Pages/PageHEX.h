// 2022/05/23 09:52:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages/Pages.h"
#include "Parser/ParserStructures.h"


class PageHEX : public Page
{
public:
    PageHEX(wxNotebook* parent) : Page(parent, TypePage::HEX, "HEX") { }

    virtual void OnPaint(wxPaintEvent &event);

    void SetDump(DumpHEX &);

private:

    DumpHEX dump;

    void WriteBytes(uint16 *address, uint8 *data, int num, int x, int &y, bool draw);
};
