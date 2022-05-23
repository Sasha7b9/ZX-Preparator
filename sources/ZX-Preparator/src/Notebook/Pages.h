// 2022/05/23 10:00:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#pragma warning(pop)


struct TypePage
{
    enum E
    {
        Info,
        HEX,
        BASIC,
        Edit,
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

    void WriteText(wxDC&, const wxString&, int& x, int&, bool fill);
};
