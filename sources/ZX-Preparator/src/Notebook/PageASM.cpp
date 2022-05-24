// 2022/05/23 09:51:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageASM.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#pragma warning(pop)


void PageASM::OnDraw(wxDC &dc)
{
    hdc = &dc;

    int x = margin_x;
    int y = margin_y;

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


void PageASM::DrawLine(int x, int &y, const LineASM &line)
{
    int x_start = x;

    hdc->DrawText(wxString::Format("%04X", line.address), x, y);

    x += 55;

    for (uint i = 0; i < line.codes.size(); i++)
    {
        hdc->DrawText(wxString::Format("%02X", line.codes[i]), x, y);

        x += 28;
    }

    x = x_start + 150;

    hdc->DrawText(line.symbols[0].string.c_str(), x, y);

    y += font.GetPointSize() + 5;
}
