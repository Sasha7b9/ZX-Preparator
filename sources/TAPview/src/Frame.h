// 2022/04/29 13:56:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#define WIN32_LEAN_AND_MEAN
#include <wx/wx.h>
#include <wx/notebook.h>
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
    wxNotebook *notebook = nullptr;

    void OnViewBrief(wxCommandEvent &);
    void OnViewFull(wxCommandEvent &);

    void OnMeasurePressure(wxCommandEvent &);
    void OnMeasureIllumination(wxCommandEvent &);
    void OnMeasureHumidity(wxCommandEvent &);
    void OnMeasureVelocity(wxCommandEvent &);
    void OnMeasureTemperature(wxCommandEvent &);

    void CreateFrameToolBar();

    void OnClose();
};
