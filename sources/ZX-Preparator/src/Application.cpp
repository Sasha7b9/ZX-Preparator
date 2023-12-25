// 2022/04/29 13:56:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Frame.h"
#include "Utils/Configurator.h"


wxIMPLEMENT_APP(Application);

wxString Application::file_name_config(wxGetCwd() + "/CPD-213.conf");

enum
{
    TIMER_ID = 111
};


bool Application::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    g_file_config = new wxFileConfig("", "", file_name_config);

    Config::SetFile("");

    // we use a PNG image in our HTML page
    wxImage::AddHandler(new wxPNGHandler);

    // create and show the main application window
    Frame *frame = new Frame("ZX-Preparator");

    frame->Show();

    Init();

    Bind(wxEVT_TIMER, &Application::OnTimer, this, TIMER_ID);

    timer.SetOwner(this, TIMER_ID);

    timer.Start();

    return true;
}


void Application::OnTimer(wxTimerEvent &)
{
    Update();
}


int Application::OnExit()
{
    wxConfigBase::Get(false)->Flush();

    wxConfigBase::Set(nullptr);

    delete g_file_config;

    return wxApp::OnExit();
}
