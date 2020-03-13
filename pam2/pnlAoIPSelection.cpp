#include "pnlAoIPSelection.h"
#include "settings.h"
#include <wx/app.h>
#include "pam2Main.h"
#include <set>
#include "images/end_hz.xpm"
#include "images/end_hz_press.xpm"
#include "images/home_hz_press.xpm"
#include "images/home_hz.xpm"
#include "images/pagedown.xpm"
#include "images/pagedown_press.xpm"
#include "images/pageup.xpm"
#include "images/pageup_press.xpm"
#include "aoipsourcemanager.h"

//(*InternalHeaders(pnlAoIPSelection)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlAoIPSelection)
const long pnlAoIPSelection::ID_M_PLBL3 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN3 = wxNewId();
const long pnlAoIPSelection::ID_M_PLST1 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN1 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN2 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN4 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN5 = wxNewId();
const long pnlAoIPSelection::ID_PANEL1 = wxNewId();
const long pnlAoIPSelection::ID_M_PLST2 = wxNewId();
const long pnlAoIPSelection::ID_M_PLBL1 = wxNewId();
const long pnlAoIPSelection::ID_M_PLST3 = wxNewId();
const long pnlAoIPSelection::ID_M_PBTN6 = wxNewId();
const long pnlAoIPSelection::ID_PANEL2 = wxNewId();
const long pnlAoIPSelection::ID_M_PLST4 = wxNewId();
const long pnlAoIPSelection::ID_M_PLST5 = wxNewId();
const long pnlAoIPSelection::ID_PANEL3 = wxNewId();
const long pnlAoIPSelection::ID_M_PSWP1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlAoIPSelection,wxPanel)
	//(*EventTable(pnlAoIPSelection)
	//*)
END_EVENT_TABLE()

using namespace std;

