// 2026/11/06 15:15:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Notebook/PageHeaderTAP.h"


CanvasHeaderTAP::CanvasHeaderTAP(Page *parent) : Canvas(parent)
{
    Bind(wxEVT_PAINT, &CanvasHeaderTAP::OnEventPaint, this);
}




ControlPanelHeaderTAP::ControlPanelHeaderTAP(wxWindow *parent) :
    ControlPanel(parent)
{

}
