// 2022/05/26 11:37:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/panel.h>
#pragma warning(pop)


/*
*   To accommodate the settings controls
*/


class ControlPanel : public wxPanel
{
public:

    static const int WIDTH = 200;

    ControlPanel(wxWindow *);

private:
};
