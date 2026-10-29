// 2022/05/23 09:52:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageInfo.h"
#include "Parser/ParserTAP.h"
#include "Parser/ParserHEX.h"
#pragma warning(push, 0)
    #include <wx/dc.h>
    #include <wx/dcclient.h>
    #include <wx/button.h>
#pragma warning(pop)


void PageInfo::SetDescription(std::shared_ptr<Description> _desc)
{
    desc = std::move(_desc);

    Refresh();
}


void CanvasInfo::OnEventPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    writer.Set(&dc, 10, 10, dY);

    auto desc = ((PageInfo *)page)->desc;

    if (!desc)
    {
        writer.DrawNextString("Is not valid file");

        return;
    }

    if (desc->IsTAP() && desc->valid)
    {
        DrawTAP(desc);
    }
    else if (desc->IsTZX() && desc->valid)
    {
        DrawTZX(desc);
    }
    else if (desc->IsHEX() && desc->valid)
    {
        DrawHEX(desc);
    }
}


void CanvasInfo::Writer::DrawNextString(const wxString &line, int dx)
{
    dc->DrawText(line, x + dx * 40, y);

    y += dY;
}


void CanvasInfo::DrawTAP(std::shared_ptr<Description> desc)
{
    auto descTAP = std::dynamic_pointer_cast<DescriptionTAP>(desc);

    writer.DrawNextString("Is TAP file");

    writer.DrawNextString(wxString::Format("Full size %d", descTAP->full_size));

    if (descTAP->unused_memory)
    {
        writer.DrawNextString(wxString::Format("!!! WARNING !!! In file discovered %d bytes unused memory", descTAP->unused_memory));
    }

    writer.DrawNextString(wxString::Format("Number blocks : %d", descTAP->numberBlocks));

    for (uint i = 0; i < (uint)descTAP->numberBlocks; i++)
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

        DescriptionTAP::Block &block = descTAP->blocks[i];

        if (block.type < 4)
        {
            writer.DrawNextString(wxString::Format("Name : %s", block.name.c_str()), 2);
        }

        if (block.type == 3 && block.size == 6912 && block.param1 == 16384)
        {
            writer.DrawNextString(wxString::Format("Type : %s", "DATA SCREEN"), 2);
        }
        else
        {
            writer.DrawNextString(wxString::Format("Type : %s", types[block.type]), 2);
        }

        writer.DrawNextString(wxString::Format("Size : #%05x/%d", block.size, block.size), 2);

        if (block.type == 0)
        {
            writer.DrawNextString(wxString::Format("Line start : %d", block.param1), 2);

            writer.DrawNextString(wxString::Format("Offset data : #%05x/%d", block.param2, block.param2), 2);
        }
        else if (block.type == 1 || block.type == 2)
        {
            writer.DrawNextString(wxString::Format("Name : %c", (char)(block.param1 >> 8)), 2);
        }
        else if (block.type == 3)
        {
            writer.DrawNextString(wxString::Format("Begin data : #%05x/%d", block.param1, block.param1), 2);
        }
    }
}


void CanvasInfo::DrawHEX(std::shared_ptr<Description> desc)
{
    auto descTAP = std::dynamic_pointer_cast<DescriptionHEX>(desc);

    writer.DrawNextString("Unsupported file");

    writer.DrawNextString(wxString::Format("Full size %d", descTAP->full_size));
}


void CanvasInfo::DrawTZX(std::shared_ptr<Description> desc)
{
    auto descTAP = std::dynamic_pointer_cast<DescriptionHEX>(desc);

    writer.DrawNextString("Is TZX file");

    writer.DrawNextString(wxString::Format("Full size %d", descTAP->full_size));
}


ControlPanelInfo::ControlPanelInfo(wxWindow *parent) : ControlPanel(parent)
{
    (new wxButton(this, wxID_ANY, "Open"))->SetPosition({ 10, 10 });
}


CanvasInfo::CanvasInfo(Page *parent) : Canvas(parent)
{
    Bind(wxEVT_PAINT, &CanvasInfo::OnEventPaint, this);
}
