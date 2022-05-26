// 2022/05/23 10:00:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Notebook/Pages/Pages.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#include <wx/sizer.h>
#pragma warning(pop)


Page::Page(wxNotebook *parent, TypePage::E _type, pchar name) :
    wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL),
    type(_type)
{
    SetName(name);

//    sizer = new wxBoxSizer(wxHORIZONTAL);

    splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOBORDER);

    canvas = new Canvas(splitter);

    control_panel = new ControlPanel(splitter);

    splitter->SplitVertically(canvas, control_panel, 100);

//    sizer->Add(splitter, 0, wxEXPAND);

//    SetSizerAndFit(sizer);

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);

    dY = font.GetPointSize() + 5;
    sbPPU = dY;

    splitter->SetSashPosition(100);

    Refresh();
}


void Page::OnSizeEvent(wxSizeEvent &)
{
    wxSize size = GetSize();

    Frame::Self()->SetTitle(wxString::Format("ZX-Preparator    %d : %d", size.x, size.y));

    size.x -= 10;
    size.y -= 10;

    SetSize(size);

    Refresh();
}
