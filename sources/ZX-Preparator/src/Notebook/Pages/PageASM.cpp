// 2022/05/23 09:51:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Pages/PageASM.h"
#include "Frame.h"
#include "Utils/Timer.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#pragma warning(pop)


void PageASM::OnPaint(wxPaintEvent &)
{
    TimerMS timer;

    if (program.lines.empty())
    {
        return;
    }

    static wxSize prev_size { 0, 0 };

    wxPaintDC dc(this);
    PrepareDC(dc);

    hdc = &dc;

    int x = margin_x;
    int y = margin_y;

    dc.SetFont(font);

//    int pos = GetScrollPos(wxVERTICAL);

    for (LineASM line : program.lines)
    {
        DrawLine(x, y, line);
    }

//    SetScrollbars(sbPPU, sbPPU, 10, (y + font.GetPointSize()) / sbPPU, 0, pos, true);

    Frame::Self()->SetTitle(wxString::Format("ZX-Preparator %d ms", timer.ElapsedTime()));

    prev_size = GetSize();
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

    y += dY;
}
