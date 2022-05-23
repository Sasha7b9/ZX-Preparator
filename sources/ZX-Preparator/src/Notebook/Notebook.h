// 2022/5/22 10:31:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Parser/ParserTAP.h"
#include "Notebook/Pages.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageHEX.h"
#include "Notebook/PageBASIC.h"
#include "Notebook/PageASM.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#include <wx/notebook.h>
#pragma warning(pop)


class Notebook : public wxNotebook
{
public:

    Notebook(wxWindow *parent);

    PageInfo *GetPageInfo() { return pageInfo; }

    PageHEX *GetPageHEX() { return pageHEX; }

    PageBASIC *GetPageBASIC() { return pageBASIC; }

    PageASM *GetPageEdit() { return pageEdit; }

private:

    PageInfo *pageInfo = nullptr;
    PageHEX *pageHEX = nullptr;
    PageBASIC *pageBASIC = nullptr;
    PageASM *pageEdit = nullptr;
};
