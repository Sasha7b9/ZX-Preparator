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

    int pos = GetScrollPos(wxVERTICAL);

    for (LineBASIC line : lines)
    {
        bool fill = false;

        WriteText(wxString::Format("%d %d ", line.number, line.size), x, y, fill);

        for (SymbolBASIC symbol : line.symbols)
        {
            fill = !fill;

            WriteText(symbol.string, x, y, fill);
        }

        y += font.GetPixelSize().y + 10;
        x = margin_x;
    }

    SetScrollbars(sbPPU, sbPPU, 10, (y + font.GetPointSize()) / sbPPU, 0, pos, true);

    Frame::Self()->SetTitle(wxString::Format("ZX-Preparator %d ms", timer.ElapsedTime()));

    prev_size = GetSize();
}


void PageBASIC::WriteText(const wxString &text, int &x, int &y, bool fill)
{
    wxSize size = hdc->GetTextExtent(text);

    if (x + size.x > GetSize().x - 20)
    {
        x = 0;
        y += font.GetPixelSize().y + 4;
    }

    if (fill)
    {
        static wxColour colour(240, 240, 240);
        static wxBrush brush(colour, wxBRUSHSTYLE_SOLID);
        static wxPen pen(colour);

        hdc->SetBrush(brush);
        hdc->SetPen(pen);
        hdc->DrawRectangle(x, y, size.x, size.y);
    }

    hdc->DrawText(text, x, y);

    x += size.x;
}
