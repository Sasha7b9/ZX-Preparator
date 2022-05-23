// 2022/05/23 09:52:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageInfo.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#pragma warning(pop)


void PageInfo::SetDescriptionTAP(DescriptionTAP &desc)
{
    descTAP = desc;

    Refresh();
}


void PageInfo::OnDraw(wxDC &dc)
{
    dc.SetFont(font);

    if (descTAP.valid)
    {
        int x = 0;
        int y = 0;
        int dY = font.GetPointSize() + 10;

        dc.DrawText("Is TAP file", x, y);

        y += dY;

        dc.DrawText(wxString::Format("Number blocks : %d", descTAP.numberBlocks), x, y);

        y += dY;

        for (uint i = 0; i < (uint)descTAP.numberBlocks; i++)
        {
            dc.DrawText(wxString::Format("Block %d:", i + 1), x + 20, y);

            y += dY;

            static const pchar types[4] =
            {
                "BASIC PROGRAM",
                "NUMBER ARRAY",
                "CHARACTER ARRAY",
                "MACHINE CODE"
            };

            dc.DrawText(wxString::Format("Type : %s", types[descTAP.blocks[i].type]), x + 40, y);

            y += dY;

            dc.DrawText(wxString::Format("Size : %d bytes", descTAP.blocks[i].size), x + 40, y);

            y += dY;
        }
    }
    else
    {
        dc.DrawText("Is not TAP file", 10, 10);
    }
}
