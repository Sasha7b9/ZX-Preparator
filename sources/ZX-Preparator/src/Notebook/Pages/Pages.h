// 2022/05/23 10:00:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Canvas.h"
#include "Notebook/ControlPanel.h"
#pragma warning(push, 0)
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#include <wx/dc.h>
#include <wx/splitter.h>
#include <wx/sizer.h>
#pragma warning(pop)


struct TypePage
{
    enum E
    {
        Info,
        HEX,
        BASIC,
        ArrayNumber,
        ArrayCharacter,
        ASM,
        PureBytes,
        Count
    };
};


class Page : public wxWindow
{
public:

    Page(wxNotebook* parent, TypePage::E, pchar name);

    virtual void OnPaint(wxPaintEvent &) {};
    void OnSizeEvent(wxSizeEvent&);

protected:

    TypePage::E type = TypePage::Count;
    Canvas *canvas = nullptr;               // For drawing info
    ControlPanel *control_panel = nullptr;  // For settings

    int dY = 0;
    int sbPPU = 0;                    // Scrollbar pixels for unit
};
