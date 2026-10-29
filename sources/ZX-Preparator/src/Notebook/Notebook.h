// 2022/5/22 10:31:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/PageInfo.h"
#include "Parser/ParserTAP.h"
#include "Parser/ParserTZX.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
    #include <wx/wx.h>
    #include <wx/notebook.h>
#pragma warning(pop)


class Notebook : public wxNotebook
{
public:

    Notebook(wxWindow *parent);

    // Применяем/парсим файл
    void AssignFile(const wxString &file_name);

private:

    PageInfo *pageInfo = nullptr;

    std::vector<Page *> pages;

    void Clear();

    void RemoveAllPages();

    void FillTAP(ParserTAP &);

    void FillTZX(ParserTZX &);

    PageInfo *GetPageInfo()
    {
        return pageInfo;
    }

    void AppendPage(Page *);
};
