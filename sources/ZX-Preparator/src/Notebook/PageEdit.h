// 2022/05/23 10:03:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"


class PageEdit : public Page
{
public:
    PageEdit(wxNotebook* parent) : Page(parent, TypePage::Edit, "Edit") { }
};
