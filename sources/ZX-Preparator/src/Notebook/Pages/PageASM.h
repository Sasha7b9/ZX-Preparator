// 2022/05/23 10:03:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages/Pages.h"
#include "Parser/ParserStructures.h"


class PageASM : public Page
{
public:

    ProgramASM program;

    PageASM(wxNotebook *parent, pchar name) : Page(parent, TypePage::ASM, name) { }

    void SetProgram(ProgramASM &);
};


class CanvasASM : public Canvas
{
public:

    CanvasASM(wxWindow *);

    virtual void OnEventPaint(wxPaintEvent &) override;

private:

    void DrawLine(int x, int &y, const LineASM &);
};
