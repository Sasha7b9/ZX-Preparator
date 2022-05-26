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

    canvas = new Canvas(this);

    control_panel = new ControlPanel(this);

//    sizer->Add(canvas, 0, wxALL);

//    sizer->Add(control_panel, 0, wxALIGN_RIGHT);

//    SetSizerAndFit(sizer);

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);

    dY = font.GetPointSize() + 5;
    sbPPU = dY;
}


void Page::OnSizeEvent(wxSizeEvent &)
{
    wxSize size = GetSize();

    wxSize size_panel = size;
    size_panel.x = 300;

    control_panel->SetSize(size_panel);

    wxSize size_canvas = size;
    size_canvas.x = size.x - size_panel.x;

    canvas->SetSize(size_canvas);

    Refresh();
}
