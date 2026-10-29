// 2022/5/22 9:25:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Notebook.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageHEX.h"
#include "Notebook/PageBASIC.h"
#include "Notebook/PageASM.h"
#include "Notebook/PageArrayNumber.h"
#include "Notebook/PageArrayCharacter.h"
#include "Notebook/PagePureBytes.h"
#include "Parser/ParserTZX.h"
#include "Parser/ParserHEX.h"
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


void Notebook::AssignFile(const wxString &file_name)
{
    if (!wxFile::Exists(file_name))
    {
        return;
    }

    Clear();

    ParserTAP tap;

    if (tap.Execute(file_name))
    {
        FillTAP(tap);

        return;
    }


    ParserTZX tzx;

    if (tzx.Execute(file_name))
    {
        FillTZX(tzx);

        return;
    }

    ParserHEX hex;

    hex.Execute(file_name);

    FillHEX(hex);
}


void Notebook::Clear()
{
    RemoveAllPages();

    GetPageInfo()->SetDescription(nullptr);
}


void Notebook::FillTAP(ParserTAP &parser)
{
    GetPageInfo()->SetDescription(parser.desc);

    PageHEX *pageHEX = new PageHEX(this);

    pageHEX->SetDump(parser.file.dump);

    AppendPage(pageHEX);

    if (parser.desc->valid)
    {
        int counter = 1;
        int num_BASIC = 1;
        int num_ArrayNumber = 1;
        int num_ArrayCharacter = 1;
        int num_ASM = 1;
        int num_pure = 1;

        for (auto b : parser.file.blocks)
        {
            auto block = std::dynamic_pointer_cast<BlockTAP>(b);

            if (block)
            {
                if (block->IsBASIC())
                {
                    PageBASIC *page = new PageBASIC(this, wxString::Format("%d - BASIC %d", counter++, num_BASIC++));

                    ProgramBASIC program;

                    block->ParseBASIC(program);

                    page->SetProgram(program);

                    AppendPage(page);
                }
                else if (block->IsArrayNumber())
                {
                    AppendPage(new PageArrayNumber(this, wxString::Format("%d - Array Number %d", counter++, num_ArrayNumber++)));
                }
                else if (block->IsArrayCharacter())
                {
                    AppendPage(new PageArrayCharacter(this, wxString::Format("%d - Array Character %d", counter++, num_ArrayCharacter++)));
                }
                else if (block->IsASM())
                {
                    PageASM *page = new PageASM(this, wxString::Format("%d - ASM %d", counter++, num_ASM++));

                    ProgramASM program;

                    block->ParseASM(program);

                    page->SetProgram(program);

                    AppendPage(page);
                }
                else if (block->IsPureBytes())
                {
                    AppendPage(new PagePureBytes(this, wxString::Format("%d - Pure %d", counter++, num_pure++)));
                }
            }

            if (counter > 32)
            {
                break;
            }
        }
    }
}


void Notebook::FillTZX(ParserTZX &)
{

}


void Notebook::FillHEX(ParserHEX &parser)
{
    GetPageInfo()->SetDescription(parser.desc);

    PageHEX *pageHEX = new PageHEX(this);

    pageHEX->SetDump(parser.file.dump);

    AppendPage(pageHEX);
}
