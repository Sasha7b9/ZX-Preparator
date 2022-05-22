#pragma once
#include "ParserTAP.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#include <wx/notebook.h>
#pragma warning(pop)


struct TypePage
{
    enum E
    {
        Info,
        Binary,
        Parsed,
        Edit,
        Count
    };
};


class Page : public wxScrolledWindow
{
public:

    Page(wxNotebook *parent, TypePage::E type, pchar _name);

    virtual void OnDraw(wxDC &) override;
    void OnSizeEvent(wxSizeEvent &);

    void SetLines(std::vector<Line> &);

private:

    TypePage::E type = TypePage::Count;

    std::vector<Line> lines;

    wxFont font;

    pchar name;

    void WriteText(wxDC &, const wxString &text, int &x, int &, bool fill);
};


class PageInfo : public Page
{
public:
    PageInfo(wxNotebook *parent) : Page(parent, TypePage::Info, "Info") { }
};


class PageBinary : public Page
{
public:
    PageBinary(wxNotebook *parent) : Page(parent, TypePage::Binary, "Binary") { }
};


class PageParsed : public Page
{
public:
    PageParsed(wxNotebook *parent) : Page(parent, TypePage::Parsed, "Parsed") { }
};


class PageEdit : public Page
{
public:
    PageEdit(wxNotebook *parent) : Page(parent, TypePage::Edit, "Edit") { }
};


class Notebook : public wxNotebook
{
public:
    Notebook(wxWindow *parent);

    Page *GetPageInfo() { return (Page *)GetPage(TypePage::Info); }

    Page *GetPageBinary() { return (Page *)GetPage(TypePage::Binary); }

    Page *GetPageParsed() { return (Page *)GetPage(TypePage::Parsed); }

    Page *GetPageEdit() { return (Page *)GetPage(TypePage::Edit); }

private:

    virtual wxWindow *GetPage(size_t index) const override { return wxNotebook::GetPage(index); }
};
