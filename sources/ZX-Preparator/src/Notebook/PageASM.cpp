// 2022/05/23 09:51:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageASM.h"
#include "Frame.h"
#include "Utils/Timer.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#pragma warning(pop)


CanvasASM::CanvasASM(Page *parent) : Canvas(parent)
{
    Bind(wxEVT_PAINT, &CanvasASM::OnEventPaint, this);
}


void CanvasASM::OnEventPaint(wxPaintEvent &)
{
    TimerMS timer;

    ProgramASM &program = ((PageASM *)page)->program;

    if (program.lines.empty())
    {
        return;
    }

    wxPaintDC dc(this);
    PrepareDC(dc);

    int x = margin_x;
    int y = margin_y;

    dc.SetFont(font);

//    int pos = GetScrollPos(wxVERTICAL);

    for (LineASM line : program.lines)
    {
        DrawLine(&dc, x, y, line);
    }

//    SetScrollbars(sbPPU, sbPPU, 10, (y + font.GetPointSize()) / sbPPU, 0, pos, true);

    Frame::Self()->SetTitle(wxString::Format("ZX-Preparator %d ms", timer.ElapsedTime()));
}


void PageASM::SetProgram(ProgramASM &_program)
{
    program = _program;

    Refresh();
}


void CanvasASM::DrawLine(wxDC *hdc, int x, int &y, const LineASM &line)
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


ControlPanelASM::ControlPanelASM(wxWindow *parent) : ControlPanel(parent)
{

}
