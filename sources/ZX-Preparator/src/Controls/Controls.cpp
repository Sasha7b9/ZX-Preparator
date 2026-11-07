// 2026/11/07 12:45:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/Controls.h"


PaintDC::PaintDC(wxWindow *window) :
    wxPaintDC(window)
{
    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    SetFont(*wxSWISS_FONT);
    SetPen(*wxGREEN_PEN);

    SetFont(font);

    dY = font.GetPointSize() + 5;

    sbPPU = dY;
}
