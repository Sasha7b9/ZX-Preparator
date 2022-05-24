// 2022/05/23 09:52:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"
#include "Parser/ParserStructures.h"


class PageBASIC : public Page
{
public:

    PageBASIC(wxNotebook *parent, pchar name) : Page(parent, TypePage::BASIC, name) { }

    virtual void OnDraw(wxDC &) override;

    void SetProgram(ProgramBASIC &);

private:

    std::vector<LineBASIC> lines;

    void WriteText(wxDC &, const wxString &, int &x, int &, bool fill);
};
