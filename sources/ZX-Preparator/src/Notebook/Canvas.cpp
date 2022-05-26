// 2022/5/22 10:30:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Pages.h"
#pragma warning(push, 0)
#include <wx/dcclient.h>
#include <wx/caret.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#pragma warning(pop)


Canvas::Canvas(Page *parent) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER),
    page(parent)
{
    SetBackgroundColour(*wxWHITE);

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);
}

