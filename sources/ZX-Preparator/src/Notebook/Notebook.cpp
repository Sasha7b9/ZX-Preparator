// 2022/5/22 9:25:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Notebook.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageHEX.h"
#include "Notebook/PageBASIC.h"


Notebook::Notebook(wxWindow *parent) : wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    pageInfo = new PageInfo(this);

    AddPage(pageInfo, pageInfo->GetName());
}


void Notebook::RemoveAllPages()
{
    for (uint i = 0; i < pages.size(); i++)
    {
        RemovePage(0);

        delete pages[i];
    }

    pages.clear();
}


void Notebook::AppendPage(Page *page)
{
    AddPage(page, page->GetName());

    pages.push_back(page);
}
