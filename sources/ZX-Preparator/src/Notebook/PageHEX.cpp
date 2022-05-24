// 2022/05/23 09:52:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageHEX.h"
#include "Utils/Timer.h"
#include "Frame.h"
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
    TimerMS timer;

    hdc = &dc;

    if (dump.bytes.empty())
    {
        return;
    }

    dc.SetFont(font);

    int x = margin_x;
    int y = margin_y;

    uint16 address = 0;

    for (uint index = 0; index < dump.bytes.size(); index += 16)
    {
        int num = 16;

        if (index + 16 > dump.bytes.size())
        {
            num = (int)(dump.bytes.size() - index);
        }

        WriteBytes(&address, &dump.bytes.data()[index], num, x, y);
    }

    int pos = GetScrollPos(wxVERTICAL);

    SetScrollbars(sbPPU, sbPPU, 10, (y + font.GetPointSize()) / sbPPU, 0, pos, true);

    Frame::Self()->SetTitle(wxString::Format("ZX-Preparator %d ms", timer.ElapsedTime()));
}


void PageHEX::WriteBytes(uint16 *address, uint8 *data, int num, int x, int &y)
{
    if (num == 16)
    {
        wxString text = wxString::Format("%04X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
            *address, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

        hdc->DrawText(text, x, y);

        y += dY;

        *address = *address + 16;
    }
}
