// 2022/05/26 11:37:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/ControlPanel.h"
#pragma warning(push, 0)
#include <wx/button.h>
#pragma warning(pop)


ControlPanel::ControlPanel(wxWindow *parent) : wxPanel(parent)
{
//    SetMaxSize({ 100, 100000 });
//    SetMinSize({ 100, 1 });

    wxButton *button = new wxButton(this, wxID_ANY, "Open");

    button->SetPosition({ 10, 10 } );
}
