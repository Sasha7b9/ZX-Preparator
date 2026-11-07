// 2026/11/07 12:45:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Controls.h"


PaintDC::PaintDC(wxWindow *window) :
    wxPaintDC(window)
{
    wxFont f = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    f.SetWeight(wxFONTWEIGHT_BOLD);

    SetFont(f);
    SetPen(*wxGREEN_PEN);

    dY = GetFont().GetPointSize() + 5;

    sbPPU = dY;
}


void PaintDC::SetFont(const wxFont &_font)
{
    font = _font;
    wxPaintDC::SetFont(_font);
}


void PaintDC::SetPen(const wxPen &_pen)
{
    pen = _pen;
    wxPaintDC::SetPen(_pen);
}


void PaintDC::SetBrush(const wxBrush &_brush)
{
    brush = _brush;
    wxPaintDC::SetBrush(_brush);
}