pnlAoIPSelection::pnlAoIPSelection(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, int n, const wxString& s)
{
	//(*Initialize(pnlAoIPSelection)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("AES67 / SMPTE2110-30 Sources"), wxPoint(0,5), wxSize(690,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl3Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_pbtnBack = new wmButton(this, ID_M_PBTN3, _("Back"), wxPoint(695,4), wxSize(100,31), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnBack->SetForegroundColour(wxColour(255,255,255));
	m_pbtnBack->SetBackgroundColour(wxColour(0,128,64));
	wxFont m_pbtnBackFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pbtnBack->SetFont(m_pbtnBackFont);
	m_pbtnBack->SetColourDisabled(wxColour(wxT("#808080")));
	m_pSwp1 = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,35), wxSize(800,440), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pSwp1->SetPageNameStyle(3);
	Panel1 = new wxPanel(m_pSwp1, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	Panel1->SetBackgroundColour(wxColour(0,0,0));
	m_plstDevices = new wmList(Panel1, ID_M_PLST1, wxPoint(0,0), wxSize(800,338), wmList::STYLE_SELECT, 2, wxSize(-1,0), 5, wxSize(2,2));
	m_plstDevices->SetBackgroundColour(wxColour(0,0,0));
	m_plstDevices->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstDevices->SetDisabledColour(wxColour(wxT("#808080")));
	m_pbtnHome = new wmButton(Panel1, ID_M_PBTN1, wxEmptyString, wxPoint(70,360), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnHome->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnPrevious = new wmButton(Panel1, ID_M_PBTN2, wxEmptyString, wxPoint(190,360), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnPrevious->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnNext = new wmButton(Panel1, ID_M_PBTN4, wxEmptyString, wxPoint(310,360), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnNext->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnEnd = new wmButton(Panel1, ID_M_PBTN5, wxEmptyString, wxPoint(430,360), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnEnd->SetColourDisabled(wxColour(wxT("#808080")));
	Panel2 = new wxPanel(m_pSwp1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	Panel2->SetBackgroundColour(wxColour(0,0,0));
	m_plstAlphebtical = new wmList(Panel2, ID_M_PLST2, wxPoint(0,0), wxSize(800,295), wmList::STYLE_SELECT, 2, wxSize(-1,0), 5, wxSize(2,2));
	m_plstAlphebtical->SetBackgroundColour(wxColour(0,0,0));
	m_plstAlphebtical->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstAlphebtical->SetDisabledColour(wxColour(wxT("#808080")));
	m_plblSearch = new wmLabel(Panel2, ID_M_PLBL1, wxEmptyString, wxPoint(200,300), wxSize(400,30), 0, _T("ID_M_PLBL1"));
	m_plblSearch->SetBorderState(uiRect::BORDER_NONE);
	m_plblSearch->GetUiRect().SetGradient(0);
	m_plblSearch->SetForegroundColour(wxColour(0,0,0));
	m_plblSearch->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_plblSearchFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblSearch->SetFont(m_plblSearchFont);
	m_plstAlphabet = new wmList(Panel2, ID_M_PLST3, wxPoint(0,330), wxSize(745,80), 0, 0, wxSize(-1,35), 18, wxSize(0,0));
	m_plstAlphabet->SetBackgroundColour(wxColour(0,0,0));
	m_plstAlphabet->SetButtonColour(wxColour(wxT("#004080")));
	m_plstAlphabet->SetSelectedButtonColour(wxColour(wxT("#1169EE")));
	m_plstAlphabet->SetDisabledColour(wxColour(wxT("#808080")));
	m_plstAlphabet->SetTextButtonColour(wxColour(wxT("#FFFFFF")));
	m_pbtnDelete = new wmButton(Panel2, ID_M_PBTN6, _("DEL"), wxPoint(745,332), wxSize(50,76), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnDelete->SetForegroundColour(wxColour(255,255,255));
	m_pbtnDelete->SetBackgroundColour(wxColour(0,64,128));
	wxFont m_pbtnDeleteFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pbtnDelete->SetFont(m_pbtnDeleteFont);
	m_pbtnDelete->SetColourDisabled(wxColour(wxT("#808080")));
	Panel3 = new wxPanel(m_pSwp1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	Panel3->SetBackgroundColour(wxColour(0,0,0));
	m_plstTagged = new wmList(Panel3, ID_M_PLST4, wxPoint(0,0), wxSize(800,260), wmList::STYLE_SELECT, 2, wxSize(-1,0), 5, wxSize(2,2));
	m_plstTagged->SetBackgroundColour(wxColour(0,0,0));
	m_plstTagged->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstTagged->SetDisabledColour(wxColour(wxT("#808080")));
	m_plstTags = new wmList(Panel3, ID_M_PLST5, wxPoint(0,265), wxSize(800,130), wmList::STYLE_SELECT, 0, wxSize(-1,35), 10, wxSize(0,0));
	m_plstTags->SetBackgroundColour(wxColour(0,0,0));
	m_plstTags->SetSelectedButtonColour(wxColour(wxT("#1169EE")));
	m_plstTags->SetDisabledColour(wxColour(wxT("#808080")));
	m_pSwp1->AddPage(Panel1, _("Paged"), false);
	m_pSwp1->AddPage(Panel2, _("Alphabetical"), false);
	m_pSwp1->AddPage(Panel3, _("Tags"), false);

	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnBackClick);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlAoIPSelection::OnlstDevicesSelected);
	Connect(ID_M_PLST1,wxEVT_LIST_PAGED,(wxObjectEventFunction)&pnlAoIPSelection::OnlstDevicesPaged);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnHomeClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnPreviousClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnNextClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnEndClick);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlAoIPSelection::OnlstDevicesSelected);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlAoIPSelection::OnlstAlphabetSelected);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlAoIPSelection::OnbtnDeleteClick);
	Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlAoIPSelection::OnlstDevicesSelected);
	Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlAoIPSelection::OnlstTagsSelected);
	Connect(ID_M_PSWP1,wxEVT_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&pnlAoIPSelection::OnSwpPageChanged);
	//*)
	Connect(wxEVT_SHOW, (wxObjectEventFunction)&pnlAoIPSelection::OnShown);

	m_pbtnEnd->SetBitmapLabel(wxBitmap(end_hz_xpm));
    m_pbtnEnd->SetBitmapSelected(wxBitmap(end_hz_press_xpm));
    m_pbtnHome->SetBitmapLabel(wxBitmap(home_hz_xpm));
    m_pbtnHome->SetBitmapSelected(wxBitmap(home_hz_press_xpm));
    m_pbtnPrevious->SetBitmapLabel(wxBitmap(pageup_xpm));
    m_pbtnPrevious->SetBitmapSelected(wxBitmap(pageup_press_xpm));
    m_pbtnNext->SetBitmapLabel(wxBitmap(pagedown_xpm));
    m_pbtnNext->SetBitmapSelected(wxBitmap(pagedown_press_xpm));

    for(int i = 0; i < 10; i++)
    {
        wxString str(wxString::Format("%d",i));
        m_plstAlphabet->AddButton(str);
    }
    for(char ascii = 65; ascii < 91; ascii++)
    {
        wxString str(wxString::Format("%c", ascii));
        m_plstAlphabet->AddButton(str);
    }

    AoipSourceManager::Get();
    DoAlphabetSearch(wxEmptyString);


	SetPosition(pos);
	SetSize(size);
}

