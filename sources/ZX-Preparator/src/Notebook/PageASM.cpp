// 2022/05/23 09:51:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageASM.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#pragma warning(pop)


void PageASM::OnDraw(wxDC &dc)
{
    hdc = &dc;

    int x = 10;
    int y = 10;

    dc.SetFont(font);

    for (LineASM line : program.lines)
    {
        DrawLine(x, y, line);
    }
}


void PageASM::SetProgram(ProgramASM &_program)
{
    program = _program;

    Refresh();
}


void PageASM::DrawLine(int &x, int &y, const LineASM &line)
{
    hdc->DrawText(wxString::Format("%04X  %s", line.address, line.symbols[0].string.c_str()), x, y);

    y += font.GetPointSize() + 5;
}
