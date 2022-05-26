// 2022/05/23 09:52:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"
#include "Parser/ParserStructures.h"


class PageHEX : public Page
{
public:
    PageHEX(wxNotebook* parent) : Page(parent, TypePage::HEX, "HEX") { }

    void SetDump(DumpHEX &);

    DumpHEX dump;
};


class CanvasHEX : public Canvas
{
public:

    CanvasHEX(Page *);

    virtual void OnEventPaint(wxPaintEvent &) override;

private:

    void WriteBytes(uint16 *address, uint8 *data, int num, int x, int &y, bool draw);
};
