// 2022/05/26 11:37:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/ControlPanel.h"
#include "Notebook/Pages.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageHEX.h"
#include "Notebook/PageASM.h"
#include "Notebook/PageBASIC.h"
#pragma warning(push, 0)
#include <wx/button.h>
#pragma warning(pop)


ControlPanel::ControlPanel(wxWindow *parent) : wxPanel(parent)
{
}


ControlPanel *ControlPanel::Create(Page *page)
{
    switch (page->GetType())
    {
        case TypePage::Info:            return new ControlPanelInfo(page);  break;
        case TypePage::HEX:             return new ControlPanelHEX(page);   break;
        case TypePage::BASIC:           return new ControlPanelBASIC(page); break;
        case TypePage::ArrayNumber:     break;
        case TypePage::ArrayCharacter:  break;
        case TypePage::ASM:             return new ControlPanelASM(page);   break;
        case TypePage::PureBytes:       break;
        case TypePage::Count:           break;
    }

    return new ControlPanel(page);
}
