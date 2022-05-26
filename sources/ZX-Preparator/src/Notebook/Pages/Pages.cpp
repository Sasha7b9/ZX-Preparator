// 2022/05/23 10:00:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Pages/Pages.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#include <wx/sizer.h>
#pragma warning(pop)


Page::Page(wxNotebook *parent, TypePage::E _type, pchar name) :
    wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL),
    type(_type)
{
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);

    canvas = new Canvas(splitter);

    canvas->SetMaxSize({ 500, 100000 });

    control_panel = new ControlPanel(splitter);

    control_panel->SetMinSize({ 100, 10000 });

    splitter->SplitVertically(canvas, control_panel);

    sizer->Add(splitter, 1, wxEXPAND);

    SetSizerAndFit(sizer);

    SetMaxSize({ 500, 500 });

    Refresh();

    /*
    SetBackgroundColour(*wxWHITE);

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);

    SetName(name);

    dY = font.GetPointSize() + 5;
    sbPPU = dY;

    SetClientSize(splitter->GetBestSize());
    */
}


void Page::OnSizeEvent(wxSizeEvent &)
{
    Refresh();
}
