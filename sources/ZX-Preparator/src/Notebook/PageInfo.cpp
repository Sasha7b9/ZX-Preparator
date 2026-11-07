// 2022/05/23 09:52:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageInfo.h"
#include "Parser/ParserTAP.h"
#include "Parser/ParserHEX.h"
#include "Parser/ParserTZX.h"
#pragma warning(push, 0)
    #include <wx/button.h>
#pragma warning(pop)


void PageInfo::SetFile(std::shared_ptr<File> _file)
{
    file = std::move(_file);

    Refresh();
}


void CanvasInfo::OnEventPaint(wxPaintEvent &)
{
    PaintDC dc(this);
    PrepareDC(dc);

    writer.Set(&dc, 10, 10, dc.GetDY());

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

    const int number_blocks = fileTAP->NumberBlocks();

    writer.DrawNextString(wxString::Format("Number blocks : %d", number_blocks));

    if (file->number_unreaded_bytes > 0)
    {
        writer.DrawNextString(wxString::Format("!!!Warning !!! Number unreaded bytes == % u", file->number_unreaded_bytes));
    }

    for (int i = 0; i < file->NumberBlocks(); i++)
    {
        auto block = std::dynamic_pointer_cast<BlockTAP>(file->blocks[(uint64)i]);

        writer.DrawNextString(wxString::Format("Block %d:", i + 1), 1);

        block->Write(writer, 2);
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
        auto block = std::dynamic_pointer_cast<BlockTZX>(file->blocks[(uint64)i]);

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
