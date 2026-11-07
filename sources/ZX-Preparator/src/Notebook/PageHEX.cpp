// 2022/05/23 09:52:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/PageHEX.h"
#include "Utils/Timer.h"
#include "Frame.h"
#include "Controls/Controls.h"
#include <algorithm>


void PageHEX::SetDump(DumpHEX &_dump)
{
    dump = _dump;

    wxSize size = canvas->GetVirtualSize();

    size.y *= 2;

    canvas->SetVirtualSize(size);

    Refresh();

    canvas->Refresh();
}


void CanvasHEX::OnEventPaint(wxPaintEvent &)
{
    DumpHEX &dump = ((PageHEX *)page)->dump;

    if (dump.bytes.empty())
    {
        return;
    }

    static wxSize prev_size { 0, 0 };

    TimerMS timer;

    PaintDC dc(this);
    PrepareDC(dc);

    int x = margin_x;
    int y = 0;

    uint16 address = 0;

    int y_min = 0;          // Redraw symbols on y from y_min
    int y_max = 10000;      // to y_max

    int pos = GetScrollPos(wxVERTICAL);

    if (prev_size == GetSize())
    {
        y_min = pos * dc.GetSB_PPU() - dc.GetSB_PPU();
        y_max = y_min + prev_size.y + 8 * dc.GetSB_PPU();
    }

    const size_t CHUNK = 16;

    for (size_t off = 0; off < dump.bytes.size(); off += CHUNK)
    {
        std::span<const uint8> chunk(dump.bytes.data() + off, std::min(CHUNK, dump.bytes.size() - off));

        if (y > y_min && y < y_max)
        {
            WriteBytes(dc, address, chunk, x, y);
        }

        address += 16;
        y += dc.GetDY();
    }

    SetScrollbars(
        dc.GetSB_PPU(),                                         // pixelsPerUnitX - шаг прокрутки в пикселях по горизонтали
        dc.GetSB_PPU(),                                         // pixelsPerUnitY - шаг прокрутки в пикселях по вертикали
        10,                                                     // noUnitsX - количество шагов прокрутки по горизонтали
        (y + dc.GetFont().GetPointSize()) / dc.GetSB_PPU(),     // noUnitsY - количество шагов прокрутки по вертикли
        0,                                                      // xPos - начальная позиция полосы прокрутки в шагах по горизонтали
        pos,                                                    // yPos - начальная позиция полосы прокрутки в шагах по вертикали
        true                                                    // noRefresh - если true, окно не будет автоматически перерисоываться после вызова SetScrollbars
    );

    Frame::Self()->SetTitle(wxString::Format("ZX-Preparator %d ms", timer.ElapsedTime()));

    prev_size = GetSize();
}


void CanvasHEX::WriteBytes(PaintDC &dc, uint16 address, std::span<const uint8> &span, int x, int y)
{
    wxString hex;
    hex.reserve(span.size() * 3 + 1);

    for (size_t i = 0; i < span.size(); i++)
    {
        if (i == 8)
        {
            hex += ' ';
        }

        hex += wxString::Format("%02X ", span[i]);
    }

    // Дополняем пробелами, если размер меньше 16
    for (size_t i = span.size(); i < 16; i++)
    {
        if (i == 8)
        {
            hex += ' ';
        }
        hex += "   ";
    }

    dc.DrawText(wxString::Format("%04X %s", address, hex), x, y);
}


ControlPanelHEX::ControlPanelHEX(wxWindow *parent) : ControlPanel(parent)
{

}


CanvasHEX::CanvasHEX(Page *parent) : Canvas(parent)
{
    Bind(wxEVT_PAINT, &CanvasHEX::OnEventPaint, this);
}
