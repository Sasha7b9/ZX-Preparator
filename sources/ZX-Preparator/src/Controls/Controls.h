// 2026/11/03 12:57:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#pragma warning(push, 0)
    #include <wx/wfstream.h>
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

