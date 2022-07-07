// 2022/05/23 10:00:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Notebook/Pages.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#include <wx/sizer.h>
#pragma warning(pop)


Page::Page(wxNotebook *parent, TypePage::E _type, pchar name) :
    wxWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
    type(_type)
{
    SetName(name);

    if (type == TypePage::Info)
    {
        canvas = new CanvasInfo(this);
        control_panel = new ControlPanelInfo(this);
    }
    else if (type == TypePage::HEX)
    {
        canvas = new CanvasHEX(this);
        control_panel = new ControlPanelHEX(this);
    }
    else
    {
        canvas = new Canvas(this);
        control_panel = new ControlPanel(this);
    }

    Bind(wxEVT_SIZE, &Page::OnSizeEvent, this);
}


void Page::OnPaint(wxPaintEvent &event)
{
    canvas->Refresh();

    event.Skip();
}


void Page::OnSizeEvent(wxSizeEvent &)
{
    wxSize size = GetClientSize();

    control_panel->SetSize({ ControlPanel::WIDTH, size.y });

    canvas->SetSize({ size.x - ControlPanel::WIDTH, size.y });;

    control_panel->SetPosition({ canvas->GetSize().x, 0 });

    Refresh();
}


Canvas::Canvas(Page *parent) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER),
    page(parent)
{
    SetBackgroundColour(*wxLIGHT_GREY);

    font = wxFont(wxFontInfo(12).Family(wxFONTFAMILY_TELETYPE));
    font.SetWeight(wxFONTWEIGHT_BOLD);

    dY = font.GetPointSize() + 5;
    sbPPU = dY;
}
