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


void PageBASIC::OnDraw(wxDC &dc)
{
    TimerMS timer;

    static wxSize prev_size{ 0, 0 };

    if (lines.empty())
    {
        return;
    }

    hdc = &dc;

    dc.SetFont(*wxSWISS_FONT);
    dc.SetPen(*wxGREEN_PEN);

    dc.SetFont(font);

    wxSize size = GetSize();

    int x = margin_x;
    int y = margin_y;

    int y_min = 0;          // Redraw symbols on y from y_min
    int y_max = 10000;      // to y_max

    int pos = GetScrollPos(wxVERTICAL);

    if (prev_size == GetSize())
    {
        y_min = pos * sbPPU - sbPPU;
        y_max = y_min + prev_size.y + 8 * sbPPU;
    }

    for (LineBASIC line : lines)
    {
        bool fill = false;

        WriteText(wxString::Format("%d %d ", line.number, line.size), x, y, fill, y > y_min && y < y_max);

        for (SymbolBASIC symbol : line.symbols)
        {
            fill = !fill;

            WriteText(symbol.string, x, y, fill, y > y_min && y < y_max);
        }

        y += font.GetPixelSize().y + 10;
        x = margin_x;
    }

    SetScrollbars(sbPPU, sbPPU, 10, (y + font.GetPointSize()) / sbPPU, 0, pos, true);

    Frame::Self()->SetTitle(wxString::Format("ZX-Preparator %d ms", timer.ElapsedTime()));

    prev_size = GetSize();
}


void PageBASIC::WriteText(const wxString &text, int &x, int &y, bool fill, bool draw)
{
    wxSize size = hdc->GetTextExtent(text);

    if (x + size.x > GetSize().x - 20)
    {
        x = 0;
        y += font.GetPixelSize().y + 4;
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
