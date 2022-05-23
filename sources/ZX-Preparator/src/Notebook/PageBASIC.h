// 2022/05/23 09:52:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"
#include "Parser/ParserStructures.h"


class PageBASIC : public Page
{
public:

    PageBASIC(wxNotebook* parent) : Page(parent, TypePage::BASIC, "Parsed") { }

    virtual void OnDraw(wxDC&) override;

    void SetProgram(ProgramBASIC&);

private:

    std::vector<LineBASIC> lines;
};
