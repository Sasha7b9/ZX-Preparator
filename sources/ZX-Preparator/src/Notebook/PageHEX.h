// 2022/05/23 09:52:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/Pages.h"


class PageHEX : public Page
{
public:
    PageHEX(wxNotebook* parent) : Page(parent, TypePage::HEX, "Binary") { }
};
