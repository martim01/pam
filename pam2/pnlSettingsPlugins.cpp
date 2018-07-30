#include "pnlSettingsPlugins.h"
#include <wx/app.h>
#include "settings.h"
#include <wx/dir.h>
#include <wx/dynlib.h>
#include "monitorpluginfactory.h"
#include "testpluginfactory.h"
#include <wx/filename.h>
#include <wx/log.h>
#include "wmlogevent.h"
//(*InternalHeaders(pnlSettingsPlugins)
#include <wx/intl.h>
#include <wx/string.h>
//*)

using   namespace std;

//(*IdInit(pnlSettingsPlugins)
const long pnlSettingsPlugins::ID_M_PLST1 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLBL1 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLBL2 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLST2 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN9 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN10 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLBL5 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLBL6 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN1 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN2 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN3 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN4 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN6 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLBL3 = wxNewId();
const long pnlSettingsPlugins::ID_M_PLBL4 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN7 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN8 = wxNewId();
const long pnlSettingsPlugins::ID_PANEL1 = wxNewId();
const long pnlSettingsPlugins::ID_M_PBTN5 = wxNewId();
//*)

DEFINE_EVENT_TYPE(wxEVT_PLUGINS_APPLY)

BEGIN_EVENT_TABLE(pnlSettingsPlugins,wxPanel)
	//(*EventTable(pnlSettingsPlugins)
	//*)
END_EVENT_TABLE()

