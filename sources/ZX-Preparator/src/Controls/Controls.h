// 2026/11/03 12:57:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/wfstream.h>
    #include <wx/dc.h>
#pragma warning(pop)


class BufferedInputStream : public wxBufferedInputStream
{
public:

    BufferedInputStream(wxFileInputStream &s) : wxBufferedInputStream(s) { }

    void StorePointer()
    {
        pointer = TellI();
    }

    void RestorePointer()
    {
        SeekI(pointer);
    }

private:

    wxFileOffset pointer = 0;
};



class PaintDC : public wxPaintDC
{
public:

    PaintDC(wxWindow *);

    wxFont &GetFont()
    {
        return font;
    }

    int GetDY() const
    {
        return dY;
    }

    int GetSB_PPU() const
    {
        return sbPPU;
    }

private:

    wxFont font;
    int dY = 0;
    int sbPPU = 0;      // Scrollbar pixels for unit
};
