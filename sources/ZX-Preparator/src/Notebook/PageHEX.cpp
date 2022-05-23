// 2022/05/23 09:52:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageHEX.h"
#pragma warning(push, 0)
#include <wx/dc.h>
#pragma warning(pop)


void PageHEX::SetDump(DumpHEX &_dump)
{
    dump = _dump;

    Refresh();
}


void PageHEX::OnDraw(wxDC &dc)
{
    if (dump.bytes.empty())
    {
        return;
    }

    dc.SetFont(font);

    int x = 10;
    int y = 10;

    uint16 address = 0;

    for (uint index = 0; index < dump.bytes.size(); index += 16)
    {
        int num = 16;

        if (index + 16 > dump.bytes.size())
        {
            num = (int)(dump.bytes.size() - index);
        }

        WriteBytes(dc, &address, &dump.bytes.data()[index], num, x, y);

        if (index > 1024)
        {
            break;
        }
    }
}


void PageHEX::WriteBytes(wxDC &dc, uint16 *address, uint8 *data, int num, int x, int &y)
{
    dc.DrawText(wxString::Format("%05X", *address), x, y);

    x += 70;

    for (int i = 0; i < num; i++)
    {
        dc.DrawText(wxString::Format("%02X", data[i]), x, y);

        x += 25;

        if (i == 7)
        {
            x += 10;
        }
    }

    *address += 16;

    y += font.GetPointSize() + 5;
}
