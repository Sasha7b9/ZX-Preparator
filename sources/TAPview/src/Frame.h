// 2022/04/29 13:56:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Notebook.h"
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#include <wx/dirctrl.h>
#pragma warning(pop)


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);

    static Frame *Self() { return self; }

    void OnPaint(wxPaintEvent &);

    void OnCloseWindow(wxCloseEvent &);

    void OnOpen(wxCommandEvent &);

private:

    static Frame *self;
    wxToolBar *toolBar = nullptr;
    Notebook *notebook = nullptr;                 // There will be pages for working with files
    wxGenericDirCtrl *controlDir = nullptr;         // Directory tree for select files

    void OnViewBrief(wxCommandEvent &);
    void OnViewFull(wxCommandEvent &);

    void OnMeasurePressure(wxCommandEvent &);
    void OnMeasureIllumination(wxCommandEvent &);
    void OnMeasureHumidity(wxCommandEvent &);
    void OnMeasureVelocity(wxCommandEvent &);
    void OnMeasureTemperature(wxCommandEvent &);

    void CreateFrameToolBar();

    void OnEventTreeItemActivated(wxTreeEvent &);
    void OnEventTreeItemKeyDown(wxTreeEvent &);
    void OnEventTreeSelChanging(wxTreeEvent &);

    void OnClose();
};
