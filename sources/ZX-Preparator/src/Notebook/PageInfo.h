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

    void DrawTAP(wxPaintDC &, std::shared_ptr<Description> desc, int x, int y);
    void DrawTZX(wxPaintDC &, std::shared_ptr<Description> desc, int x, int y);
    void DrawHEX(wxPaintDC &, std::shared_ptr<Description> desc, int x, int y);
};


class ControlPanelInfo : public ControlPanel
{
public:
    ControlPanelInfo(wxWindow *);
};
