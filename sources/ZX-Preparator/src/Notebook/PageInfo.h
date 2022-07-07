// 2022/05/23 09:52:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"
#include "Parser/ParserStructures.h"


class PageInfo : public Page
{
public:
    PageInfo(wxNotebook* parent) : Page(parent, TypePage::Info, "Info") { }

    void SetDescriptionTAP(DescriptionTAP &);

    DescriptionTAP descTAP;
};


class CanvasInfo : public Canvas
{
public:
    CanvasInfo(Page *);

    virtual void OnEventPaint(wxPaintEvent &) override;
};


class ControlPanelInfo : public ControlPanel
{
public:
    ControlPanelInfo(wxWindow *);
};
