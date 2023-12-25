// 2022/05/23 09:52:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageInfo.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/button.h>
#pragma warning(pop)


void PageInfo::SetDescriptionTAP(DescriptionTAP &desc)
{
    descTAP = desc;

    Refresh();
}


void CanvasInfo::OnEventPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    dc.SetFont(font);

    int x = 10;
    int y = 10;

    DescriptionTAP &descTAP = ((PageInfo *)page)->descTAP;

    if (descTAP.valid)
    {
        dc.DrawText("Is TAP file", x, y);

        y += dY;

        if (descTAP.unused_memory)
        {
            dc.DrawText(wxString::Format("!!! WARNING !!! In file discovered %d bytes unused memory", descTAP.unused_memory), x, y);

            y += dY;
        }

        dc.DrawText(wxString::Format("Number blocks : %d", descTAP.numberBlocks), x, y);

        y += dY;

        for (uint i = 0; i < (uint)descTAP.numberBlocks; i++)
        {
            dc.DrawText(wxString::Format("Block %d:", i + 1), x + 20, y);

            y += dY;

            static const pchar types[5] =
            {
                "BASIC PROGRAM",
                "NUMBER ARRAY",
                "CHARACTER ARRAY",
                "MACHINE CODE",
                "PURE BYTES (bytes without header)"
            };

            DescriptionTAP::Block &block = descTAP.blocks[i];

            if (block.type < 4)
            {
                dc.DrawText(wxString::Format("Name : %s", block.name.c_str()), x + 40, y);

                y += dY;
            }

            if (block.type == 3 && block.size == 6912 && block.param1 == 16384)
            {
                dc.DrawText(wxString::Format("Type : %s", "DATA SCREEN"), x + 40, y);
            }
            else
            {
                dc.DrawText(wxString::Format("Type : %s", types[block.type]), x + 40, y);
            }

            y += dY;

            dc.DrawText(wxString::Format("Size : #%05x/%d", block.size, block.size), x + 40, y);

            y += dY;

            if (block.type == 0)
            {
                dc.DrawText(wxString::Format("Line start : %d", block.param1), x + 40, y);

                y += dY;

                dc.DrawText(wxString::Format("Offset data : #%05x/%d", block.param2, block.param2), x + 40, y);

                y += dY;
            }
            else if (block.type == 1 || block.type == 2)
            {
                dc.DrawText(wxString::Format("Name : %c", (char)(block.param1 >> 8)), x + 40, y);

                y += dY;
            }
            else if (block.type == 3)
            {
                dc.DrawText(wxString::Format("Begin data : #%05x/%d", block.param1, block.param1), x + 40, y);

                y += dY;
            }
        }
    }
    else
    {
        dc.DrawText("Is not TAP file", x, y);
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
