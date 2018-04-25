#include "pnlSettings.h"
#include "portaudio.h"
#include <wx/app.h>
#include <wx/log.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include "settings.h"
//#include "meter.h"
#include "version.h"

//(*InternalHeaders(pnlSettings)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "images/end_hz.xpm"
#include "images/end_hz_press.xpm"
#include "images/home_hz_press.xpm"
#include "images/home_hz.xpm"
#include "images/pagedown.xpm"
#include "images/pagedown_press.xpm"
#include "images/pageup.xpm"
#include "images/pageup_press.xpm"

using namespace std;

//(*IdInit(pnlSettings)
const long pnlSettings::ID_M_PLBL37 = wxNewId();
const long pnlSettings::ID_M_PLBL3 = wxNewId();
const long pnlSettings::ID_M_PLST1 = wxNewId();
const long pnlSettings::ID_M_PLST2 = wxNewId();
const long pnlSettings::ID_M_PBTN1 = wxNewId();
const long pnlSettings::ID_M_PBTN2 = wxNewId();
const long pnlSettings::ID_M_PBTN4 = wxNewId();
const long pnlSettings::ID_M_PBTN5 = wxNewId();
const long pnlSettings::ID_PANEL1 = wxNewId();
const long pnlSettings::ID_M_PLBL4 = wxNewId();
const long pnlSettings::ID_M_PLBL1 = wxNewId();
const long pnlSettings::ID_M_PLST5 = wxNewId();
const long pnlSettings::ID_M_PBTN3 = wxNewId();
const long pnlSettings::ID_M_PLST6 = wxNewId();
const long pnlSettings::ID_M_PLST4 = wxNewId();
const long pnlSettings::ID_M_PLBL5 = wxNewId();
const long pnlSettings::ID_PANEL2 = wxNewId();
const long pnlSettings::ID_PANEL4 = wxNewId();
const long pnlSettings::ID_PANEL5 = wxNewId();
const long pnlSettings::ID_PANEL3 = wxNewId();
const long pnlSettings::ID_M_PLST3 = wxNewId();
const long pnlSettings::ID_PANEL6 = wxNewId();
const long pnlSettings::ID_M_PSWP1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettings,wxPanel)
	//(*EventTable(pnlSettings)
	//*)
END_EVENT_TABLE()


