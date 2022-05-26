// 2022/05/23 09:52:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageBASIC.h"
#include "Utils/Timer.h"
#include "Frame.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#pragma warning(pop)


void PageBASIC::SetProgram(ProgramBASIC &program)
{
    for (LineBASIC line : program.lines)
    {
        lines.push_back(line);
    }

    Refresh();
}


void CanvasBASIC::OnEventPaint(wxPaintEvent &)
{
    TimerMS timer;

    static wxSize prev_size{ 0, 0 };

    if (lines.empty())
    {
        return;
    }

    wxPaintDC dc(this);
    PrepareDC(dc);

    hdc = &dc;

    dc.SetFont(*wxSWISS_FONT);
    dc.SetPen(*wxGREEN_PEN);

    dc.SetFont(font);

    wxSize size = GetSize();

    int y = margin_y;

    int y_min = 0;          // Redraw symbols on y from y_min
    int y_max = 10000;      // to y_max

    int pos = GetScrollPos(wxVERTICAL);

    if (prev_size == GetSize())
    {
        y_min = pos * sbPPU;
        y_max = y_min + prev_size.y;
        y_min -= 500;
    }

    const int x0 = margin_x + width_line_field;
    int x = x0;

    for (LineBASIC line : lines)
    {
        x = x0;
        bool fill = false;
        bool draw = y > y_min && y < y_max;

        if (draw)
        {
            dc.DrawText(wxString::Format("% 5d", line.number), margin_x, y);
        }

        for (SymbolBASIC symbol : line.symbols)
        {
            fill = !fill;

            WriteText(symbol.string, x, y, fill, draw);
        }

        y += font.GetPixelSize().y + 10;
    }

//    SetScrollbars(sbPPU, sbPPU, 10, (y + font.GetPointSize()) / sbPPU, 0, pos, true);

    Frame::Self()->SetTitle(wxString::Format("ZX-Preparator %d ms", timer.ElapsedTime()));

    prev_size = GetSize();
}


void PageBASIC::WriteText(const wxString &text, int &x, int &y, bool fill, bool draw)
{
    wxSize size = hdc->GetTextExtent(text);

    if (x + size.x > GetSize().x - 20)
    {
        x = margin_x + width_line_field;
        y += dY;
    }

    if (fill && draw)
    {
        static wxColour colour(240, 240, 240);
        static wxBrush brush(colour, wxBRUSHSTYLE_SOLID);
        static wxPen pen(colour);

        hdc->SetBrush(brush);
        hdc->SetPen(pen);
        hdc->DrawRectangle(x, y, size.x, size.y);
    }

    if (draw)
    {
        hdc->DrawText(text, x, y);
    }

    x += size.x;
}
