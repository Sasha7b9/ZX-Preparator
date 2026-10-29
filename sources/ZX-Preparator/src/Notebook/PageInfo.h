// 2022/05/23 09:52:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"
#include "Parser/Parser.h"


struct Writer
{
    void Set(wxPaintDC *_dc, int _x, int _y, int _dY)
    {
        dc = _dc;
        x = _x;
        y = _y;
        dY = _dY;
    }

    void DrawNextString(const wxString &, int dx = 0);

private:

    wxPaintDC *dc;
    int x;
    int y;
    int dY;
};


class PageInfo : public Page
{
public:

    PageInfo(wxNotebook* parent) : Page(parent, TypePage::Info, "Info") { }

    void SetFile(std::shared_ptr<File>);

    std::shared_ptr<File> file;
};


class CanvasInfo : public Canvas
{
public:

    CanvasInfo(Page *);

    virtual void OnEventPaint(wxPaintEvent &) override;

private:

    void DrawTAP(std::shared_ptr<File>);
    void DrawTZX(std::shared_ptr<File>);
    void DrawHEX(std::shared_ptr<File>);

    Writer writer;
};


class ControlPanelInfo : public ControlPanel
{
public:
    ControlPanelInfo(wxWindow *);
};
