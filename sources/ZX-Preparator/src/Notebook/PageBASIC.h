// 2022/05/23 09:52:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"
#include "Parser/ParserStructures.h"


class PageBASIC : public Page
{
public:

    PageBASIC(wxNotebook *parent, pchar name) : Page(parent, TypePage::BASIC, name) { }

    void SetProgram(ProgramBASIC &);

private:

    std::vector<LineBASIC> lines;

    void WriteText(const wxString &, int &x, int &y, bool fill, bool draw);

    const int width_line_field = 65;
};


class CanvasBASIC : public Canvas
{
public:

    CanvasBASIC(Page *);


    virtual void OnEventPaint(wxPaintEvent &) override;
};
