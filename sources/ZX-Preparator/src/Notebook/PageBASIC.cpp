// 2022/05/23 09:52:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageBASIC.h"
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
    dc.SetFont(*wxSWISS_FONT);
    dc.SetPen(*wxGREEN_PEN);

    dc.SetFont(font);

    wxSize size = GetSize();

    int x = 0;
    int y = 0;

    for (LineBASIC line : lines)
    {
        bool fill = false;

        WriteText(dc, wxString::Format("%d %d ", line.number, line.size), x, y, fill);

        for (SymbolBASIC symbol : line.symbols)
        {
            fill = !fill;

            WriteText(dc, symbol.string, x, y, fill);
        }

        y += font.GetPixelSize().y + 4;
        x = 0;
    }

    dc.DrawText(wxString::Format("%d : %d", GetSize().x, GetSize().y), x, y);
}
