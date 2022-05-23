// 2022/5/22 10:31:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Parser/ParserTAP.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#include <wx/notebook.h>
#pragma warning(pop)


class PageInfo;
class PageHEX;


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

    Page(wxNotebook *parent, TypePage::E, pchar name);

    virtual void OnDraw(wxDC &) override {};
    void OnSizeEvent(wxSizeEvent &);

protected:

    TypePage::E type = TypePage::Count;

    wxFont font;

    void WriteText(wxDC &, const wxString &, int &x, int &, bool fill);
};


class PageBASIC : public Page
{
public:

    PageBASIC(wxNotebook *parent) : Page(parent, TypePage::BASIC, "Parsed") { }

    virtual void OnDraw(wxDC &) override;

    void SetProgram(ProgramBASIC &);

private:

    std::vector<LineBASIC> lines;
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

    PageInfo *GetPageInfo() { return pageInfo; }

    PageHEX *GetPageBinary() { return pageBinary; }

    PageBASIC *GetPageParsed() { return pageParsed; }

    PageEdit *GetPageEdit() { return pageEdit; }

private:

    PageInfo *pageInfo = nullptr;
    PageHEX *pageBinary = nullptr;
    PageBASIC *pageParsed = nullptr;
    PageEdit *pageEdit = nullptr;
};
