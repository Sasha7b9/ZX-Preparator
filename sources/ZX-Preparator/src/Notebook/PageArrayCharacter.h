// 2022/05/23 11:36:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"


class PageArrayCharacter : public Page
{
public:
    PageArrayCharacter(wxNotebook *parent, pchar name) : Page(parent, TypePage::ArrayCharacter, name) { }
};

