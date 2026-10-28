// 2022/04/29 13:56:43 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#define WIN32_LEAN_AND_MEAN
#pragma warning (push, 0)
#include <wx/wx.h>
#pragma warning (pop)


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;

    static wxString file_name_config;       // Имя файла конфигурации

private:

    virtual int OnExit() wxOVERRIDE;
};
