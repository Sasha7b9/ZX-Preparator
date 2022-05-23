// 2022/5/22 9:25:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Notebook/Notebook.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageHEX.h"
#include "Notebook/PageBASIC.h"


Notebook::Notebook(wxWindow *parent) : wxNotebook(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP)
{
    pageInfo = new PageInfo(this);
    pageHEX = new PageHEX(this);
    pageBASIC = new PageBASIC(this);
    pageASM = new PageASM(this);

    AddPage(pageInfo, pageInfo->GetName());
    AddPage(pageHEX, pageHEX->GetName());
    AddPage(pageBASIC, pageBASIC->GetName());
    AddPage(pageASM, pageASM->GetName());
}