pnlSettingsPlugins::pnlSettingsPlugins(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
{
	//(*Initialize(pnlSettingsPlugins)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(0,30), wxSize(600,420), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetBackgroundColour(wxColour(0,0,0));
	m_plstPossible = new wmList(Panel1, ID_M_PLST1, wxPoint(10,40), wxSize(200,180), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
	m_plstPossible->SetBackgroundColour(wxColour(0,0,0));
	m_plstPossible->SetButtonColour(wxColour(wxT("#008000")));
	m_plstPossible->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pLbl1 = new wmLabel(Panel1, ID_M_PLBL1, _("Possible Plugins"), wxPoint(10,10), wxSize(200,30), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,160));
	m_pLbl2 = new wmLabel(Panel1, ID_M_PLBL2, _("Current Plugins"), wxPoint(390,10), wxSize(200,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,160));
	m_plstCurrent = new wmList(Panel1, ID_M_PLST2, wxPoint(390,40), wxSize(200,180), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
	m_plstCurrent->SetBackgroundColour(wxColour(0,0,0));
	m_plstCurrent->SetButtonColour(wxColour(wxT("#008000")));
	m_plstCurrent->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pbtnCurrentPrevious = new wmButton(Panel1, ID_M_PBTN9, _("Previous Page"), wxPoint(390,224), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN9"));
	m_pbtnCurrentPrevious->SetColourDisabled(wxColour(wxT("#000000")));
	m_pbtnCurrentPrevious->SetTextColourDisabled(wxColour(wxT("#000000")));
	m_pbtnCurrentNext = new wmButton(Panel1, ID_M_PBTN10, _("Next Page"), wxPoint(500,224), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN10"));
	m_pbtnCurrentNext->SetColourDisabled(wxColour(wxT("#000000")));
	m_pbtnCurrentNext->SetTextColourDisabled(wxColour(wxT("#000000")));
	m_plblCurrentVersion = new wmLabel(Panel1, ID_M_PLBL5, _("Version:"), wxPoint(390,270), wxSize(200,25), 0, _T("ID_M_PLBL5"));
	m_plblCurrentVersion->SetBorderState(uiRect::BORDER_NONE);
	m_plblCurrentVersion->SetForegroundColour(wxColour(0,0,0));
	m_plblCurrentVersion->SetBackgroundColour(wxColour(255,255,255));
	m_plblCurrentDetails = new wmLabel(Panel1, ID_M_PLBL6, wxEmptyString, wxPoint(390,300), wxSize(200,100), 0, _T("ID_M_PLBL6"));
	m_plblCurrentDetails->SetBorderState(uiRect::BORDER_NONE);
	m_plblCurrentDetails->SetForegroundColour(wxColour(0,0,0));
	m_plblCurrentDetails->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnSet = new wmButton(Panel1, ID_M_PBTN1, _("Hold To Add"), wxPoint(250,30), wxSize(100,50), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnSet->Disable();
	m_pbtnSet->SetBackgroundColour(wxColour(0,155,0));
	m_pbtnSet->SetColourDisabled(wxColour(wxT("#A0A0A0")));
	m_pbtnClear = new wmButton(Panel1, ID_M_PBTN2, _("Hold To Remove"), wxPoint(250,100), wxSize(100,50), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnClear->Disable();
	m_pbtnClear->SetBackgroundColour(wxColour(155,0,0));
	m_pbtnClear->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_pbtnUp = new wmButton(Panel1, ID_M_PBTN3, _("Move Up"), wxPoint(250,200), wxSize(100,50), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnUp->Disable();
	m_pbtnUp->SetBackgroundColour(wxColour(0,64,64));
	m_pbtnUp->SetColourDisabled(wxColour(wxT("#A0A0A0")));
	m_pbtnDown = new wmButton(Panel1, ID_M_PBTN4, _("Move Down"), wxPoint(250,255), wxSize(100,50), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnDown->Disable();
	m_pbtnDown->SetBackgroundColour(wxColour(0,64,64));
	m_pbtnDown->SetColourDisabled(wxColour(wxT("#A0A0A0")));
	m_pbtnApply = new wmButton(Panel1, ID_M_PBTN6, _("Hold To Apply\n(Restarts App)"), wxPoint(250,330), wxSize(100,50), wmButton::STYLE_HOLD, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnApply->SetBackgroundColour(wxColour(255,111,40));
	m_pbtnApply->SetColourDisabled(wxColour(wxT("#B0B0B0")));
	m_plblVersion = new wmLabel(Panel1, ID_M_PLBL3, _("Version:"), wxPoint(10,270), wxSize(200,25), 0, _T("ID_M_PLBL3"));
	m_plblVersion->SetBorderState(uiRect::BORDER_NONE);
	m_plblVersion->SetForegroundColour(wxColour(0,0,0));
	m_plblVersion->SetBackgroundColour(wxColour(255,255,255));
	m_plblDetails = new wmLabel(Panel1, ID_M_PLBL4, wxEmptyString, wxPoint(10,300), wxSize(200,100), 0, _T("ID_M_PLBL4"));
	m_plblDetails->SetBorderState(uiRect::BORDER_NONE);
	m_plblDetails->SetForegroundColour(wxColour(0,0,0));
	m_plblDetails->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnPossiblePrevious = new wmButton(Panel1, ID_M_PBTN7, _("Previous Page"), wxPoint(10,224), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnPossiblePrevious->SetColourDisabled(wxColour(wxT("#000000")));
	m_pbtnPossiblePrevious->SetTextColourDisabled(wxColour(wxT("#000000")));
	m_pbtnPossibleNext = new wmButton(Panel1, ID_M_PBTN8, _("Next Page"), wxPoint(120,224), wxSize(90,40), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
	m_pbtnPossibleNext->SetColourDisabled(wxColour(wxT("#000000")));
	m_pbtnPossibleNext->SetTextColourDisabled(wxColour(wxT("#000000")));
	m_pbtnPlugin = new wmButton(this, ID_M_PBTN5, _("Plugins"), wxPoint(175,5), wxSize(250,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnPlugin->SetBackgroundColour(wxColour(84,140,203));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsPlugins::OnlstPossibleSelected);
	Connect(ID_M_PLST1,wxEVT_LIST_PAGED,(wxObjectEventFunction)&pnlSettingsPlugins::OnlstPossiblePaged);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsPlugins::OnlstCurrentSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_PAGED,(wxObjectEventFunction)&pnlSettingsPlugins::OnlstCurrentPaged);
	Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnCurrentPreviousClick);
	Connect(ID_M_PBTN10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnCurrentNextClick);
	Connect(ID_M_PBTN1,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsPlugins::OnpbtnSetHeld);
	Connect(ID_M_PBTN2,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnClearHeld);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnUpClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnDownClick);
	Connect(ID_M_PBTN6,wxEVT_BUTTON_HELD,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnApplyHeld);
	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnPossiblePreviousClick);
	Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnPossibleNextClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsPlugins::OnbtnPluginClick);
	//*)
    SetBackgroundColour(*wxBLACK);

    m_plblDetails->SetTextAlign(wxALIGN_LEFT | wxALIGN_TOP);
    m_plblCurrentDetails->SetTextAlign(wxALIGN_LEFT | wxALIGN_TOP);
    m_plblVersion->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    m_plblCurrentVersion->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    m_pbtnPlugin->SetToggleLook(true, wxT("Monitors"), wxT("Tests"), 33.0);
    m_sSection = wxT("Monitor Plugins");
    ShowMonitorPlugins();
}

pnlSettingsPlugins::~pnlSettingsPlugins()
{
	//(*Destroy(pnlSettingsPlugins)
	//*)
}


void pnlSettingsPlugins::OnlstPossibleSelected(wxCommandEvent& event)
{
    m_itPossible = m_mPossible.find(event.GetString());
    m_nPossible = event.GetInt();
    m_pbtnSet->Enable(m_plstPossible->GetSelectionCount() == 1);

    m_plblVersion->SetLabel(wxString::Format(wxT("Version: %s"),m_itPossible->second.sVersion.c_str()));
    m_plblDetails->SetLabel(m_itPossible->second.sDetails);

}

void pnlSettingsPlugins::OnlstCurrentSelected(wxCommandEvent& event)
{
    m_nSelected = event.GetInt();
    m_pbtnClear->Enable((m_plstCurrent->GetSelectionCount() == 1));
    m_pbtnUp->Enable((m_plstCurrent->GetSelectionCount() == 1));
    m_pbtnDown->Enable((m_plstCurrent->GetSelectionCount() == 1));

    map<wxString, plugin>::iterator itCurrent = m_mPossible.find(event.GetString());
    if(itCurrent != m_mPossible.end())
    {
        m_plblCurrentVersion->SetLabel(wxString::Format(wxT("Version: %s"), itCurrent->second.sVersion.c_str()));
        m_plblCurrentDetails->SetLabel(itCurrent->second.sDetails);
    }
    else
    {
        m_plblCurrentDetails->SetLabel(wxT("?"));
        m_plblCurrentVersion->SetLabel(wxT("Version: ?"));
    }

}

void pnlSettingsPlugins::OnpbtnSetHeld(wxCommandEvent& event)
{
    if(m_itPossible != m_mPossible.end())
    {
        Settings::Get().Write(m_sSection, wxString::Format(wxT("%04d"), m_plstCurrent->GetItemCount()+1), m_itPossible->second.sLibrary);
        m_plstCurrent->Freeze();
        int nButton = m_plstCurrent->AddButton(m_itPossible->first);
        m_plstCurrent->SetButtonColour(nButton, wxColour(0,128,0));
        m_plstCurrent->Thaw();
        m_plstCurrent->SelectButton(nButton, true);

        m_plstPossible->DeleteButton(m_nPossible);
        m_pbtnSet->Enable(false);
        // @todo load plugin

        m_plblDetails->SetLabel(wxEmptyString);
        m_plblVersion->SetLabel(wxT("Version: "));

        EnablePageButtons();
    }
    else
    {
        wxLogMessage(wxT("Plugin not found"));
    }
}

void pnlSettingsPlugins::OnbtnClearHeld(wxCommandEvent& event)
{
    m_plstPossible->Freeze();
    m_plstPossible->AddButton(m_plstCurrent->GetButtonText(m_nSelected));
    m_plstPossible->Thaw();
    m_plstPossible->Refresh();


    Settings::Get().RemoveKey(m_sSection, wxString::Format(wxT("%04d"), m_nSelected+1));
    m_plstCurrent->DeleteButton(m_nSelected);
    m_nSelected = 0xFFFFFFF;

    m_pbtnClear->Enable(false);
    m_pbtnUp->Enable(false);
    m_pbtnDown->Enable(false);

    m_plblCurrentDetails->SetLabel(wxEmptyString);
    m_plblCurrentVersion->SetLabel(wxT("Version: "));

    // @todo remove plugin
    EnablePageButtons();
}

void pnlSettingsPlugins::OnbtnUpClick(wxCommandEvent& event)
{
    if(m_nSelected != 0)    //first button
    {
        wxString sLib = Settings::Get().Read(m_sSection,wxString::Format(wxT("%04d"), m_nSelected+1), wxEmptyString);
        wxString sText = m_plstCurrent->GetButtonText(m_nSelected);

        wxString sBeforeLib = Settings::Get().Read(m_sSection,wxString::Format(wxT("%04d"), m_nSelected), wxEmptyString);
        wxString sBeforeText = m_plstCurrent->GetButtonText(m_nSelected-1);

        Settings::Get().Write(m_sSection, wxString::Format(wxT("%04d"), m_nSelected), sLib);
        m_plstCurrent->SetButtonText(m_nSelected-1, sText);

        Settings::Get().Write(m_sSection, wxString::Format(wxT("%04d"), m_nSelected+1), sBeforeLib);
        m_plstCurrent->SetButtonText(m_nSelected, sBeforeText);

        m_plstCurrent->SelectButton(m_nSelected-1, true);
    }
}

void pnlSettingsPlugins::OnbtnDownClick(wxCommandEvent& event)
{
    if(m_nSelected+1 != m_plstCurrent->GetItemCount())    //first button
    {
        wxString sLib = Settings::Get().Read(m_sSection,wxString::Format(wxT("%04d"), m_nSelected+1), wxEmptyString);
        wxString sText = m_plstCurrent->GetButtonText(m_nSelected);

        wxString sAfterLib = Settings::Get().Read(m_sSection,wxString::Format(wxT("%04d"), m_nSelected+2), wxEmptyString);
        wxString sAfterText = m_plstCurrent->GetButtonText(m_nSelected+1);

        Settings::Get().Write(m_sSection, wxString::Format(wxT("%04d"), m_nSelected+2), sLib);
        m_plstCurrent->SetButtonText(m_nSelected+1, sText);

        Settings::Get().Write(m_sSection, wxString::Format(wxT("%04d"), m_nSelected+1), sAfterLib);
        m_plstCurrent->SetButtonText(m_nSelected, sAfterText);

        m_plstCurrent->SelectButton(m_nSelected+1, true);
    }
}

void pnlSettingsPlugins::OnbtnPluginClick(wxCommandEvent& event)
{
    m_bTests = event.IsChecked();

    if(m_bTests)
    {
        m_sSection = wxT("Test Plugins");
        ShowTestPlugins();
    }
    else
    {
        m_sSection = wxT("Monitor Plugins");
        ShowMonitorPlugins();
    }
}

void pnlSettingsPlugins::ClearLists()
{
    m_mPossible.clear();
    m_plstCurrent->Clear();
    m_plstPossible->Clear();
    m_pbtnClear->Enable(false);
    m_pbtnDown->Enable(false);
    m_pbtnSet->Enable(false);
    m_pbtnUp->Enable(false);

    m_plblDetails->SetLabel(wxEmptyString);
    m_plblVersion->SetLabel(wxT("Version: "));
    m_plblCurrentDetails->SetLabel(wxEmptyString);
    m_plblCurrentVersion->SetLabel(wxT("Version: "));
}

void pnlSettingsPlugins::ShowMonitorPlugins()
{

    ClearLists();

    wxString sLibDir = MonitorPluginFactory::Get()->GetPluginDirectory();

    size_t nButton = 0;
    map<wxString, wxString>::const_iterator itBegin, itEnd;
    if(Settings::Get().GetSectionDataBegin(m_sSection, itBegin) && Settings::Get().GetSectionDataEnd(m_sSection, itEnd))
    {
        for(; itBegin != itEnd; ++itBegin)
        {
            plugin aPlugin = MonitorPluginFactory::Get()->GetPluginDetails(sLibDir, itBegin->second);
            if(aPlugin.sName != wxEmptyString)
            {
                #ifdef __WXGNU__
                    aPlugin.sLibrary = aPlugin.sLibrary.Mid(3);
                #endif // __WXGNU__
                m_mPossible.insert(make_pair(aPlugin.sName, aPlugin));

                m_plstCurrent->AddButton(aPlugin.sName);
            }
        }
    }

    wxFileName fnDir(sLibDir);
    fnDir.MakeAbsolute();
    wmLog::Get()->Log(wxString::Format(wxT("Lib Directory: %s"), fnDir.GetFullPath().c_str()));
    wxArrayString asLibs;
    #ifdef __WXMSW__
        wxDir::GetAllFiles(fnDir.GetFullPath(), &asLibs, wxT("*.dll"));
    #else
        wxDir::GetAllFiles(fnDir.GetFullPath(), &asLibs, wxT("*.so"));
    #endif

    for(size_t i = 0; i < asLibs.GetCount(); i++)
    {
        wxFileName fnLib(asLibs[i]);
        wxString sLibToLoad(fnLib.GetName());
        #ifdef __WXGNU__
            sLibToLoad = sLibToLoad.Mid(3);
        #endif // __WXGNU__
        wmLog::Get()->Log(wxString::Format(wxT("Lib : %s"), sLibToLoad.c_str()));
        plugin aPlugin = MonitorPluginFactory::Get()->GetPluginDetails(fnDir.GetFullPath(), sLibToLoad);
        if(aPlugin.sName != wxEmptyString)
        {
            if(m_mPossible.insert(make_pair(aPlugin.sName, aPlugin)).second)
            {
                m_plstPossible->AddButton(aPlugin.sName);
            }
        }
    }
    EnablePageButtons();
}

void pnlSettingsPlugins::ShowTestPlugins()
{
    ClearLists();

    wxString sLibDir = TestPluginFactory::Get()->GetPluginDirectory();


    size_t nButton = 0;
    map<wxString, wxString>::const_iterator itBegin, itEnd;
    if(Settings::Get().GetSectionDataBegin(m_sSection, itBegin) && Settings::Get().GetSectionDataEnd(m_sSection, itEnd))
    {
        for(; itBegin != itEnd; ++itBegin)
        {
            plugin aPlugin = TestPluginFactory::Get()->GetPluginDetails(sLibDir, itBegin->second);
            if(aPlugin.sName != wxEmptyString)
            {
                m_mPossible.insert(make_pair(aPlugin.sName, aPlugin));
                m_plstCurrent->AddButton(aPlugin.sName);
            }
        }
    }


    wxArrayString asLibs;
    #ifdef __WXMSW__
        wxDir::GetAllFiles(sLibDir, &asLibs, wxT("*.dll"));
    #else
        wxDir::GetAllFiles(sLibDir, &asLibs, wxT("*.so"));
    #endif

    for(size_t i = 0; i < asLibs.GetCount(); i++)
    {
        wxFileName fnLib(asLibs[i]);
        wxString sLibToLoad(fnLib.GetName());
        #ifdef __WXGNU__
            sLibToLoad = sLibToLoad.Mid(3);
        #endif // __WXGNU__
        plugin aPlugin = TestPluginFactory::Get()->GetPluginDetails(sLibDir, sLibToLoad);
        if(aPlugin.sName != wxEmptyString)
        {
            if(m_mPossible.insert(make_pair(aPlugin.sName, aPlugin)).second)
            {
                m_plstPossible->AddButton(aPlugin.sName);
            }
        }
    }
    EnablePageButtons();
}

void pnlSettingsPlugins::OnbtnApplyHeld(wxCommandEvent& event)
{
    //tell the main window to reload all plugins
    wxCommandEvent eventApply(wxEVT_PLUGINS_APPLY);
    wxPostEvent(wxTheApp->GetTopWindow(), eventApply);
}

void pnlSettingsPlugins::OnlstPossiblePaged(wxCommandEvent& event)
{
    m_pbtnPossiblePrevious->Enable(m_plstPossible->GetCurrentPageNumber() != 1);
    m_pbtnPossibleNext->Enable(m_plstPossible->GetCurrentPageNumber() != m_plstPossible->GetPageCount());
}

void pnlSettingsPlugins::OnlstCurrentPaged(wxCommandEvent& event)
{

    m_pbtnCurrentPrevious->Enable(m_plstCurrent->GetCurrentPageNumber() != 1);
    m_pbtnCurrentNext->Enable(m_plstCurrent->GetCurrentPageNumber() != m_plstCurrent->GetPageCount());
}

void pnlSettingsPlugins::OnbtnPossiblePreviousClick(wxCommandEvent& event)
{
    m_plstPossible->ShowPreviousPage(false,true);
}

void pnlSettingsPlugins::OnbtnPossibleNextClick(wxCommandEvent& event)
{
    m_plstPossible->ShowNextPage(false,true);
}

void pnlSettingsPlugins::OnbtnCurrentPreviousClick(wxCommandEvent& event)
{
    m_plstCurrent->ShowPreviousPage(false, true);
}

void pnlSettingsPlugins::OnbtnCurrentNextClick(wxCommandEvent& event)
{
    m_plstCurrent->ShowNextPage(false, true);
}

void pnlSettingsPlugins::EnablePageButtons()
{
    if(m_plstPossible->GetPageCount()  > 1)
    {
        m_pbtnPossiblePrevious->Enable(m_plstPossible->GetCurrentPageNumber() != 1);
        m_pbtnPossibleNext->Enable(m_plstPossible->GetCurrentPageNumber() != m_plstPossible->GetPageCount());
    }
    else
    {
        m_pbtnPossibleNext->Disable();
        m_pbtnPossiblePrevious->Disable();
    }
    if(m_plstCurrent->GetPageCount() > 1)
    {
        m_pbtnCurrentPrevious->Enable(m_plstCurrent->GetCurrentPageNumber() != 1);
        m_pbtnCurrentNext->Enable(m_plstCurrent->GetCurrentPageNumber() != m_plstCurrent->GetPageCount());
    }
    else
    {
        m_pbtnCurrentNext->Disable();
        m_pbtnCurrentPrevious->Disable();

    }
}
