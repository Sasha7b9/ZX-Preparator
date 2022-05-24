// 2022/05/23 10:00:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#include <wx/dc.h>
#pragma warning(pop)


struct TypePage
{
    enum E
    {
        Info,
        HEX,
        BASIC,
        ArrayNumber,
        ArrayCharacter,
        ASM,
        PureBytes,
        Count
    };
};


class Page : public wxScrolledWindow
{
public:

    Page(wxNotebook* parent, TypePage::E, pchar name);

    virtual void OnDraw(wxDC&) override {};
    void OnSizeEvent(wxSizeEvent&);

protected:

    TypePage::E type = TypePage::Count;

    wxFont font;

    wxDC *hdc;

    const int margin_x = 10;
    const int margin_y = 10;
    int dY = 0;
    int sbPPU = 0;                    // Scrollbar pixels for unit
};
