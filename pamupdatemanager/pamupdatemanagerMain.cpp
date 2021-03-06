#include "pamupdatemanagerMain.h"
#include "version.h"
#include "monitorpluginfactory.h"
#include "testpluginfactory.h"
#include "settings.h"
#include "updatemanager.h"
#include <wx/log.h>


//(*InternalHeaders(pamupdatemanagerDialog)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;

//(*IdInit(pamupdatemanagerDialog)
const long pamupdatemanagerDialog::ID_M_PLBL37 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN6 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN5 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN4 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN2 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN3 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL4 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL2 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL3 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLST1 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL1 = wxNewId();
const long pamupdatemanagerDialog::ID_HTMLWINDOW1 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL2 = wxNewId();
const long pamupdatemanagerDialog::ID_HTMLWINDOW2 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL3 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL5 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLST2 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL4 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PSWP2 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN7 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLST3 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL5 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL6 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL7 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL6 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PSWP1 = wxNewId();
const long pamupdatemanagerDialog::ID_TIMER1 = wxNewId();
const long pamupdatemanagerDialog::ID_TIMER2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pamupdatemanagerDialog,wxDialog)
	//(*EventTable(pamupdatemanagerDialog)
	//*)
END_EVENT_TABLE()

pamupdatemanagerDialog::pamupdatemanagerDialog(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pamupdatemanagerDialog)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(800,480));
	Move(wxDefaultPosition);
	SetBackgroundColour(wxColour(0,0,0));
	wxFont thisFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pswpSplash = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,0), wxSize(800,480), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpSplash->SetPageNameStyle(0);
	pnlMain = new wxPanel(m_pswpSplash, ID_PANEL5, wxPoint(0,0), wxSize(800,480), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	pnlMain->SetBackgroundColour(wxColour(0,0,0));
	m_plblSettings = new wmLabel(pnlMain, ID_M_PLBL37, _("Available Updates"), wxPoint(0,0), wxSize(800,40), 0, _T("ID_M_PLBL37"));
	m_plblSettings->SetBorderState(uiRect::BORDER_NONE);
	m_plblSettings->SetForegroundColour(wxColour(255,255,255));
	m_plblSettings->SetBackgroundColour(wxColour(61,146,146));
	wxFont m_plblSettingsFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblSettings->SetFont(m_plblSettingsFont);
	m_pbtnClose = new wmButton(pnlMain, ID_M_PBTN1, _("Close"), wxPoint(645,425), wxSize(150,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnSelectAll = new wmButton(pnlMain, ID_M_PBTN6, _("Select All"), wxPoint(645,185), wxSize(150,40), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnSelectAll->SetBackgroundColour(wxColour(176,176,176));
	m_pbtnSelectNone = new wmButton(pnlMain, ID_M_PBTN5, _("Deselect All"), wxPoint(645,230), wxSize(150,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnSelectNone->SetBackgroundColour(wxColour(176,176,176));
	m_pbtnUpdate = new wmButton(pnlMain, ID_M_PBTN4, _("Update Selected"), wxPoint(645,370), wxSize(150,50), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnUpdate->Disable();
	m_pbtnUpdate->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnUpdate->SetColourSelected(wxColour(wxT("#008040")));
	m_pbtnUpdate->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnChangelog = new wmButton(pnlMain, ID_M_PBTN2, _("View Changelog"), wxPoint(645,280), wxSize(150,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnChangelog->Disable();
	m_pbtnChangelog->SetBackgroundColour(wxColour(0,125,125));
	m_pbtnChangelog->SetColourSelected(wxColour(wxT("#FF8000")));
	m_pbtnChangelog->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnDependencies = new wmButton(pnlMain, ID_M_PBTN3, _("View Dependencies"), wxPoint(645,325), wxSize(150,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnDependencies->Disable();
	m_pbtnDependencies->SetBackgroundColour(wxColour(0,125,125));
	m_pbtnDependencies->SetColourSelected(wxColour(wxT("#FF8000")));
	m_pbtnDependencies->SetColourDisabled(wxColour(wxT("#808080")));
	m_pswpMain = new wmSwitcherPanel(pnlMain, ID_M_PSWP2, wxPoint(0,45), wxSize(650,430), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
	m_pswpMain->SetPageNameStyle(0);
	Panel1 = new wxPanel(m_pswpMain, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(Panel1, ID_M_PLBL1, _("Name"), wxPoint(2,0), wxSize(158,30), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,0,128));
	m_pLbl4 = new wmLabel(Panel1, ID_M_PLBL4, _("Type"), wxPoint(162,0), wxSize(158,30), 0, _T("ID_M_PLBL4"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(0,0,128));
	m_pLbl2 = new wmLabel(Panel1, ID_M_PLBL2, _("Current Version"), wxPoint(322,0), wxSize(158,30), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,128));
	m_pLbl3 = new wmLabel(Panel1, ID_M_PLBL3, _("Available Version"), wxPoint(482,0), wxSize(158,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,0,128));
	m_plstUpdates = new wmList(Panel1, ID_M_PLST1, wxPoint(0,30), wxSize(645,400), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI|wmList::STYLE_SELECT_ROW, 1, wxSize(-1,30), 4, wxSize(2,1));
	m_plstUpdates->SetBackgroundColour(wxColour(0,0,0));
	m_plstUpdates->SetBorderStyle(2);
	m_plstUpdates->SetButtonColour(wxColour(wxT("#FFFFFF")));
	m_plstUpdates->SetPressedButtonColour(wxColour(wxT("#0080FF")));
	m_plstUpdates->SetSelectedButtonColour(wxColour(wxT("#0080C0")));
	m_plstUpdates->SetTextButtonColour(wxColour(wxT("#000000")));
	Panel2 = new wxPanel(m_pswpMain, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	m_phtmlChangelog = new wxTouchScreenHtml(Panel2, ID_HTMLWINDOW1, wxPoint(0,0), wxSize(650,430), 0, _T("ID_HTMLWINDOW1"));
	Panel3 = new wxPanel(m_pswpMain, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	m_phtmlDependencies = new wxTouchScreenHtml(Panel3, ID_HTMLWINDOW2, wxPoint(0,0), wxSize(650,430), wxHW_SCROLLBAR_AUTO, _T("ID_HTMLWINDOW2"));
	Panel4 = new wxPanel(m_pswpMain, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	Panel4->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl5 = new wmLabel(Panel4, ID_M_PLBL5, _("Update Log"), wxPoint(2,0), wxSize(641,30), 0, _T("ID_M_PLBL5"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(0,0,128));
	m_plstLog = new wmList(Panel4, ID_M_PLST2, wxPoint(0,30), wxSize(645,400), 0, 1, wxSize(-1,30), 1, wxSize(2,1));
	m_plstLog->SetBackgroundColour(wxColour(0,0,0));
	m_plstLog->SetBorderStyle(2);
	m_plstLog->SetButtonColour(wxColour(wxT("#FFFFFF")));
	m_plstLog->SetTextButtonColour(wxColour(wxT("#000000")));
	m_pswpMain->AddPage(Panel1, _("List"), false);
	m_pswpMain->AddPage(Panel2, _("Changelog"), false);
	m_pswpMain->AddPage(Panel3, _("Dependencies"), false);
	m_pswpMain->AddPage(Panel4, _("Log"), false);
	m_pbtnShow = new wmButton(pnlMain, ID_M_PBTN7, _("Show"), wxPoint(645,45), wxSize(150,30), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnShow->SetBackgroundColour(wxColour(0,0,128));
	m_plstShow = new wmList(pnlMain, ID_M_PLST3, wxPoint(645,80), wxSize(150,100), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,33), 3, wxSize(-1,-1));
	m_plstShow->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plstShowFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plstShow->SetFont(m_plstShowFont);
	m_plstShow->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	Panel5 = new wxPanel(m_pswpSplash, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
	Panel5->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl6 = new wmLabel(Panel5, ID_M_PLBL6, _("Please wait retrieving update list..."), wxPoint(0,40), wxSize(800,440), 0, _T("ID_M_PLBL6"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->SetForegroundColour(wxColour(128,128,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl6Font(28,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_pLbl6->SetFont(m_pLbl6Font);
	m_pLbl7 = new wmLabel(Panel5, ID_M_PLBL7, _("PAM Update Manager"), wxPoint(0,0), wxSize(800,40), 0, _T("ID_M_PLBL7"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(61,146,146));
	wxFont m_pLbl7Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl7->SetFont(m_pLbl7Font);
	m_pswpSplash->AddPage(pnlMain, _("Main"), false);
	m_pswpSplash->AddPage(Panel5, _("Splash"), true);
	m_timerUpdate.SetOwner(this, ID_TIMER1);
	m_timerStart.SetOwner(this, ID_TIMER2);
	m_timerStart.Start(100, true);

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnCloseClick);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnSelectAllClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnSelectNoneClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnUpdateClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnChangelogClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnDependenciesClick);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnlstUpdatesSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnlstUpdatesSelected);
	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnShowClick);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnlstShowSelected);
	Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&pamupdatemanagerDialog::OntimerUpdateTrigger);
	Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&pamupdatemanagerDialog::Onm_timerStartTrigger);
	//*)

	 Settings::Get().Write(wxT("Version"), wxT("pamupdatemanager"), wxString::Format(wxT("%d.%d.%d.%d"), AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::BUILD, AutoVersion::REVISION));

	 m_pbtnShow->SetToggle(true, wxT("New"), wxT("All"), 40);
	 m_pbtnShow->ToggleSelection(Settings::Get().Read(wxT("Update"), wxT("ShowAll"), 0), false);

	 m_plstShow->AddButton(wxT("App"));
	 m_plstShow->AddButton(wxT("Lib"));
	 m_plstShow->AddButton(wxT("Monitor"));
	 m_plstShow->AddButton(wxT("Tests"));
	 m_plstShow->AddButton(wxT("Docs"));
	 m_plstShow->AddButton(wxT("Profiles"));
	 m_plstShow->AddButton(wxT("Config"));
	 m_plstShow->SelectAll(true, false);

}

pamupdatemanagerDialog::~pamupdatemanagerDialog()
{
	//(*Destroy(pamupdatemanagerDialog)
	//*)
	#ifdef __WXGNU__
	wxExecute(wxT("sudo umount /mnt/share"), wxEXEC_SYNC);
	#endif // __WXGNU__
}


void pamupdatemanagerDialog::OnbtnCloseClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}


wxString pamupdatemanagerDialog::GetMonitorPluginVersion(const wxString& sPlugin)
{
    plugin aPlugin = MonitorPluginFactory::Get()->GetPluginDetails(MonitorPluginFactory::Get()->GetPluginDirectory(), sPlugin);
    if(aPlugin.sName != wxEmptyString)
    {
        return aPlugin.sVersion;
    }
    return wxT("0.0.0.0");
}

wxString pamupdatemanagerDialog::GetTestPluginVersion(const wxString& sPlugin)
{
    plugin aPlugin = TestPluginFactory::Get()->GetPluginDetails(TestPluginFactory::Get()->GetPluginDirectory(), sPlugin);
    if(aPlugin.sName != wxEmptyString)
    {
        return aPlugin.sVersion;
    }
    return wxT("0.0.0.0");
}

void pamupdatemanagerDialog::OnbtnSelectAllClick(wxCommandEvent& event)
{
    m_plstUpdates->SelectAll(true,true);
}

void pamupdatemanagerDialog::OnbtnSelectNoneClick(wxCommandEvent& event)
{
    m_plstUpdates->SelectAll(false,true);
    m_pbtnChangelog->Enable(false);
    m_pbtnDependencies->Enable(false);
    m_pbtnUpdate->Enable(false);
}

void pamupdatemanagerDialog::OnbtnChangelogClick(wxCommandEvent& event)
{
    m_pbtnDependencies->ToggleSelection(false, false);
    if(event.IsChecked())
    {
        m_pswpMain->ChangeSelection(wxT("Changelog"));
        wxString sText;
        wxArrayInt aiButtons(m_plstUpdates->GetSelectedButtons());
        for(int i = 0; i < aiButtons.GetCount(); i+=4)
        {
            sText << wxT("<h3>") << m_plstUpdates->GetButtonText(aiButtons[i]) << wxT("   (") << UpdateManager::Get().GetVersion(m_plstUpdates->GetButtonText(aiButtons[i])) << wxT(")</h3>");
            sText << UpdateManager::Get().GetChangelog(m_plstUpdates->GetButtonText(aiButtons[i])) << wxT("</br>");
        }
        m_phtmlChangelog->SetPage(sText);
    }
    else
    {
        m_pswpMain->ChangeSelection(wxT("List"));
    }

}

void pamupdatemanagerDialog::OnbtnDependenciesClick(wxCommandEvent& event)
{
    m_pbtnChangelog->ToggleSelection(false, false);
    if(event.IsChecked())
    {
        m_pswpMain->ChangeSelection(wxT("Dependencies"));

        wxString sText;
        wxArrayInt aiButtons(m_plstUpdates->GetSelectedButtons());
        for(int i = 0; i < aiButtons.GetCount(); i+=4)
        {
            sText << wxT("<h3>") << m_plstUpdates->GetButtonText(aiButtons[i]) << wxT("   (") << UpdateManager::Get().GetVersion(m_plstUpdates->GetButtonText(aiButtons[i])) << wxT(")</h3>");
            map<wxString,wxString> mDependsOn(UpdateManager::Get().GetDependencies(m_plstUpdates->GetButtonText(aiButtons[i])));
            if(mDependsOn.empty() == false)
            {
                sText << wxT("<table border='1' cellpadding='3'><tr><td bgcolor='#aaaaff'>Dependency</td><td bgcolor='#aaaaff'>Version</td></tr>");
                for(map<wxString,wxString>::iterator itDepends = mDependsOn.begin(); itDepends != mDependsOn.end(); ++itDepends)
                {
                    sText << wxT("<tr><td>") << itDepends->first << wxT("</td><td>") << itDepends->second << wxT("</td></tr>");
                }
                sText << wxT("</table>");
            }
        }
        m_phtmlDependencies->SetPage(sText);

    }
    else
    {
        m_pswpMain->ChangeSelection(wxT("List"));
    }
}

void pamupdatemanagerDialog::OnbtnUpdateClick(wxCommandEvent& event)
{
    if(m_pswpMain->GetPageText(m_pswpMain->GetSelection()) != wxT("Log"))
    {
        m_pbtnChangelog->Enable(false);
        m_pbtnDependencies->Enable(false);
        m_pbtnUpdate->Enable(false);
        m_pbtnSelectAll->Enable(false);
        m_pbtnSelectNone->Enable(false);
        m_pswpMain->ChangeSelection(wxT("Log"));

        m_pbtnUpdate->SetLabel(wxT("Updating..."));

        m_mUpdate.clear();
        m_plstLog->Clear();

        wxString sMessage;
        //we need to download all the selected and all the dependencies...
        wxArrayInt aiButtons(m_plstUpdates->GetSelectedButtons());


        for(int i = 0; i < aiButtons.GetCount(); i+=4)
        {

            wxString sUpdate(m_plstUpdates->GetButtonText(aiButtons[i]));
            wxString sVersion(UpdateManager::Get().GetVersion(sUpdate));


            pair<map<wxString, wxString>::iterator, bool> ins = m_mUpdate.insert(make_pair(sUpdate, sVersion));
            if(ins.second == false)
            {   //different version in list
                if(ins.first->second != sVersion)
                {
                    size_t nIndex = m_plstLog->AddButton(wxString::Format(wxT("'%s' cannot be updated to version '%s' as it is already being updating to version '%s'"), sUpdate.c_str(), sVersion.c_str(), ins.first->second.c_str()));
                    m_plstLog->ShowButton(nIndex, wmList::BOTTOM, false);
                }
                else
                {
                    size_t nIndex = m_plstLog->AddButton(wxString::Format(wxT("Selected '%s' already a dependency"), sUpdate.c_str()), wxNullBitmap, 0, wmList::wmENABLED, wxColour(100,200,100));
                    m_plstLog->ShowButton(nIndex, wmList::BOTTOM, false);
                }
            }
            else
            {
                size_t nIndex = m_plstLog->AddButton(wxString::Format(wxT("Selected '%s'"), sUpdate.c_str()), wxNullBitmap, 0, wmList::wmENABLED, wxColour(100,200,100));
                m_plstLog->ShowButton(nIndex, wmList::BOTTOM, false);
            }

            map<wxString,wxString> mDependsOn(UpdateManager::Get().GetDependencies(sUpdate));
            for(map<wxString,wxString>::iterator itDependsOn = mDependsOn.begin(); itDependsOn != mDependsOn.end(); ++itDependsOn)
            {
                pair<map<wxString, wxString>::iterator, bool> ins = m_mUpdate.insert(make_pair(itDependsOn->first, itDependsOn->second));
                if(ins.second == false)
                {
                    if(ins.first->second != itDependsOn->second)//different version in list
                    {
                        size_t nIndex = m_plstLog->AddButton(wxString::Format(wxT("Dependency '%s' cannot be updated to version %s as it is already begin updating to version %s"), itDependsOn->first.c_str(), itDependsOn->second.c_str(), ins.first->second.c_str()));
                        m_plstLog->ShowButton(nIndex, wmList::BOTTOM, false);
                    }
                }
                else
                {
                    size_t nIndex = m_plstLog->AddButton(wxString::Format(wxT("Dependency '%s'"), itDependsOn->first.c_str()), wxNullBitmap, 0, wmList::wmENABLED, wxColour(100,200,100));
                    m_plstLog->ShowButton(nIndex, wmList::BOTTOM, false);
                }
            }
        }

        m_plstLog->AddButton(wxEmptyString);
        size_t nIndex = m_plstLog->AddButton(wxT("Updating..."));
        m_plstLog->ShowButton(nIndex, wmList::BOTTOM, false);


        m_bUpdate = false;
        m_timerUpdate.Start(10,true);
    }
    else
    {
        m_pbtnChangelog->Enable(true);
        m_pbtnDependencies->Enable(true);
        m_pbtnSelectAll->Enable(true);
        m_pbtnSelectNone->Enable(true);

        m_pswpMain->ChangeSelection(wxT("List"));

        m_pbtnUpdate->SetLabel(wxT("Update Selected"));

    }

}

void pamupdatemanagerDialog::OnlstUpdatesSelected(wxCommandEvent& event)
{
    m_pbtnChangelog->Enable((m_plstUpdates->GetSelectionCount()!=0));
    m_pbtnDependencies->Enable((m_plstUpdates->GetSelectionCount()!=0));
    m_pbtnUpdate->Enable((m_plstUpdates->GetSelectionCount()!=0));
}

void pamupdatemanagerDialog::OntimerUpdateTrigger(wxTimerEvent& event)
{
    wxLogNull ln;
    map<wxString, wxString>::iterator itUpdate = m_mUpdate.begin();
    if(itUpdate != m_mUpdate.end())
    {
        if(!m_bUpdate)
        {
            size_t nIndex = m_plstLog->AddButton(wxString::Format(wxT("Update '%s' to %s"), itUpdate->first.c_str(), itUpdate->second.c_str()));
            m_plstLog->ShowButton(nIndex, wmList::BOTTOM, false);

            m_bUpdate = true;
            m_timerUpdate.Start(100,true);
            m_plstLog->Refresh();
        }
        else
        {
            if(UpdateManager::Get().Update(itUpdate->first) == false)
            {
                size_t nIndex = m_plstLog->AddButton(wxString::Format(wxT("Failed to update '%s'"), itUpdate->first.c_str()), wxNullBitmap, 0, wmList::wmENABLED, wxColour(200,100,100));
                m_plstLog->ShowButton(nIndex, wmList::BOTTOM, false);
            }
            else
            {
                size_t nIndex = m_plstLog->AddButton(wxString::Format(wxT("Updated '%s'"), itUpdate->first.c_str()), wxNullBitmap, 0, wmList::wmENABLED, wxColour(100,200,100));
                m_plstLog->ShowButton(nIndex, wmList::BOTTOM, false);
            }
            m_bUpdate = false;
            m_mUpdate.erase(itUpdate);
            m_timerUpdate.Start(100,true);
            m_plstLog->Refresh();
        }

    }
    else
    {
        m_pbtnUpdate->SetLabel(wxT("Back to list"));
        m_pbtnUpdate->Enable(true);
    }
}

void pamupdatemanagerDialog::Onm_timerStartTrigger(wxTimerEvent& event)
{
    	m_plblSettings->SetLabel(wxT("PAM Update Manager"));

	SetPosition(wxPoint(0,0));

	m_plstUpdates->SetGradient(0);
	m_plstLog->SetGradient(0);
	m_plstLog->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	UpdateManager::Get().GetUpdateList();

	PopulateUpdateList();

    m_pswpSplash->ChangeSelection(wxT("Main"));
}


void pamupdatemanagerDialog::PopulateUpdateList()
{
    m_plstUpdates->Freeze();
    m_plstUpdates->Clear();

    for(map<wxString, UpdateObject>::const_iterator itObject = UpdateManager::Get().GetUpdateListBegin(); itObject != UpdateManager::Get().GetUpdateListEnd(); ++itObject)
    {
        wxString sLocalVersion;
        wxString sType;

        switch(itObject->second.nType)
        {
        case UpdateObject::APP:
            sType = wxT("App");
            sLocalVersion = Settings::Get().Read(wxT("Version"), itObject->first, wxT("0.0.0.0"));
            break;
        case UpdateObject::CORE_DLL:
            sType = wxT("Core Lib");
            if(itObject->first == wxT("pambase"))
            {
                sLocalVersion  = Settings::Get().GetLibraryVersion();
            }
            else
            {
                sLocalVersion = wxT("0.0.0.0"); //@todo get core lib version
            }

            break;
        case UpdateObject::DOCUMENTATION:
            sType = wxT("Doc");
            sLocalVersion = wxT("0.0.0.0");
            break;
        case UpdateObject::PLUGIN_MONITOR:
            sType=wxT("Monitor");
            sLocalVersion = GetMonitorPluginVersion(itObject->first);
            break;
        case UpdateObject::PLUGIN_TEST:
            sType = wxT("Test");
            sLocalVersion = GetTestPluginVersion(itObject->first);
            break;
        case UpdateObject::PROFILES:
            sType = wxT("Profile");
            sLocalVersion = wxEmptyString;
            break;
        case UpdateObject::CONFIG:
            sType = wxT("Config");
            sLocalVersion = wxEmptyString;
            break;
        }

        if(UpdateManager::Get().UpdateVersionIsNewer(itObject->second.sVersion, sLocalVersion) || Settings::Get().Read(wxT("Update"), wxT("ShowAll"), 0))
        {
            if(m_plstShow->IsSelected(itObject->second.nType-1))
            {
                m_plstUpdates->AddButton(itObject->first);
                m_plstUpdates->AddButton(sType);
                m_plstUpdates->AddButton(sLocalVersion);
                m_plstUpdates->AddButton(itObject->second.sVersion);
            }
        }
    }
    m_plstUpdates->Thaw();
}

void pamupdatemanagerDialog::OnbtnShowClick(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Update"), wxT("ShowAll"), event.IsChecked());
    PopulateUpdateList();
}

void pamupdatemanagerDialog::OnlstShowSelected(wxCommandEvent& event)
{
    PopulateUpdateList();
}