pnlSettings::pnlSettings(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    //(*Initialize(pnlSettings)
    Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
    SetBackgroundColour(wxColour(0,0,0));
    m_plblSettings = new wmLabel(this, ID_M_PLBL37, _("Settings"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
    m_plblSettings->SetBorderState(uiRect::BORDER_NONE);
    m_plblSettings->SetForegroundColour(wxColour(255,255,255));
    m_plblSettings->SetBackgroundColour(wxColour(61,146,146));
    wxFont m_plblSettingsFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_plblSettings->SetFont(m_plblSettingsFont);
    m_pswpSettings = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,40), wxSize(600,440), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
    m_pswpSettings->SetPageNameStyle(0);
    pnlInput = new wxPanel(m_pswpSettings, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    pnlInput->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl3 = new wmLabel(pnlInput, ID_M_PLBL3, _("Audio Input"), wxPoint(0,5), wxSize(600,30), 0, _T("ID_M_PLBL3"));
    m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl3->SetForegroundColour(wxColour(255,255,255));
    m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
    wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl3->SetFont(m_pLbl3Font);
    m_plstDevices = new wmList(pnlInput, ID_M_PLST1, wxPoint(0,70), wxSize(600,300), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
    m_plstDevices->SetBackgroundColour(wxColour(0,0,0));
    m_plstDevices->SetSelectedButtonColour(wxColour(wxT("#008000")));
    m_plstInput = new wmList(pnlInput, ID_M_PLST2, wxPoint(0,36), wxSize(600,34), wmList::STYLE_SELECT, 0, wxSize(100,30), 3, wxSize(-1,-1));
    m_plstInput->SetBackgroundColour(wxColour(0,0,0));
    m_plstInput->SetButtonColour(wxColour(wxT("#400080")));
    m_plstInput->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_pbtnHome = new wmButton(pnlInput, ID_M_PBTN1, wxEmptyString, wxPoint(70,390), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnHome->SetColourDisabled(wxColour(wxT("#808080")));
    m_pbtnPrevious = new wmButton(pnlInput, ID_M_PBTN2, wxEmptyString, wxPoint(190,390), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
    m_pbtnPrevious->SetColourDisabled(wxColour(wxT("#808080")));
    m_pbtnNext = new wmButton(pnlInput, ID_M_PBTN4, wxEmptyString, wxPoint(310,390), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
    m_pbtnNext->SetColourDisabled(wxColour(wxT("#808080")));
    m_pbtnEnd = new wmButton(pnlInput, ID_M_PBTN5, wxEmptyString, wxPoint(430,390), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
    m_pbtnEnd->SetColourDisabled(wxColour(wxT("#808080")));
    pnlOutput = new wxPanel(m_pswpSettings, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    pnlOutput->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl4 = new wmLabel(pnlOutput, ID_M_PLBL4, _("Audio Output"), wxPoint(0,5), wxSize(600,30), 0, _T("ID_M_PLBL4"));
    m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl4->SetForegroundColour(wxColour(255,255,255));
    m_pLbl4->SetBackgroundColour(wxColour(0,64,0));
    wxFont m_pLbl4Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl4->SetFont(m_pLbl4Font);
    m_pLbl1 = new wmLabel(pnlOutput, ID_M_PLBL1, _("Play Buffer"), wxPoint(0,280), wxSize(80,30), 0, _T("ID_M_PLBL1"));
    m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl1->SetForegroundColour(wxColour(255,255,255));
    m_pLbl1->SetBackgroundColour(wxColour(144,144,144));
    m_pLbl1->GetUiRect().SetGradient(0);
    m_plstBuffer = new wmList(pnlOutput, ID_M_PLST5, wxPoint(80,280), wxSize(520,30), wmList::STYLE_SELECT, 0, wxSize(-1,26), 4, wxSize(5,1));
    m_plstBuffer->Disable();
    m_plstBuffer->SetBackgroundColour(wxColour(144,144,144));
    m_plstBuffer->SetButtonColour(wxColour(wxT("#008040")));
    m_plstBuffer->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_pbtnOutput = new wmButton(pnlOutput, ID_M_PBTN3, _("Playback"), wxPoint(0,40), wxSize(200,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN3"));
    m_pbtnOutput->SetBackgroundColour(wxColour(64,0,64));
    m_plstLatency = new wmList(pnlOutput, ID_M_PLST6, wxPoint(80,320), wxSize(520,30), wmList::STYLE_SELECT, 0, wxSize(-1,26), 10, wxSize(5,1));
    m_plstLatency->Disable();
    m_plstLatency->SetBackgroundColour(wxColour(144,144,144));
    m_plstLatency->SetButtonColour(wxColour(wxT("#008040")));
    m_plstLatency->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_plstPlayback = new wmList(pnlOutput, ID_M_PLST4, wxPoint(0,70), wxSize(600,200), wmList::STYLE_SELECT, 0, wxSize(-1,60), 4, wxSize(5,1));
    m_plstPlayback->Disable();
    m_plstPlayback->SetBackgroundColour(wxColour(0,0,0));
    m_plstPlayback->SetSelectedButtonColour(wxColour(wxT("#008000")));
    m_pLbl5 = new wmLabel(pnlOutput, ID_M_PLBL5, _("Latency"), wxPoint(0,320), wxSize(80,30), 0, _T("ID_M_PLBL5"));
    m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl5->SetForegroundColour(wxColour(255,255,255));
    m_pLbl5->SetBackgroundColour(wxColour(144,144,144));
    m_pLbl1->GetUiRect().SetGradient(0);
    pnlSettingsRTP = new pnlRTP(m_pswpSettings, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    pnlSettingsNetwork = new pnlNetworkSetup(m_pswpSettings, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    m_ppnlPlugins = new pnlSettingsPlugins(m_pswpSettings, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    pnlThreads = new wxPanel(m_pswpSettings, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    pnlThreads->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
    m_plstThreads = new wmList(pnlThreads, ID_M_PLST3, wxDefaultPosition, wxSize(600,480), 0, 0, wxSize(-1,-1), 3, wxSize(-1,-1));
    m_plstThreads->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
    m_pswpSettings->AddPage(pnlInput, _("Audio Input"), false);
    m_pswpSettings->AddPage(pnlOutput, _("Audio Output"), false);
    m_pswpSettings->AddPage(pnlSettingsRTP, _("AoIP"), false);
    m_pswpSettings->AddPage(pnlSettingsNetwork, _("Network"), false);
    m_pswpSettings->AddPage(m_ppnlPlugins, _("Plugins"), false);
    m_pswpSettings->AddPage(pnlThreads, _("Threads"), false);

    Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstDevicesSelected);
    Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstInputSelected);
    Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnHomeClick);
    Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnPreviousClick);
    Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnNextClick);
    Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnEndClick);
    Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstBufferSelected);
    Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnOutputClick);
    Connect(ID_M_PLST6,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlblLatencySelected);
    Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstPlaybackSelected);
    //*)

    m_pbtnEnd->SetBitmapLabel(wxBitmap(end_hz_xpm));
    m_pbtnEnd->SetBitmapSelected(wxBitmap(end_hz_press_xpm));
    m_pbtnHome->SetBitmapLabel(wxBitmap(home_hz_xpm));
    m_pbtnHome->SetBitmapSelected(wxBitmap(home_hz_press_xpm));
    m_pbtnPrevious->SetBitmapLabel(wxBitmap(pageup_xpm));
    m_pbtnPrevious->SetBitmapSelected(wxBitmap(pageup_press_xpm));
    m_pbtnNext->SetBitmapLabel(wxBitmap(pagedown_xpm));
    m_pbtnNext->SetBitmapSelected(wxBitmap(pagedown_press_xpm));

    m_pswpSettings->Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, (wxObjectEventFunction)&pnlSettings::OnswpSettingsPageChanged, NULL, this);
    SetSize(size);
    SetPosition(pos);


    for(unsigned int i = 0; i < 10; i++)
    {
        m_plstLatency->AddButton(wxString::Format(wxT("%u ms"), i*20));
    }

    m_plstInput->AddButton(wxT("Soundcard"));
    m_plstInput->AddButton(wxT("RTP"));
    m_plstInput->AddButton(wxT("File"));

    m_pbtnOutput->SetToggleLook(true, wxT("OFF"), wxT("ON"),50.0);

    ShowSoundcardOutputs();

    m_plstPlayback->Disable();



    m_plstInput->SelectButton(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")), true);
    m_pbtnOutput->ToggleSelection(Settings::Get().Read(wxT("Output"), wxT("Enabled"), 1), true);

    ShowRTPDefined();

    m_plstBuffer->AddButton(wxT("1024"));
    m_plstBuffer->AddButton(wxT("2048"));
    m_plstBuffer->AddButton(wxT("4096"));
    m_plstBuffer->AddButton(wxT("8192"));

    m_plstBuffer->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Buffer"), wxT("2048")), false);

    m_plstLatency->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Latency"), 0)/20, false);

    m_plblSettings->SetLabel(wxString::Format(wxT("Settings: %ld.%ld.%ld.%ld"), AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::BUILD, AutoVersion::REVISION));
}

pnlSettings::~pnlSettings()
{
	//(*Destroy(pnlSettings)
	//*)
}


void pnlSettings::OnlstDevicesSelected(wxCommandEvent& event)
{
    wxString sDevice(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")));
    if(sDevice == wxT("Soundcard"))
    {
        Settings::Get().Write(wxT("Input"), wxT("Device"), (int)event.GetClientData());
    }
    else if(sDevice == wxT("RTP"))
    {
        Settings::Get().Write(wxT("Input"), wxT("RTP"), event.GetString());
    }
    else if(sDevice == wxT("File"))
    {
        Settings::Get().Write(wxT("Input"), wxT("File"), event.GetString());
    }
}

void pnlSettings::OnlstMeters_M36Selected(wxCommandEvent& event)
{
//    if(event.GetString() == wxT("M3"))
//    {
//        Settings::Get()->SetMeterMSMode(meter::M3);
//    }
//    else
//    {
//        Settings::Get()->SetMeterMSMode(meter::M6);
//    }
}

void pnlSettings::OnlstMeters_SpeedSelected(wxCommandEvent& event)
{
//    if(event.GetString() == wxT("Slow"))
//    {
//        Settings::Get()->SetMeterSpeed(meter::SLOW);
//    }
//    else if(event.GetString() == wxT("Fast"))
//    {
//        Settings::Get()->SetMeterSpeed(meter::FAST);
//    }
//    else
//    {
//        Settings::Get()->SetMeterSpeed(meter::NORMAL);
//    }
}

void pnlSettings::OnBtnExit(wxCommandEvent& event)
{
    wxTheApp->GetTopWindow()->Close();
}

void pnlSettings::OnbtnInputClick(wxCommandEvent& event)
{


}

void pnlSettings::OnbtnAddSourceClick(wxCommandEvent& event)
{

}

void pnlSettings::OnbtnOutputClick(wxCommandEvent& event)
{
    m_plstPlayback->Enable(event.IsChecked());

    Settings::Get().Write(wxT("Output"), wxT("Enabled"), event.IsChecked());

}

void pnlSettings::OnlstPlaybackSelected(wxCommandEvent& event)
{

    Settings::Get().Write(wxT("Output"), wxT("Device"), (int)event.GetClientData());

}



void pnlSettings::ShowSoundcardInputs()
{
    m_plstDevices->Freeze();
    m_plstDevices->Clear();

    int nDevices =  Pa_GetDeviceCount();
    for(int i = 0; i < nDevices; i++)
    {
        const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(i);
        if(pInfo && pInfo->maxInputChannels > 0)
        {
            m_plstDevices->AddButton(wxString::Format(wxT("%s %d"), wxString::FromAscii(pInfo->name).c_str(), i), wxNullBitmap, (void*)i);
        }
    }

    m_plstDevices->Thaw();

    ShowPagingButtons();

    int nInput = Settings::Get().Read(wxT("Input"), wxT("Device"), 0);
    int nDevice = m_plstDevices->FindButton(reinterpret_cast<void*>(nInput));
    if(nDevice == wmList::NOT_FOUND)
    {
        nDevice = 0;
    }
    m_plstDevices->SelectButton(nDevice);
}


void pnlSettings::ShowSoundcardOutputs()
{

    int nDevices =  Pa_GetDeviceCount();
    for(int i = 0; i < nDevices; i++)
    {
        const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(i);
        if(pInfo && pInfo->maxOutputChannels > 0)
        {
            m_plstPlayback->AddButton(wxString::Format(wxT("%s %d"), wxString::FromAscii(pInfo->name).c_str(), i), wxNullBitmap, (void*)i);
        }
    }

    m_plstPlayback->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Device"), (int)Pa_GetDefaultOutputDevice()));
}

void pnlSettings::ShowRTPDefined()
{
    m_plstDevices->Freeze();
    m_plstDevices->Clear();

    int i = 0;


    map<wxString, wxString>::const_iterator itBegin, itEnd;
    if(Settings::Get().GetSectionDataBegin(wxT("RTP"), itBegin) && Settings::Get().GetSectionDataEnd(wxT("RTP"), itEnd))
    {
        for(map<wxString, wxString>::const_iterator itSource = itBegin; itSource != itEnd; ++itSource)
        {
            m_plstDevices->AddButton(itSource->first, wxNullBitmap, (void*)i);
            ++i;
        }
    }
    m_plstDevices->Thaw();

    ShowPagingButtons();

    m_plstDevices->SelectButton(Settings::Get().Read(wxT("Input"), wxT("RTP"), wxEmptyString));


}

void pnlSettings::ShowFiles()
{
    m_plstDevices->Freeze();
    m_plstDevices->Clear();


    wxArrayString asFiles;
    wxDir::GetAllFiles(Settings::Get().Read(wxT("Input"), wxT("Directory"), wxStandardPaths::Get().GetDocumentsDir()), &asFiles, wxT("*.wav"), wxDIR_FILES);

    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fn(asFiles[i]);
        m_plstDevices->AddButton(fn.GetName(), wxNullBitmap, (void*)i);
    }
    m_plstDevices->Thaw();

    ShowPagingButtons();

    m_plstDevices->SelectButton(Settings::Get().Read(wxT("Input"), wxT("File"), wxEmptyString));

}

void pnlSettings::ShowPagingButtons()
{
    m_pbtnEnd->Show(m_plstDevices->GetPageCount() > 1);
    m_pbtnHome->Show(m_plstDevices->GetPageCount() > 1);
    m_pbtnPrevious->Show(m_plstDevices->GetPageCount() > 1);
    m_pbtnNext->Show(m_plstDevices->GetPageCount() > 1);
}

void pnlSettings::ReloadRTP()
{
    if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")) == wxT("RTP"))
    {
        ShowRTPDefined();
    }
}

void pnlSettings::OnbtnDeleteSourceClick(wxCommandEvent& event)
{
}

void pnlSettings::OnbtnDeleteSourceHeld(wxCommandEvent& event)
{

}

void pnlSettings::OnlstBufferSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Buffer"), event.GetString());
}

void pnlSettings::OnbtnMeterShadingClick(wxCommandEvent& event)
{
    //Settings::Get()->Write(wxT("Meters/shading"), event.IsChecked());
}

void pnlSettings::OnlblLatencySelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Latency"), event.GetInt()*20);
}

void pnlSettings::OnlstInputSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Input"), wxT("Type"),event.GetString());
    RefreshInputs();

}

void pnlSettings::RefreshInputs()
{
    wxString sType = Settings::Get().Read(wxT("Input"), wxT("Type"), wxEmptyString);
    if(sType == wxT("Soundcard"))
    {
        ShowSoundcardInputs();
    }
    else if(sType == wxT("RTP"))
    {
        ShowRTPDefined();
    }
    else if(sType == wxT("File"))
    {
        ShowFiles();
    }
}

void pnlSettings::OnswpSettingsPageChanged(wxNotebookEvent& event)
{
    if(m_pswpSettings->GetPageText(m_pswpSettings->GetSelection()) == wxT("Audio Input"))
    {
       RefreshInputs();
    }
}

void pnlSettings::OnbtnHomeClick(wxCommandEvent& event)
{

}

void pnlSettings::OnbtnPreviousClick(wxCommandEvent& event)
{
    m_plstDevices->ShowPreviousPage(false, false);
}

void pnlSettings::OnbtnNextClick(wxCommandEvent& event)
{
    m_plstDevices->ShowNextPage(false, false);
}

void pnlSettings::OnbtnEndClick(wxCommandEvent& event)
{
}
