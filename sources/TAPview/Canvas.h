#pragma once
#pragma warning(push, 0)
#include <wx/scrolwin.h>
#pragma warning(pop)


class Canvas : public wxScrolledWindow
{
public:
    Canvas() { }
    Canvas(wxWindow *parent);
    ~Canvas();

    void SetFontSize(int);
    void CreateCaret();

private:

    wxFont  font;
    long    widthChar, heightChar;      // size (in pixels) of one character
    int     xChars, yChars;             // the size (in text coords) of the window
    int     xMargin, yMargin;           // the margin around the text (looks nicer)
    wxChar *text = nullptr;

    // update the geometry
    void ChangeSize();
};