pnlAoIPSelection::~pnlAoIPSelection()
{
	//(*Destroy(pnlAoIPSelection)
	//*)
}

void pnlAoIPSelection::OnShown(wxShowEvent& event)
{
    if(event.IsShown())
    {
        wxString sScreen(Settings::Get().Read("AoIPSelection", "Screen", "Paged"));
        if(sScreen == "Paged")
        {
            ShowPaged();
        }
        else if(sScreen == "Alphabetical")
        {
            ShowAlphabetical();
        }
        else if(sScreen == "Tagged")
        {
            ShowTagged();
        }
        m_pSwp1->ChangeSelection(sScreen);
    }
}


void pnlAoIPSelection::ShowPaged()
{
    m_plstDevices->Freeze();
    m_plstDevices->Clear();
    int i = 0;
    for(auto itSource = AoipSourceManager::Get().GetSourceBegin(); itSource != AoipSourceManager::Get().GetSourceEnd(); ++itSource)
    {
        m_plstDevices->AddButton(itSource->second.sName, wxNullBitmap, (void*)itSource->first);
    }

    m_plstDevices->Thaw();
    ShowPagingButtons();

    size_t nButton = m_plstDevices->FindButton((void*)Settings::Get().Read(wxT("Input"), wxT("AoIP"), 0));
    if(nButton != 0xFFFFFFFF)
    {
        m_plstDevices->SelectButton(nButton,false);
    }
}

void pnlAoIPSelection::ShowAlphabetical()
{
    m_plstAlphabet->SelectButton(Settings::Get().Read("AoIPSelection", "Alphabet", "A"));
}


void pnlAoIPSelection::OnlstDevicesSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Input"), wxT("AoIP"), (int)event.GetClientData());
    ShowMainScreen();
}

void pnlAoIPSelection::OnbtnHomeClick(wxCommandEvent& event)
{
    m_plstDevices->ShowFirstPage(false,false);

}

void pnlAoIPSelection::OnbtnPreviousClick(wxCommandEvent& event)
{
    m_plstDevices->ShowPreviousPage(false, false);

}

void pnlAoIPSelection::OnbtnNextClick(wxCommandEvent& event)
{
    m_plstDevices->ShowNextPage(false, false);

}

void pnlAoIPSelection::OnbtnEndClick(wxCommandEvent& event)
{
    m_plstDevices->ShowLastPage(false, false);

}

void pnlAoIPSelection::ShowPagingButtons()
{
    m_pbtnEnd->Show(m_plstDevices->GetPageCount() > 1 && m_plstDevices->GetCurrentPageNumber() < m_plstDevices->GetPageCount());
    m_pbtnHome->Show(m_plstDevices->GetPageCount() > 1 && m_plstDevices->GetCurrentPageNumber() > 1);
    m_pbtnPrevious->Show(m_plstDevices->GetPageCount() > 1 && m_plstDevices->GetCurrentPageNumber() > 1);
    m_pbtnNext->Show(m_plstDevices->GetPageCount() > 1 && m_plstDevices->GetCurrentPageNumber() < m_plstDevices->GetPageCount());
}

void pnlAoIPSelection::OnbtnBackClick(wxCommandEvent& event)
{
    ShowMainScreen();
}

void pnlAoIPSelection::ShowMainScreen()
{
    wmSwitcherPanel* pPanel = dynamic_cast<wmSwitcherPanel*>(GetParent());
    if(pPanel)
    {
        pPanel->ChangeSelection("Main");
    }
}

void pnlAoIPSelection::OnlstAlphabetSelected(wxCommandEvent& event)
{
   DoAlphabetSearch(event.GetString());
}

