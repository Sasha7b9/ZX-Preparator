#pragma once
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#include <wx/notebook.h>
#pragma warning(pop)


class Page : public wxScrolledWindow
{
public:

    Page(wxNotebook *parent, int index);

    virtual void OnDraw(wxDC &) override;

    void SetLines(std::vector<std::string> &);

private:

    int index = -1;

    std::vector<std::string> lines;
};
