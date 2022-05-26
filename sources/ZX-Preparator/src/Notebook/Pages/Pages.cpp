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

    sizer = new wxBoxSizer(wxHORIZONTAL);

    canvas = new Canvas(this);

    control_panel = new ControlPanel(this);

    sizer->Add(canvas, 0, wxEXPAND);

    sizer->Add(control_panel, 0, wxEXPAND);

    SetSizerAndFit(sizer);

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);

    dY = font.GetPointSize() + 5;
    sbPPU = dY;

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
