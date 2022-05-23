// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "Parser/ParserTAP.h"
#include "Notebook/Notebook.h"
#include "Notebook/PageInfo.h"
#include "Notebook/PageBASIC.h"
#pragma warning (push, 0)
#include <wx/statline.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/textfile.h>
#include <wx/splitter.h>
#pragma warning (pop)


Frame *Frame::self = nullptr;


enum
{
    TOOL_OPEN,
    TOOL_SAVE,
    TOOL_NEW,

    TOOL_UNDO,
    TOOL_REDO,

    TOOL_VIEW_BRIEF,        // Сокращённый вид отображения
    TOOL_VIEW_FULL,         // Полный вид отображения

    MEAS_PRESSURE,          // Давление
    MEAS_ILLUMINATION,      // Освещённость
    MEAS_HUMIDITY,          // Влажность
    MEAS_VELOCITY,          // Скорость
    MEAS_TEMPERATURE        // Температура
};


Frame::Frame(const wxString &title)
    : wxFrame((wxFrame *)NULL, wxID_ANY, title)
{
    self = this;

    SetIcon(wxICON(MAIN_ICON));

    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_OPEN);
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    menuBar->Append(menuFile, _("Файл"));

    wxMenu *menuSettings = new wxMenu;
    menuBar->Append(menuSettings, _("Настройки"));

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    menuBar->Append(menuHelp, _("Помощь"));

    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &Frame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Frame::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &Frame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_CLOSE_WINDOW, &Frame::OnCloseWindow, this);

    Bind(wxEVT_PAINT, &Frame::OnPaint, this);

    CreateFrameToolBar();

//    wxSplitterWindow *splitter = new wxSplitterWindow(this);

    notebook = new Notebook(this);

    controlDir = new wxGenericDirCtrl(this);
    controlDir->Bind(wxEVT_TREE_ITEM_ACTIVATED, &Frame::OnEventTreeItemActivated, this);
    controlDir->Bind(wxEVT_TREE_KEY_DOWN, &Frame::OnEventTreeItemKeyDown, this);
    controlDir->Bind(wxEVT_TREE_SEL_CHANGED, &Frame::OnEventTreeSelChanged, this);
    controlDir->Bind(wxEVT_TREE_SEL_CHANGING, &Frame::OnEventTreeSelChanging, this);
    controlDir->SetMaxSize({ 200, 2000 });

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(controlDir, 1, wxEXPAND);
    sizer->Add(notebook, 1, wxEXPAND);

    SetSizerAndFit(sizer);

//    ParserTAP parser;
//
//    if (parser.Execute("C:/Temp/WTHAS.TAP"))
//    {
//        ProgramBASIC program;
//        parser.blocks[0].Decode(program);
//        notebook->GetPageBASIC()->SetProgram(program);
//    }

    SetSize({ 1024, 768 });
}


void Frame::CreateFrameToolBar()
{
    toolBar = CreateToolBar();

    toolBar->AddSeparator();

    toolBar->Realize();
}


void Frame::OnEventTreeItemActivated(wxTreeEvent &event)
{
    event.Skip();
}


void Frame::OnEventTreeItemKeyDown(wxTreeEvent &event)
{
    event.Skip();
}


void Frame::OnEventTreeSelChanged(wxTreeEvent& event)
{
    wxString file = controlDir->GetPath(event.GetItem());

    if (wxFile::Exists(file))               // Проверка на то, что это файл, а не каталог
    {
        notebook->AssignFIle(file);
    }

    event.Skip();
}


void Frame::OnEventTreeSelChanging(wxTreeEvent &event)
{
    event.Skip();
}


void Frame::OnOpen(wxCommandEvent &)
{
    wxFileDialog dialog(this, "Choice tap file", wxEmptyString, wxEmptyString, "*.tap", wxFD_OPEN);

    if (dialog.ShowModal() == wxID_OK)
    {
        ParserTAP parser;

        parser.Execute(dialog.GetPath());

        ProgramBASIC program;

        parser.file.GetBlock(0).ParseBASIC(program);
    }
}


void Frame::OnQuit(wxCommandEvent &WXUNUSED(event))
{
    self = nullptr;

    Close(true);

    OnClose();
}


void Frame::OnCloseWindow(wxCloseEvent &event)
{
    self = nullptr;

    event.Skip();

    OnClose();
}


void Frame::OnAbout(wxCommandEvent &WXUNUSED(event))
{
    wxBoxSizer *topsizer;
    wxDialog dlg(this, wxID_ANY, wxString(_("About")));

    topsizer = new wxBoxSizer(wxVERTICAL);

#if wxUSE_STATLINE
    topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
#endif // wxUSE_STATLINE

    wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
    bu1->SetDefault();

    topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

    dlg.SetSizer(topsizer);
    topsizer->Fit(&dlg);

    dlg.ShowModal();
}


void Frame::OnPaint(wxPaintEvent &)
{

}
