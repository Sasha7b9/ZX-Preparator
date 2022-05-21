#include "defines.h"
#include "Page.h"


Page::Page(wxNotebook *parent, int _index) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL),
    index(_index)
{
    SetBackgroundColour(*wxWHITE);
    SetScrollbars(20, 20, 50, 50);

    if (index == 0 || index == 2)
    {
        SetScrollbars(20, 20, 10, 10);
    }

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);
}


void Page::WriteText(wxDC &dc, const wxString &text, int &x, int &y, bool fill)
{
    wxSize size = dc.GetTextExtent(text);

    if (x + size.x > GetSize().x)
    {
        x = 0;
        y += font.GetPixelSize().y;
    }

    if (fill)
    {
        dc.SetBrush(*wxLIGHT_GREY_BRUSH);
        dc.SetPen(*wxLIGHT_GREY_PEN);
        dc.DrawRectangle(x, y, size.x, size.y);
    }

    dc.DrawText(text, x, y);
    x += size.x;
}


void Page::OnDraw(wxDC &dc)
{
    // vars to use ...
    wxPen wP;
    wxBrush wB;
    wxPoint points[6];
    wxColour wC;

    dc.SetFont(*wxSWISS_FONT);
    dc.SetPen(*wxGREEN_PEN);

    switch (index)
    {
    case 0:
        {
            dc.SetFont(font);

            wxSize size = GetSize();

            int x = 0;
            int y = 0;

            for (Line line : lines)
            {
                bool fill = false;

                WriteText(dc, wxString::Format("%d %d ", line.number, line.size), x, y, fill);

                for (Symbol symbol : line.symbols)
                {
                    fill = !fill;

                    WriteText(dc, symbol.string, x, y, fill);
                }

                y += font.GetPixelSize().y;
                x = 0;
            }

            dc.DrawText(wxString::Format("%d : %d", GetSize().x, GetSize().y), x, y);
        }
        break;


    case 1:
        // draw lines to make a cross
        dc.DrawLine(0, 0, 200, 200);
        dc.DrawLine(200, 0, 0, 200);
        // draw point colored line and spline
        wP = *wxCYAN_PEN;
        wP.SetWidth(3);
        dc.SetPen(wP);

        dc.DrawPoint(25, 15);
        dc.DrawLine(50, 30, 200, 30);
        dc.DrawSpline(50, 200, 50, 100, 200, 10);
        break;

    case 2:
    case 3:
        // draw standard shapes
        dc.SetBrush(*wxCYAN_BRUSH);
        dc.SetPen(*wxRED_PEN);
        dc.DrawRectangle(10, 10, 100, 70);
        wB = wxBrush("DARK ORCHID", wxBRUSHSTYLE_TRANSPARENT);
        dc.SetBrush(wB);
        dc.DrawRoundedRectangle(50, 50, 100, 70, 20);
        dc.SetBrush(wxBrush("GOLDENROD"));
        dc.DrawEllipse(100, 100, 100, 50);

        points[0].x = 100; points[0].y = 200;
        points[1].x = 70; points[1].y = 260;
        points[2].x = 160; points[2].y = 230;
        points[3].x = 40; points[3].y = 230;
        points[4].x = 130; points[4].y = 260;
        points[5].x = 100; points[5].y = 200;

        dc.DrawPolygon(5, points);
        dc.DrawLines(6, points, 160);
        break;
    }
}


void Page::SetLines(std::vector<Line> &_lines)
{
    for (Line line : _lines)
    {
        lines.push_back(line);
    }

    Refresh();
}


void Page::OnSizeEvent(wxSizeEvent &)
{
    Refresh();
}