void pnlAoIPSelection::DoAlphabetSearch(const wxString& sLetter)
{
    std::map<wxString, unsigned int> mMatch;
    std::set<wxString> setAlphabet;

    wxString sSearch;
    if(sLetter == "Del")
    {
        if(m_sSearch.empty() == false)
        {
            sSearch = m_sSearch.Left(m_sSearch.length()-1);
        }
    }
    else
    {
        sSearch = m_sSearch+sLetter;
    }


    for(auto itSource = AoipSourceManager::Get().GetSourceBegin(); itSource != AoipSourceManager::Get().GetSourceEnd(); ++itSource)
    {
        wxString sCheck(itSource->second.sName.Upper());
        sCheck.Replace(" ", "");
        if(sCheck.length() >= sSearch.length() && sCheck.Left(sSearch.length()) == sSearch)
        {
            mMatch.insert(std::make_pair(itSource->second.sName, itSource->first));
            if(sCheck.length() > sSearch.length())
            {
                setAlphabet.insert(sCheck.Mid(sSearch.length(),1).Upper());
            }
        }
    }

    if(mMatch.empty() == false)
    {
        m_sSearch = sSearch;
        m_plblSearch->SetLabel(m_sSearch);
        m_plstAlphebtical->Freeze();
        m_plstAlphebtical->Clear();
        for(auto pairSource : mMatch)
        {
            m_plstAlphebtical->AddButton(pairSource.first, wxNullBitmap, (void*)pairSource.second);
        }
        m_plstAlphebtical->Thaw();

        size_t nButton = m_plstAlphebtical->FindButton((void*)Settings::Get().Read(wxT("Input"), wxT("AoIP"), 0));
        if(nButton != 0xFFFFFFFF)
        {
            m_plstAlphebtical->SelectButton(nButton,false);
        }
    }

    m_plstAlphabet->Freeze();
    for(size_t i = 0; i < m_plstAlphabet->GetItemCount(); i++)
    {
        m_plstAlphabet->EnableButton(i, (setAlphabet.find(m_plstAlphabet->GetButtonText(i)) != setAlphabet.end()) ? wmList::wmENABLED : wmList::wmDISABLED);
    }
    m_plstAlphabet->Thaw();
}


void pnlAoIPSelection::OnlstTagsSelected(wxCommandEvent& event)
{
    DoTagSearch(event.GetString());
}

void pnlAoIPSelection::OnSwpPageChanged(wxBookCtrlEvent& event)
{
    Settings::Get().Write("AoIPSelection", "Screen", m_pSwp1->GetSelectionName());
    if(m_pSwp1->GetSelectionName() == "Paged")
    {
        ShowPaged();
    }
    else if(m_pSwp1->GetSelectionName() == "Alphabetical")
    {
        ShowAlphabetical();
    }
    else if(m_pSwp1->GetSelectionName() == "Tags")
    {
        ShowTagged();
    }
}


void pnlAoIPSelection::ShowTagged()
{
    DoTagSearch(wxEmptyString);
}

void pnlAoIPSelection::OnlstDevicesPaged(wxCommandEvent& event)
{
    ShowPagingButtons();
}

void pnlAoIPSelection::OnbtnDeleteClick(wxCommandEvent& event)
{
    DoAlphabetSearch("Del");
}

void pnlAoIPSelection::DoTagSearch(const wxString& sTag)
{

    if(m_lstTag.empty())
    {
        m_mTagged.insert(AoipSourceManager::Get().GetSourceBegin(), AoipSourceManager::Get().GetSourceEnd());
    }

    std::map<unsigned int, AoIPSource> mTemp;
    std::set<wxString> setTags;
    for(auto pairSource : m_mTagged)
    {
        if(sTag.empty() || pairSource.second.setTags.find(sTag) != pairSource.second.setTags.end())
        {
            //keep the source
            mTemp.insert(pairSource);

            //save all the possible tags
            setTags.insert(pairSource.second.setTags.begin(), pairSource.second.setTags.end());
        }
    }

    if(mTemp.empty() == false)
    {
        m_mTagged = mTemp;  //copy accross
        m_lstTag.push_back(sTag);  //store the breadcrumn

        //remove all breadcrumbs from possible choices
        for(auto tag : m_lstTag)
        {
            setTags.erase(tag);
        }

        m_plstTags->Freeze();
        m_plstTags->Clear();
        for(auto tag : setTags)
        {
            m_plstTags->AddButton(tag);
        }
        m_plstTags->Thaw();

        m_plstTagged->Freeze();
        m_plstTagged->Clear();
        for(auto pairSource : m_mTagged)
        {
            m_plstTagged->AddButton(pairSource.second.sName, wxNullBitmap, (void*)pairSource.first);
        }
        m_plstTagged->Thaw();
    }
}
