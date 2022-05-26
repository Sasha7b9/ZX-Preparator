// 2022/05/23 10:00:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Pages/Pages.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#pragma warning(pop)


Page::Page(wxNotebook *parent, TypePage::E _type, pchar name) :
    wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL),
    type(_type)
{
    splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);

    splitter->SetMinSize({ 100, 100 });

    canvas = new Canvas(splitter);

    canvas->SetMinClientSize({ 100, 100 });

    control_panel = new ControlPanel(splitter);

    splitter->SplitVertically(canvas, control_panel);

    SetBackgroundColour(*wxWHITE);

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);

    SetName(name);

    dY = font.GetPointSize() + 5;
    sbPPU = dY;

    SetClientSize(splitter->GetBestSize());
}


void Page::OnSizeEvent(wxSizeEvent &)
{
    Refresh();
}
