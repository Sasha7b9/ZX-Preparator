// 2022/5/22 9:25:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Notebook.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageHEX.h"
#include "Notebook/PageBASIC.h"
#include "Notebook/PageArrayNumber.h"
#include "Notebook/PageArrayCharacter.h"
#include "Notebook/PagePureBytes.h"
#pragma warning(push, 0)
#include <wx/file.h>
#pragma warning(pop)


Notebook::Notebook(wxWindow *parent) : wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    pageInfo = new PageInfo(this);

    wxNotebook::AddPage(pageInfo, pageInfo->GetName());

//    Ge
}


void Notebook::RemoveAllPages()
{
    if (pages.size())
    {
        for (size_t i = pages.size(); i > 0; i--)
        {
            DeletePage(i);
        }

        pages.clear();
    }
}


void Notebook::AppendPage(Page *page)
{
    AddPage(page, page->GetName());

    pages.push_back(page);
}


void Notebook::AssignFIle(const wxString &file_name)
{
    if (!wxFile::Exists(file_name))
    {
        return;
    }

    RemoveAllPages();

    ParserTAP parser;

    parser.Execute(file_name, this);
}
