// 2022/05/23 10:03:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"


class PageASM : public Page
{
public:
    PageASM(wxNotebook *parent, pchar name) : Page(parent, TypePage::ASM, name) { }
};
