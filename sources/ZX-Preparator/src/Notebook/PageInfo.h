// 2022/05/23 09:52:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"
#include "Parser/Parser.h"


class PageInfo : public Page
{
public:

    PageInfo(wxNotebook* parent) : Page(parent, TypePage::Info, "Info") { }

    void SetDescription(std::shared_ptr<Description>);

    std::shared_ptr<Description> desc;
};


class CanvasInfo : public Canvas
{
public:

    CanvasInfo(Page *);

    virtual void OnEventPaint(wxPaintEvent &) override;

private:

    void DrawTAP(std::shared_ptr<Description> desc);
    void DrawTZX(std::shared_ptr<Description> desc);
    void DrawHEX(std::shared_ptr<Description> desc);

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

    Writer writer;
};


class ControlPanelInfo : public ControlPanel
{
public:
    ControlPanelInfo(wxWindow *);
};
