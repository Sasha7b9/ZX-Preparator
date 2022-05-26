// 2022/5/22 10:30:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/scrolwin.h>
#pragma warning(pop)


class Page;


class Canvas : public wxScrolledWindow
{
public:
    Canvas(Page *);

    virtual void OnEventPaint(wxPaintEvent &) { };

protected:

    wxFont  font;
    wxDC   *hdc = nullptr;
    Page   *page = nullptr;             // parent
    const int margin_x = 10;
    const int margin_y = 10;
    int dY = 0;
    int sbPPU = 0;                      // Scrollbar pixels for unit
};
