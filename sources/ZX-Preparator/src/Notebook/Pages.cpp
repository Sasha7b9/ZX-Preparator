// 2022/05/23 10:00:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Pages.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#pragma warning(pop)


Page::Page(wxNotebook *parent, TypePage::E _type, pchar name) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL),
    type(_type)
{
    SetBackgroundColour(*wxWHITE);
    SetScrollbars(20, 20, 50, 50);

    if (type == TypePage::Info || type == TypePage::BASIC)
    {
        SetScrollbars(20, 20, 10, 10);
    }

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);

    SetName(name);
}


void Page::WriteText(wxDC &dc, const wxString &text, int &x, int &y, bool fill)
{
    wxSize size = dc.GetTextExtent(text);

    if (x + size.x > GetSize().x)
    {
        x = 0;
        y += font.GetPixelSize().y + 4;
    }

    if (fill)
    {
        static wxColour colour(240, 240, 240);
        static wxBrush brush(colour, wxBRUSHSTYLE_SOLID);
        static wxPen pen(colour);

        dc.SetBrush(brush);
        dc.SetPen(pen);
        dc.DrawRectangle(x, y, size.x, size.y);
    }

    dc.DrawText(text, x, y);
    x += size.x;
}


void Page::OnSizeEvent(wxSizeEvent &)
{
    Refresh();
}
