// 2022/05/23 11:36:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"


class PageArrayNumber : public Page
{
public:
    PageArrayNumber(wxNotebook *parent, pchar name) : Page(parent, TypePage::ArrayNumber, name) { }
};
