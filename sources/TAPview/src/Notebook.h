#pragma once
#include "ParserTAP.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#include <wx/notebook.h>
#pragma warning(pop)


class Notebook : public wxNotebook
{
public:
    Notebook(wxWindow *parent) : wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP) {}
};


class Page : public wxScrolledWindow
{
public:

    Page(wxNotebook *parent, int index);

    virtual void OnDraw(wxDC &) override;
    void OnSizeEvent(wxSizeEvent &);

    void SetLines(std::vector<Line> &);

private:

    int index = -1;

    std::vector<Line> lines;

    wxFont font;

    void WriteText(wxDC &, const wxString &text, int &x, int &, bool fill);
};
