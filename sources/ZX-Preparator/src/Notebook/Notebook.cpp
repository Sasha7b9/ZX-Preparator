// 2022/5/22 9:25:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Notebook.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageHEX.h"
#include "Notebook/PageBASIC.h"
#include "Notebook/PageArrayNumber.h"
#include "Notebook/PageArrayCharacter.h"
#include "Notebook/PagePureBytes.h"


Notebook::Notebook(wxWindow *parent) : wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    pageInfo = new PageInfo(this);

    AddPage(pageInfo, pageInfo->GetName());
}


void Notebook::RemoveAllPages()
{
    if (pages.size())
    {
        for (uint i = pages.size(); i > 0; i--)
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


void Notebook::AssignFIle(wxString &file_name)
{
    RemoveAllPages();

    ParserTAP parser;

    parser.Execute(file_name);

    GetPageInfo()->SetDescriptionTAP(parser.desc);

    PageHEX *pageHEX = new PageHEX(this);

    pageHEX->SetDump(parser.file.dump);

    AppendPage(pageHEX);

    if (parser.desc.valid)
    {
        int counter = 1;
        int num_BASIC = 1;
        int num_ArrayNumber = 1;
        int num_ArrayCharacter = 1;
        int num_ASM = 1;
        int num_pure = 1;

        for (BlockTAP block : parser.file.blocks)
        {
            if (block.IsBASIC())
            {
                AppendPage(new PageBASIC(this, wxString::Format("%d - BASIC %d", counter++, num_BASIC++)));
            }
            else if (block.IsArrayNumber())
            {
                AppendPage(new PageArrayNumber(this, wxString::Format("%d - Array Number %d", counter++, num_ArrayNumber++)));
            }
            else if (block.IsArrayCharacter())
            {
                AppendPage(new PageArrayCharacter(this, wxString::Format("%d - Array Character %d", counter++, num_ArrayCharacter++)));
            }
            else if (block.IsASM())
            {
                AppendPage(new PageASM(this, wxString::Format("%d - ASM %d", counter++, num_ASM++)));
            }
            else if (block.IsPureBytes())
            {
                AppendPage(new PagePureBytes(this, wxString::Format("%d - Pure %d", counter++, num_pure++)));
            }

            if (counter > 32)
            {
                break;
            }
        }
    }
}
