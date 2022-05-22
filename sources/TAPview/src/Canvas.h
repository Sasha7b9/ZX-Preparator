// 2022/5/22 10:30:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
    long    widthChar = 0, heightChar = 0;      // size (in pixels) of one character
    int     xChars = 0, yChars = 0;             // the size (in text coords) of the window
    int     xMargin = 0, yMargin = 0;           // the margin around the text (looks nicer)
    int     xCaret = 0, yCaret = 0;             // position (in text coords) of the caret
    wxChar *text = nullptr;

    // update the geometry
    void ChangeSize();
};
