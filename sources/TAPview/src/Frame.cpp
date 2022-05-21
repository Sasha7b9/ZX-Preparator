// 2022/04/29 13:56:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Frame.h"
#include "ParserTAP.h"
#include "Page.h"
#pragma warning (push, 0)
#include <wx/statline.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/textfile.h>
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

    Bind(wxEVT_MENU, &Frame::OnViewBrief, this, TOOL_VIEW_BRIEF);
    Bind(wxEVT_MENU, &Frame::OnViewFull, this, TOOL_VIEW_FULL);

    Bind(wxEVT_PAINT, &Frame::OnPaint, this);

    CreateFrameToolBar();

    notebook = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBK_TOP);

    for (int i = 0; i < 4; i++)
    {
        static const char *names[4] =
        {
            "Info",
            "Bin",
            "Parsed",
            "Edit"
        };

        notebook->AddPage(new Page(notebook, i), names[i]);
    }

    controlDir = new wxGenericDirCtrl(this);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(controlDir, 1, wxEXPAND);
    sizer->Add(notebook, 1, wxEXPAND);

    SetSizerAndFit(sizer);

    wxFileInputStream file_input("C:/Temp/WTHAS.TAP");
    wxBufferedInputStream buf_input(file_input);
    ParserTAP parser;
    parser.Run(buf_input);
    std::vector<Line> lines;
    parser.blocks[0].Parse(lines);
    Page *page = (Page *)notebook->GetPage(0);
    page->SetLines(lines);

    SetSize({ 1024, 768 });
}


void Frame::CreateFrameToolBar()
{
    toolBar = CreateToolBar();

    toolBar->AddSeparator();

    Bind(wxEVT_MENU, &Frame::OnMeasurePressure, this, MEAS_PRESSURE);
    Bind(wxEVT_MENU, &Frame::OnMeasureIllumination, this, MEAS_ILLUMINATION);
    Bind(wxEVT_MENU, &Frame::OnMeasureHumidity, this, MEAS_HUMIDITY);
    Bind(wxEVT_MENU, &Frame::OnMeasureVelocity, this, MEAS_VELOCITY);
    Bind(wxEVT_MENU, &Frame::OnMeasureTemperature, this, MEAS_TEMPERATURE);

    toolBar->Realize();
}


void Frame::OnOpen(wxCommandEvent &)
{
    wxFileDialog dialog(this, "Choice tap file", wxEmptyString, wxEmptyString, "*.tap", wxFD_OPEN);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxFileInputStream file_input(dialog.GetPath());

        wxBufferedInputStream buf_input(file_input);

        ParserTAP parser;

        parser.Run(buf_input);

        std::vector<Line> lines;

        parser.blocks[0].Parse(lines);

        Page *page = (Page *)notebook->GetPage(0);

        page->SetLines(lines);

//        wxTextFile text_file;
//
//        text_file.Create(wxString("D:\\out.txt"));
//
//        for (uint i = 0; i < lines.size(); i++)
//        {
//            text_file.AddLine(lines[i]);
//        }
//
//        text_file.Write();
    }
}


void Frame::OnViewBrief(wxCommandEvent &)
{

}


void Frame::OnViewFull(wxCommandEvent &)
{

}


void Frame::OnMeasurePressure(wxCommandEvent &)
{

}


void Frame::OnMeasureIllumination(wxCommandEvent &)
{

}


void Frame::OnMeasureHumidity(wxCommandEvent &)
{

}


void Frame::OnMeasureVelocity(wxCommandEvent &)
{

}


void Frame::OnMeasureTemperature(wxCommandEvent &)
{

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
