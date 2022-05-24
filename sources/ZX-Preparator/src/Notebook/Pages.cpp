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

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);

    SetName(name);

    dY = font.GetPointSize() + 5;
    sbPPU = dY;
}


void Page::OnSizeEvent(wxSizeEvent &)
{
    Refresh();
}
