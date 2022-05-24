// 2022/5/22 10:30:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Canvas.h"
#pragma warning(push, 0)
#include <wx/dcclient.h>
#include <wx/caret.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#pragma warning(pop)


Canvas::Canvas(wxWindow *parent) :
    wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER)
{
    text = (wxChar *)NULL;

    SetBackgroundColour(*wxWHITE);

    SetFontSize(12);

    xCaret = 0;
    yCaret = 0;
    xChars = 0;
    yChars = 0;

    xMargin = 5;
    yMargin = 5;

    CreateCaret();
}


Canvas::~Canvas()
{
    free(text);
}


void Canvas::SetFontSize(int fontSize)
{
    font = wxFont(wxFontInfo(fontSize).Family(wxFONTFAMILY_TELETYPE));

    wxClientDC dc(this);

    dc.SetFont(font);
    heightChar = dc.GetCharHeight();
    widthChar = dc.GetCharWidth();

    wxCaret *caret = GetCaret();

    if (caret)
    {
        caret->SetSize(widthChar, heightChar);

        ChangeSize();
    }
}


void Canvas::ChangeSize()
{
    wxSize size = GetClientSize();
    xChars = (size.x - 2 * xMargin) / widthChar;
    yChars = (size.y - 2 * yMargin) / heightChar;
    if (!xChars)
        xChars = 1;
    if (!yChars)
        yChars = 1;

    free(text);
    text = (wxChar *)calloc((size_t)(xChars * yChars), sizeof(wxChar));

    wxFrame *frame = wxDynamicCast(GetParent(), wxFrame);
    //wxFrame *frame = (wxFrame *)GetParent();

    if (frame && frame->GetStatusBar())
    {
        wxString msg;
        msg.Printf("Panel size is (%d, %d)", xChars, yChars);
        frame->SetStatusText(msg, 1);
    }
}


void Canvas::CreateCaret()
{
    wxCaret *caret = new wxCaret(this, widthChar, heightChar);
    SetCaret(caret);

    caret->Move(xMargin, yMargin);
    caret->Show();
}
