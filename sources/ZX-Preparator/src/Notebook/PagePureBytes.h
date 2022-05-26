// 2022/05/23 14:36:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"


class PagePureBytes : public Page
{
public:
    PagePureBytes(wxNotebook *parent, pchar name) : Page(parent, TypePage::PureBytes, name) { }
};
