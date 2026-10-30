// 2022/05/23 09:52:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageInfo.h"
#include "Parser/ParserTAP.h"
#include "Parser/ParserHEX.h"
#include "Parser/ParserTZX.h"
#pragma warning(push, 0)
    #include <wx/dc.h>
    #include <wx/dcclient.h>
    #include <wx/button.h>
#pragma warning(pop)


void PageInfo::SetFile(std::shared_ptr<File> _file)
{
    file = std::move(_file);

    Refresh();
}


void CanvasInfo::OnEventPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    writer.Set(&dc, 10, 10, dY);

    auto file = ((PageInfo *)page)->file;

    if (!file || !file->is_valid)
    {
        writer.DrawNextString("Is not valid file");

        return;
    }

    if (file->ToTAP() && file->is_valid)
    {
        DrawTAP(file);
    }
    else if (file->ToTZX() && file->is_valid)
    {
        DrawTZX(file);
    }
    else if (file->ToHEX() && file->is_valid)
    {
        DrawHEX(file);
    }
}


void Writer::DrawNextString(const wxString &line, int dx)
{
    dc->DrawText(line, x + dx * 40, y);

    y += dY;
}


void CanvasInfo::DrawTAP(std::shared_ptr<File> file)
{
    auto fileTAP = std::dynamic_pointer_cast<FileTAP>(file);

    writer.DrawNextString("Is TAP file");

    writer.DrawNextString(wxString::Format("Full size %d", fileTAP->FullSize()));

    writer.DrawNextString(wxString::Format("Number blocks : %d", fileTAP->NumberBlocks()));

    for (uint i = 0; i < (uint)fileTAP->NumberBlocks(); i++)
    {
        writer.DrawNextString(wxString::Format("Block %d:", i + 1), 1);

        static const pchar types[5] =
        {
            "BASIC PROGRAM",
            "NUMBER ARRAY",
            "CHARACTER ARRAY",
            "MACHINE CODE",
            "PURE BYTES (bytes without header)"
        };

        auto block = std::dynamic_pointer_cast<BlockTAP>(fileTAP->blocks[i]);

        writer.DrawNextString(wxString::Format("Name : %s", (block->header.type < 4) ? block->header.file_name : "Unknown"), 2);

        if (block->IsCODE() && block->data.size == 6912 && block->header.param1 == 16384)
        {
            writer.DrawNextString(wxString::Format("Type : %s", "DATA SCREEN"), 2);
        }
        else
        {
            writer.DrawNextString(wxString::Format("Type : %s", types[block->header.type]), 2);
        }

        writer.DrawNextString(wxString::Format("Size : #%05x/%d", block->data.size, block->data.size), 2);

        if(block->IsBASIC())
        {
            writer.DrawNextString(wxString::Format("Line start : %d", block->header.param1), 2);

            writer.DrawNextString(wxString::Format("The start of the variable area relative to the start of the program : #%05x/%d", block->header.param2, block->header.param2), 2);
        }
        else if (block->IsArrayNumber() || block->IsArrayCharacter())
        {
            writer.DrawNextString(wxString::Format("Name : %c", (char)(block->header.param1 >> 8)), 2);
        }
        else if (block->IsCODE())
        {
            writer.DrawNextString(wxString::Format("Begin data : #%05x/%d", block->header.param1, block->header.param1), 2);
        }
    }
}


void CanvasInfo::DrawHEX(std::shared_ptr<File> file)
{
    auto fileTAP = std::dynamic_pointer_cast<FileHEX>(file);

    writer.DrawNextString("Unsupported file");

    writer.DrawNextString(wxString::Format("Full size %d", fileTAP->FullSize()));
}


void CanvasInfo::DrawTZX(std::shared_ptr<File> _file)
{
    auto file = std::dynamic_pointer_cast<FileTZX>(_file);

    writer.DrawNextString(wxString::Format("Is TZX file version %d:%d", file->header.revision_major, file->header.revision_minor));

    writer.DrawNextString(wxString::Format("Full size %d", file->FullSize()));

    writer.DrawNextString(wxString::Format("Number blocks : %d", file->NumberBlocks()));

    if (file->number_unreaded_bytes > 0)
    {
        writer.DrawNextString(wxString::Format("!!!Warning !!! Number unreaded bytes == % u", file->number_unreaded_bytes));
    }

    for (int i = 0; i < file->NumberBlocks(); i++)
    {
        auto block = std::dynamic_pointer_cast<BlockTZX>(file->blocks[i]);

        writer.DrawNextString(wxString::Format("Block %d:", i + 1), 1);

        block->Write(writer, 2);
    }
}


ControlPanelInfo::ControlPanelInfo(wxWindow *parent) : ControlPanel(parent)
{
    (new wxButton(this, wxID_ANY, "Open"))->SetPosition({ 10, 10 });
}


CanvasInfo::CanvasInfo(Page *parent) : Canvas(parent)
{
    Bind(wxEVT_PAINT, &CanvasInfo::OnEventPaint, this);
}
