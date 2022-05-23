// 2022/5/22 10:31:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Parser/ParserTAP.h"
#include "Notebook/Pages.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#include <wx/notebook.h>
#pragma warning(pop)


class PageInfo;
class PageHEX;
class PageBASIC;


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
