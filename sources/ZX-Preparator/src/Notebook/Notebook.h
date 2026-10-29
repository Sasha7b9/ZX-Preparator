// 2022/5/22 10:31:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/PageInfo.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
    #include <wx/wx.h>
    #include <wx/notebook.h>
#pragma warning(pop)


class Notebook : public wxNotebook
{
public:

    Notebook(wxWindow *parent);

    void AssignFIle(const wxString &file_name);

    PageInfo *GetPageInfo()
    {
        return pageInfo;
    }

    void AppendPage(Page *);

private:

    PageInfo *pageInfo = nullptr;

    std::vector<Page *> pages;

    void RemoveAllPages();
};
