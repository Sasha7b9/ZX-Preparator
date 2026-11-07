// 2022/05/23 09:52:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"
#include "Parser/StructuresZX.h"
#include "Controls/Controls.h"
#include <span>


class PageHEX : public Page
{
public:
    PageHEX(wxNotebook* parent) : Page(parent, TypePage::HEX, "HEX") { }

    void SetDump(DumpHEX &);

    DumpHEX dump;
};


class CanvasHEX : public Canvas
{
public:

    CanvasHEX(Page *);

    virtual void OnEventPaint(wxPaintEvent &) override;

private:

    void WriteBytes(PaintDC &, uint16 *address, std::span<const uint8> &, int x, int y);
};


class ControlPanelHEX : public ControlPanel
{
public:
    ControlPanelHEX(wxWindow *);
};
