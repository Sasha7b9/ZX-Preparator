// 2022/5/22 10:31:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook/PageInfo.h"
#include "Parser/ParserTAP.h"
#include "Parser/ParserTZX.h"
#include "Parser/ParserHEX.h"
#include "Parser/ParserZ80.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
    #include <wx/wx.h>
    #include <wx/notebook.h>
#pragma warning(pop)
#include <map>


class Notebook;


// Здесь будем хранить историю, чтобы при переключениями между файлами восстанавливать состояние вкладок
struct NotebookHistory
{
    NotebookHistory(Notebook *_notebook) : notebook(_notebook) { }

    // Теперь поддерживаем историю данного файла
    void SetCurrentFile(const wxString &file_name);

    // Сохранить состояние для файла, установленного функцией SetCurrentFile()
    void SaveState();

    // Восстановить состояние для файла, установленного функциенй SetCurrentFile();
    void RestoreState();

private:

    struct HistoryStruct
    {
        HistoryStruct(const wxString &file = "") : name_file(file) {}
        wxString name_file;     // Файл, для которого сохранены настройки
        wxString name_page;     // Открытая страница для данного файла
    };

    std::map <wxString, HistoryStruct> files;

    wxString current_file;

    Notebook *notebook;
};


class Notebook : public wxNotebook
{
public:

    Notebook(wxWindow *parent);

    // Применяем/парсим файл
    void AssignFile(const wxString &file_name);

private:

    PageInfo *pageInfo = nullptr;

    std::vector<Page *> pages;

    NotebookHistory history;

    void Clear();

    void RemoveAllPages();

    void FillTAP(ParserTAP &);

    void FillTZX(ParserTZX &);

    void FillZ80(ParserZ80 &);

    void FillHEX(ParserHEX &);

    PageInfo *GetPageInfo()
    {
        return pageInfo;
    }

    void AppendPage(Page *);

    void OnEventNotebookPageChanged(wxBookCtrlEvent &);
};
