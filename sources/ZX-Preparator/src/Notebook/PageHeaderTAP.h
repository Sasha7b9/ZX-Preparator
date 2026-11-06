// 2026/11/06 09:11:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Notebook/Pages.h"


class PageHeaderTAP : public Page
{
public:

    PageHeaderTAP(wxNotebook *parent, pchar name) : Page(parent, TypePage::HeaderBASIC, name) { }
};


class CanvasHeaderTAP : public Canvas
{
public:

    CanvasHeaderTAP(Page *);
};


class ControlPanelHeaderTAP : public ControlPanel
{
public:
    ControlPanelHeaderTAP(wxWindow *);
};
