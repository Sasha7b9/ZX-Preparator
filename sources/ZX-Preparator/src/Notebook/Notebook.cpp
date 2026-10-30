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
#include "Parser/ParserZ80.h"
#pragma warning(push, 0)
    #include <wx/file.h>
#pragma warning(pop)


Notebook::Notebook(wxWindow *parent) :
    wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP),
    history(this)
{
    pageInfo = new PageInfo(this);

    wxNotebook::AddPage(pageInfo, pageInfo->GetName());

    Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &Notebook::OnEventNotebookPageChanged, this);
}


void Notebook::RemoveAllPages()
{
    Unbind(wxEVT_NOTEBOOK_PAGE_CHANGED, &Notebook::OnEventNotebookPageChanged, this);

    if (pages.size())
    {
        for (size_t i = pages.size(); i > 0; i--)
        {
            DeletePage(i);
        }

        pages.clear();
    }

    Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &Notebook::OnEventNotebookPageChanged, this);
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

    history.SetCurrentFile(file_name);

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

    ParserZ80 z80;

    if (z80.Execute(file_name))
    {
        FillZ80(z80);

        return;
    }

    ParserHEX hex;

    hex.Execute(file_name);

    FillHEX(hex);
}


void Notebook::Clear()
{
    RemoveAllPages();

    GetPageInfo()->SetFile(nullptr);
}


void Notebook::FillTAP(ParserTAP &parser)
{
    GetPageInfo()->SetFile(parser.file);

    PageHEX *pageHEX = new PageHEX(this);

    pageHEX->SetDump(parser.file->dump);

    AppendPage(pageHEX);

    if (parser.file->is_valid)
    {
        int counter = 1;
        int num_BASIC = 1;
        int num_ArrayNumber = 1;
        int num_ArrayCharacter = 1;
        int num_ASM = 1;
        int num_pure = 1;

        for (auto b : parser.file->blocks)
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
                else if (block->IsCODE())
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

    history.RestoreState();
}


void Notebook::FillTZX(ParserTZX &parser)
{
    GetPageInfo()->SetFile(parser.file);

    PageHEX *pageHEX = new PageHEX(this);

    pageHEX->SetDump(parser.file->dump);

    AppendPage(pageHEX);

    history.RestoreState();
}


void Notebook::FillZ80(ParserZ80 &)
{
    history.RestoreState();
}


void Notebook::FillHEX(ParserHEX &parser)
{
    GetPageInfo()->SetFile(parser.file);

    PageHEX *pageHEX = new PageHEX(this);

    pageHEX->SetDump(parser.file->dump);

    AppendPage(pageHEX);

    history.RestoreState();
}


void NotebookHistory::SetCurrentFile(const wxString &file_name)
{
    if (files.find(file_name) == files.end())
    {
        files[file_name] = HistoryStruct(file_name);
    }

    current_file = file_name;
}


void NotebookHistory::SaveState()
{
    if (files.find(current_file) != files.end())
    {
        files[current_file].name_page = notebook->GetCurrentPage()->GetName();
    }
}


void NotebookHistory::RestoreState()
{
    if (files.find(current_file) != files.end())
    {
        wxString name_page = files[current_file].name_page;

        const size_t pageCount = notebook->GetPageCount();

        for (size_t i = 0; i < pageCount; ++i)
        {
            if (notebook->GetPageText(i) == name_page)
            {
                if (i == 0)
                {
                    i = i;
                }

                notebook->SetSelection(static_cast<int>(i));

                break;
            }
        }
    }
}


void Notebook::OnEventNotebookPageChanged(wxBookCtrlEvent &event)
{
    event.Skip();

    history.SaveState();
}
