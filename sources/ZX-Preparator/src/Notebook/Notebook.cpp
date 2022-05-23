// 2022/5/22 9:25:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Notebook.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageHEX.h"
#include "Notebook/PageBASIC.h"
#include "Notebook/PageArrayNumber.h"
#include "Notebook/PageArrayCharacter.h"


Notebook::Notebook(wxWindow *parent) : wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    pageInfo = new PageInfo(this);

    AddPage(pageInfo, pageInfo->GetName());
}


void Notebook::RemoveAllPages()
{
    for (uint i = 0; i < pages.size(); i++)
    {
        DeletePage(i + 1);
    }

    pages.clear();
}


void Notebook::AppendPage(Page *page)
{
    AddPage(page, page->GetName());

    pages.push_back(page);
}


void Notebook::AssignFIle(wxString &file_name)
{
    RemoveAllPages();

    ParserTAP parser;

    parser.Execute(file_name);

    GetPageInfo()->SetDescriptionTAP(parser.desc);

    PageHEX *pageHEX = new PageHEX(this);

    AppendPage(pageHEX);

    if (parser.desc.valid)
    {
        for (BlockTAP block : parser.blocks)
        {
            if (block.IsBASIC())
            {
                AppendPage(new PageBASIC(this));
            }
            else if (block.IsArrayNumber())
            {
                AppendPage(new PageArrayNumber(this));
            }
            else if (block.IsArrayCharacter())
            {
                AppendPage(new PageArrayCharacter(this));
            }
            else if (block.IsASM())
            {
                AppendPage(new PageASM(this));
            }
        }
    }
}
