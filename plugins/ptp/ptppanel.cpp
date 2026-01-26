#include "ptppanel.h"
#include "ptpbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "wxptp.h"
#include "settings.h"
#include "ptpclock.h"
#include "ptpstructs.h"
#include "log.h"
#include <wx/log.h>
#include "macdb.h"
#include "timemanager.h"

//(*InternalHeaders(ptpPanel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

#include "syncmaster.xpm"
#include "grandmaster.xpm"
#include "slave.xpm"

//(*IdInit(ptpPanel)
const long ptpPanel::ID_M_PLBL80 = wxNewId();
const long ptpPanel::ID_M_PLST1 = wxNewId();
const long ptpPanel::ID_M_PLBL79 = wxNewId();
const long ptpPanel::ID_M_PLBL1 = wxNewId();
const long ptpPanel::ID_M_PLBL2 = wxNewId();
const long ptpPanel::ID_M_PLBL3 = wxNewId();
const long ptpPanel::ID_M_PLBL12 = wxNewId();
const long ptpPanel::ID_M_PLBL13 = wxNewId();
const long ptpPanel::ID_M_PLBL14 = wxNewId();
const long ptpPanel::ID_M_PLBL30 = wxNewId();
const long ptpPanel::ID_M_PLBL24 = wxNewId();
const long ptpPanel::ID_M_PLBL25 = wxNewId();
const long ptpPanel::ID_M_PLBL26 = wxNewId();
const long ptpPanel::ID_M_PLBL27 = wxNewId();
const long ptpPanel::ID_M_PLBL4 = wxNewId();
const long ptpPanel::ID_M_PLBL15 = wxNewId();
const long ptpPanel::ID_M_PLBL5 = wxNewId();
const long ptpPanel::ID_M_PLBL16 = wxNewId();
const long ptpPanel::ID_M_PLBL7 = wxNewId();
const long ptpPanel::ID_M_PLBL17 = wxNewId();
const long ptpPanel::ID_M_PLBL8 = wxNewId();
const long ptpPanel::ID_M_PLBL18 = wxNewId();
const long ptpPanel::ID_M_PLBL9 = wxNewId();
const long ptpPanel::ID_M_PLBL19 = wxNewId();
const long ptpPanel::ID_M_PLBL6 = wxNewId();
const long ptpPanel::ID_M_PLBL20 = wxNewId();
const long ptpPanel::ID_M_PLBL11 = wxNewId();
const long ptpPanel::ID_M_PLBL21 = wxNewId();
const long ptpPanel::ID_M_PLBL10 = wxNewId();
const long ptpPanel::ID_M_PLBL35 = wxNewId();
const long ptpPanel::ID_M_PLBL39 = wxNewId();
const long ptpPanel::ID_M_PLBL22 = wxNewId();
const long ptpPanel::ID_PANEL9 = wxNewId();
const long ptpPanel::ID_PANEL1 = wxNewId();
const long ptpPanel::ID_M_PLBL23 = wxNewId();
const long ptpPanel::ID_M_PLBL31 = wxNewId();
const long ptpPanel::ID_M_PLBL32 = wxNewId();
const long ptpPanel::ID_M_PLBL33 = wxNewId();
const long ptpPanel::ID_M_PLBL34 = wxNewId();
const long ptpPanel::ID_PANEL10 = wxNewId();
const long ptpPanel::ID_PANEL2 = wxNewId();
const long ptpPanel::ID_M_PLBL28 = wxNewId();
const long ptpPanel::ID_M_PLBL29 = wxNewId();
const long ptpPanel::ID_M_PLBL36 = wxNewId();
const long ptpPanel::ID_M_PLBL37 = wxNewId();
const long ptpPanel::ID_M_PLBL38 = wxNewId();
const long ptpPanel::ID_PANEL11 = wxNewId();
const long ptpPanel::ID_PANEL3 = wxNewId();
const long ptpPanel::ID_PANEL5 = wxNewId();
const long ptpPanel::ID_M_PLBL40 = wxNewId();
const long ptpPanel::ID_M_PLBL41 = wxNewId();
const long ptpPanel::ID_M_PLBL42 = wxNewId();
const long ptpPanel::ID_M_PLBL43 = wxNewId();
const long ptpPanel::ID_M_PLBL44 = wxNewId();
const long ptpPanel::ID_M_PLBL46 = wxNewId();
const long ptpPanel::ID_M_PLBL47 = wxNewId();
const long ptpPanel::ID_PANEL13 = wxNewId();
const long ptpPanel::ID_PANEL4 = wxNewId();
const long ptpPanel::ID_M_PLBL48 = wxNewId();
const long ptpPanel::ID_M_PLBL49 = wxNewId();
const long ptpPanel::ID_M_PLBL50 = wxNewId();
const long ptpPanel::ID_M_PLBL51 = wxNewId();
const long ptpPanel::ID_M_PLBL52 = wxNewId();
const long ptpPanel::ID_PANEL12 = wxNewId();
const long ptpPanel::ID_PANEL7 = wxNewId();
const long ptpPanel::ID_PANEL6 = wxNewId();
const long ptpPanel::ID_M_PSWP1 = wxNewId();
const long ptpPanel::ID_M_PLBL54 = wxNewId();
const long ptpPanel::ID_M_PLBL55 = wxNewId();
const long ptpPanel::ID_M_PLBL56 = wxNewId();
const long ptpPanel::ID_M_PLBL57 = wxNewId();
const long ptpPanel::ID_M_PLBL61 = wxNewId();
const long ptpPanel::ID_M_PLBL58 = wxNewId();
const long ptpPanel::ID_M_PLBL62 = wxNewId();
const long ptpPanel::ID_M_PLBL59 = wxNewId();
const long ptpPanel::ID_M_PLBL63 = wxNewId();
const long ptpPanel::ID_M_PLBL60 = wxNewId();
const long ptpPanel::ID_M_PLBL64 = wxNewId();
const long ptpPanel::ID_M_PBTN29 = wxNewId();
const long ptpPanel::ID_M_PBTN5 = wxNewId();
const long ptpPanel::ID_PANEL8 = wxNewId();
const long ptpPanel::ID_PANEL14 = wxNewId();
const long ptpPanel::ID_M_PLBL45 = wxNewId();
const long ptpPanel::ID_M_PLBL69 = wxNewId();
const long ptpPanel::ID_M_PLBL74 = wxNewId();
const long ptpPanel::ID_M_PLBL70 = wxNewId();
const long ptpPanel::ID_M_PLBL75 = wxNewId();
const long ptpPanel::ID_M_PLBL71 = wxNewId();
const long ptpPanel::ID_M_PLBL76 = wxNewId();
const long ptpPanel::ID_M_PLBL72 = wxNewId();
const long ptpPanel::ID_M_PLBL77 = wxNewId();
const long ptpPanel::ID_M_PLBL73 = wxNewId();
const long ptpPanel::ID_M_PLBL78 = wxNewId();
const long ptpPanel::ID_HISTORY_GRAPH = wxNewId();
const long ptpPanel::ID_M_PLBL68 = wxNewId();
const long ptpPanel::ID_M_PLST5 = wxNewId();
const long ptpPanel::ID_M_PBTN8 = wxNewId();
const long ptpPanel::ID_M_PBTN3 = wxNewId();
const long ptpPanel::ID_M_PBTN7 = wxNewId();
const long ptpPanel::ID_M_PBTN4 = wxNewId();
const long ptpPanel::ID_PANEL15 = wxNewId();
const long ptpPanel::ID_M_PLBL53 = wxNewId();
const long ptpPanel::ID_CUSTOM1 = wxNewId();
const long ptpPanel::ID_M_PLBL67 = wxNewId();
const long ptpPanel::ID_M_PLST4 = wxNewId();
const long ptpPanel::ID_M_PLBL65 = wxNewId();
const long ptpPanel::ID_M_PLST2 = wxNewId();
const long ptpPanel::ID_M_PLBL66 = wxNewId();
const long ptpPanel::ID_M_PLST3 = wxNewId();
const long ptpPanel::ID_M_PBTN1 = wxNewId();
const long ptpPanel::ID_M_PBTN6 = wxNewId();
const long ptpPanel::ID_M_PBTN2 = wxNewId();
const long ptpPanel::ID_PANEL16 = wxNewId();
const long ptpPanel::ID_M_PSWP2 = wxNewId();
//*)

const wxColour ptpPanel::CLR_CLOCK            = wxColour(50,50,150);
const wxColour ptpPanel::CLR_CLOCK_SELECTED   = wxColour(150,150,250);

const wxBitmap ptpPanel::BMP_SLAVE 	 = wxBitmap(slave_xpm);
const wxBitmap ptpPanel::BMP_SYNC	 = wxBitmap(syncmaster_xpm); 
const wxBitmap ptpPanel::BMP_GRAND   = wxBitmap(grandmaster_xpm);

using namespace pml::ptpmonkey;


wxIMPLEMENT_DYNAMIC_CLASS(ptpPanel, pmPanel)

BEGIN_EVENT_TABLE(ptpPanel,pmPanel)
	//(*EventTable(ptpPanel)
	//*)
END_EVENT_TABLE()

const std::map<pml::ptpmonkey::clck::enumAccuracy, wxString> kAccuracy{ { pml::ptpmonkey::clck::enumAccuracy::k25Ns,  "Within 25ns"},
                  { pml::ptpmonkey::clck::enumAccuracy::k100Ns, "Within 100ns"},
                  { pml::ptpmonkey::clck::enumAccuracy::k250Ns, "Within 250ns" },
                  { pml::ptpmonkey::clck::enumAccuracy::k1Us,   "Within 1us"},
                  { pml::ptpmonkey::clck::enumAccuracy::k2_5Us, "Within 2.5us"},
                  { pml::ptpmonkey::clck::enumAccuracy::k10Us,  "Within 10us"},
                  { pml::ptpmonkey::clck::enumAccuracy::k25Us,  "Within 25us"},
                  { pml::ptpmonkey::clck::enumAccuracy::k100Us, "Within 100us"},
                  { pml::ptpmonkey::clck::enumAccuracy::k250Us, "Within 250us"},
                  { pml::ptpmonkey::clck::enumAccuracy::k1Ms,   "Within 1ms"},
                  { pml::ptpmonkey::clck::enumAccuracy::k2_5Ms, "Within 2.5ms"},
                  { pml::ptpmonkey::clck::enumAccuracy::k10Ms,  "Within 10ms"},
                  { pml::ptpmonkey::clck::enumAccuracy::k25Ms,  "Within 25ms"},
                  { pml::ptpmonkey::clck::enumAccuracy::k100Ms, "Within 100ms"},
                  { pml::ptpmonkey::clck::enumAccuracy::k250Ms, "Within 250ms"},
                  { pml::ptpmonkey::clck::enumAccuracy::k1S,    "Within 1s"},
                  { pml::ptpmonkey::clck::enumAccuracy::k10S,   "Within 10s"},
                  { pml::ptpmonkey::clck::enumAccuracy::kOver,  "Over 10ss"},
                  { pml::ptpmonkey::clck::enumAccuracy::kUnknown,"Unknown" } };

const std::map<pml::ptpmonkey::clck::enumTimeSource, wxString> kTimeSource{ { pml::ptpmonkey::clck::enumTimeSource::kAtomic, "Atomic"},
                    { pml::ptpmonkey::clck::enumTimeSource::kGps, "GPS"},
                    { pml::ptpmonkey::clck::enumTimeSource::kRadio,"Radio"},
                    { pml::ptpmonkey::clck::enumTimeSource::kPtp, "PTP"},
                    { pml::ptpmonkey::clck::enumTimeSource::kNtp, "NTP"},
                    { pml::ptpmonkey::clck::enumTimeSource::kHandset, "Handset"},
                    { pml::ptpmonkey::clck::enumTimeSource::kOther, "Other"},
                    { pml::ptpmonkey::clck::enumTimeSource::kInternal, "Internal"}};
const std::map<std::int8_t, wxString> kRate{{static_cast<std::int8_t>(Rate::kPerSec128), "128/s"},
             {static_cast<std::int8_t>(Rate::kPerSec64), "64/s"},
             {static_cast<std::int8_t>(Rate::kPerSec32), "32/s"},
             {static_cast<std::int8_t>(Rate::kPerSec16), "16/s"},
             {static_cast<std::int8_t>(Rate::kPerSec8), "8/s"},
             {static_cast<std::int8_t>(Rate::kPerSec4), "4/s"},
             {static_cast<std::int8_t>(Rate::kPerSec2), "2/s"},
             {static_cast<std::int8_t>(Rate::kEvery1Sec), "1/s"},
             {static_cast<std::int8_t>(Rate::kEvery2Sec), "1/2s"},
             {static_cast<std::int8_t>(Rate::kEvery4Sec), "1/4s"},
             {static_cast<std::int8_t>(Rate::kEvery8Sec), "1/8s"},
             {static_cast<std::int8_t>(Rate::kEvery16Sec), "1/16s"}};

ptpPanel::ptpPanel(wxWindow* parent, ptpBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder),
    m_nDomain(Settings::Get().Read("Time", "PTP_Domain", 0)),
    m_pClock(nullptr),
    m_pLocalClock(nullptr),
    m_bRunning(false)
{
	//(*Initialize(ptpPanel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxSize(800,480), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pSwpMain = new wmSwitcherPanel(this, ID_M_PSWP2, wxDefaultPosition, wxSize(800,480), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
	m_pSwpMain->SetPageNameStyle(0);
	m_ppnlInfo = new wxPanel(m_pSwpMain, ID_PANEL14, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL14"));
	m_ppnlInfo->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl10 = new wmLabel(m_ppnlInfo, ID_M_PLBL80, _("Clocks"), wxPoint(605,5), wxSize(190,25), 0, _T("ID_M_PLBL80"));
	m_pLbl10->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl10->GetUiRect().SetGradient(wxWEST);
	m_pLbl10->SetForegroundColour(wxColour(255,255,255));
	m_pLbl10->SetBackgroundColour(wxColour(128,128,192));
	wxFont m_pLbl10Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_pLbl10->SetFont(m_pLbl10Font);
	m_plstClocks = new wmList(m_ppnlInfo, ID_M_PLST1, wxPoint(605,35), wxSize(190,440), wmList::STYLE_SELECT, 1, wxSize(-1,-1), 1, wxSize(-1,-1));
	m_plstClocks->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	m_plstClocks->SetButtonColour(wxColour(wxT("#000000")));
	m_plstClocks->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_plblDomain = new wmLabel(m_ppnlInfo, ID_M_PLBL79, _("Domain:"), wxPoint(5,5), wxSize(280,25), 0, _T("ID_M_PLBL79"));
	m_plblDomain->SetBorderState(uiRect::BORDER_NONE);
	m_plblDomain->GetUiRect().SetGradient(wxWEST);
	m_plblDomain->SetForegroundColour(wxColour(255,255,255));
	m_plblDomain->SetBackgroundColour(wxColour(128,128,192));
	wxFont m_plblDomainFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_plblDomain->SetFont(m_plblDomainFont);
	m_ptitleIdentity = new wmLabel(m_ppnlInfo, ID_M_PLBL1, _("Port Identitiy"), wxPoint(5,33), wxSize(100,25), 0, _T("ID_M_PLBL1"));
	m_ptitleIdentity->SetBorderState(uiRect::BORDER_FLAT);
	m_ptitleIdentity->GetUiRect().SetGradient(wxWEST);
	m_ptitleIdentity->SetForegroundColour(wxColour(255,255,255));
	m_ptitleIdentity->SetBackgroundColour(wxColour(92,122,224));
	m_ptitleAddress = new wmLabel(m_ppnlInfo, ID_M_PLBL2, _("Address"), wxPoint(5,61), wxSize(100,25), 0, _T("ID_M_PLBL2"));
	m_ptitleAddress->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleAddress->GetUiRect().SetGradient(wxWEST);
	m_ptitleAddress->SetForegroundColour(wxColour(255,255,255));
	m_ptitleAddress->SetBackgroundColour(wxColour(92,122,224));
	m_ptitleState = new wmLabel(m_ppnlInfo, ID_M_PLBL3, _("State"), wxPoint(5,89), wxSize(100,25), 0, _T("ID_M_PLBL3"));
	m_ptitleState->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleState->GetUiRect().SetGradient(wxWEST);
	m_ptitleState->SetForegroundColour(wxColour(255,255,255));
	m_ptitleState->SetBackgroundColour(wxColour(92,122,224));
	m_plblIdentity = new wmLabel(m_ppnlInfo, ID_M_PLBL12, wxEmptyString, wxPoint(105,33), wxSize(180,25), 0, _T("ID_M_PLBL12"));
	m_plblIdentity->SetBorderState(uiRect::BORDER_DOWN);
	m_plblIdentity->GetUiRect().SetGradient(0);
	m_plblIdentity->SetForegroundColour(wxColour(0,0,0));
	m_plblIdentity->SetBackgroundColour(wxColour(255,255,255));
	m_pblAddress = new wmLabel(m_ppnlInfo, ID_M_PLBL13, wxEmptyString, wxPoint(105,61), wxSize(180,25), 0, _T("ID_M_PLBL13"));
	m_pblAddress->SetBorderState(uiRect::BORDER_DOWN);
	m_pblAddress->GetUiRect().SetGradient(0);
	m_pblAddress->SetForegroundColour(wxColour(0,0,0));
	m_pblAddress->SetBackgroundColour(wxColour(255,255,255));
	m_plblState = new wmLabel(m_ppnlInfo, ID_M_PLBL14, wxEmptyString, wxPoint(105,89), wxSize(180,25), 0, _T("ID_M_PLBL14"));
	m_plblState->SetBorderState(uiRect::BORDER_DOWN);
	m_plblState->GetUiRect().SetGradient(0);
	m_plblState->SetForegroundColour(wxColour(0,0,0));
	m_plblState->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlAnnouncements = new wxPanel(m_ppnlInfo, ID_PANEL1, wxPoint(5,135), wxSize(280,275), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_ppnlAnnouncements->SetBackgroundColour(wxColour(0,0,0));
	m_ptitleAnnouncements = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL30, _("Announcements"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL30"));
	m_ptitleAnnouncements->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleAnnouncements->GetUiRect().SetGradient(wxWEST);
	m_ptitleAnnouncements->SetForegroundColour(wxColour(255,255,255));
	m_ptitleAnnouncements->SetBackgroundColour(wxColour(98,219,101));
	wxFont m_ptitleAnnouncementsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitleAnnouncements->SetFont(m_ptitleAnnouncementsFont);
	m_ptitleAnnRate = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL24, _("Rate"), wxPoint(0,28), wxSize(70,25), 0, _T("ID_M_PLBL24"));
	m_ptitleAnnRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleAnnRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleAnnRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleAnnRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblAnnRate = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL25, wxEmptyString, wxPoint(70,28), wxSize(70,25), 0, _T("ID_M_PLBL25"));
	m_plblAnnRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblAnnRate->GetUiRect().SetGradient(0);
	m_plblAnnRate->SetForegroundColour(wxColour(0,0,0));
	m_plblAnnRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleCount = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL26, _("Count"), wxPoint(140,28), wxSize(70,25), 0, _T("ID_M_PLBL26"));
	m_ptitleCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblAnnCount = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL27, wxEmptyString, wxPoint(210,28), wxSize(70,25), 0, _T("ID_M_PLBL27"));
	m_plblAnnCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblAnnCount->GetUiRect().SetGradient(0);
	m_plblAnnCount->SetForegroundColour(wxColour(0,0,0));
	m_plblAnnCount->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleUtc = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL4, _("UTC Offset"), wxPoint(140,84), wxSize(70,25), 0, _T("ID_M_PLBL4"));
	m_ptitleUtc->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleUtc->GetUiRect().SetGradient(wxWEST);
	m_ptitleUtc->SetForegroundColour(wxColour(255,255,255));
	m_ptitleUtc->SetBackgroundColour(wxColour(92,122,224));
	m_plblUTC = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL15, wxEmptyString, wxPoint(210,84), wxSize(70,25), 0, _T("ID_M_PLBL15"));
	m_plblUTC->SetBorderState(uiRect::BORDER_DOWN);
	m_plblUTC->GetUiRect().SetGradient(0);
	m_plblUTC->SetForegroundColour(wxColour(0,0,0));
	m_plblUTC->SetBackgroundColour(wxColour(255,255,255));
	m_ptitlePriority1 = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL5, _("Priority 1"), wxPoint(0,56), wxSize(70,25), 0, _T("ID_M_PLBL5"));
	m_ptitlePriority1->SetBorderState(uiRect::BORDER_NONE);
	m_ptitlePriority1->GetUiRect().SetGradient(wxWEST);
	m_ptitlePriority1->SetForegroundColour(wxColour(255,255,255));
	m_ptitlePriority1->SetBackgroundColour(wxColour(92,122,224));
	m_plblPriority1 = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL16, wxEmptyString, wxPoint(70,56), wxSize(70,25), 0, _T("ID_M_PLBL16"));
	m_plblPriority1->SetBorderState(uiRect::BORDER_DOWN);
	m_plblPriority1->GetUiRect().SetGradient(0);
	m_plblPriority1->SetForegroundColour(wxColour(0,0,0));
	m_plblPriority1->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleClass = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL7, _("Class"), wxPoint(140,56), wxSize(70,25), 0, _T("ID_M_PLBL7"));
	m_ptitleClass->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleClass->GetUiRect().SetGradient(wxWEST);
	m_ptitleClass->SetForegroundColour(wxColour(255,255,255));
	m_ptitleClass->SetBackgroundColour(wxColour(92,122,224));
	m_plblClass = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL17, wxEmptyString, wxPoint(210,56), wxSize(70,25), 0, _T("ID_M_PLBL17"));
	m_plblClass->SetBorderState(uiRect::BORDER_DOWN);
	m_plblClass->GetUiRect().SetGradient(0);
	m_plblClass->SetForegroundColour(wxColour(0,0,0));
	m_plblClass->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleAccuracy = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL8, _("Accuracy"), wxPoint(0,84), wxSize(70,25), 0, _T("ID_M_PLBL8"));
	m_ptitleAccuracy->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleAccuracy->GetUiRect().SetGradient(wxWEST);
	m_ptitleAccuracy->SetForegroundColour(wxColour(255,255,255));
	m_ptitleAccuracy->SetBackgroundColour(wxColour(92,122,224));
	m_plblAccuracy = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL18, _("Unknown"), wxPoint(70,84), wxSize(70,25), 0, _T("ID_M_PLBL18"));
	m_plblAccuracy->SetBorderState(uiRect::BORDER_DOWN);
	m_plblAccuracy->GetUiRect().SetGradient(0);
	m_plblAccuracy->SetForegroundColour(wxColour(0,0,0));
	m_plblAccuracy->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleVariance = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL9, _("Variance"), wxPoint(0,112), wxSize(70,25), 0, _T("ID_M_PLBL9"));
	m_ptitleVariance->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleVariance->GetUiRect().SetGradient(wxWEST);
	m_ptitleVariance->SetForegroundColour(wxColour(255,255,255));
	m_ptitleVariance->SetBackgroundColour(wxColour(92,122,224));
	m_plblVariance = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL19, wxEmptyString, wxPoint(70,112), wxSize(70,25), 0, _T("ID_M_PLBL19"));
	m_plblVariance->SetBorderState(uiRect::BORDER_DOWN);
	m_plblVariance->GetUiRect().SetGradient(0);
	m_plblVariance->SetForegroundColour(wxColour(0,0,0));
	m_plblVariance->SetBackgroundColour(wxColour(255,255,255));
	m_ptitlePriority2 = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL6, _("Priority 2"), wxPoint(140,112), wxSize(70,25), 0, _T("ID_M_PLBL6"));
	m_ptitlePriority2->SetBorderState(uiRect::BORDER_NONE);
	m_ptitlePriority2->GetUiRect().SetGradient(wxWEST);
	m_ptitlePriority2->SetForegroundColour(wxColour(255,255,255));
	m_ptitlePriority2->SetBackgroundColour(wxColour(92,122,224));
	m_plblPriority2 = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL20, wxEmptyString, wxPoint(210,112), wxSize(70,25), 0, _T("ID_M_PLBL20"));
	m_plblPriority2->SetBorderState(uiRect::BORDER_DOWN);
	m_plblPriority2->GetUiRect().SetGradient(0);
	m_plblPriority2->SetForegroundColour(wxColour(0,0,0));
	m_plblPriority2->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleSteps = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL11, _("Steps"), wxPoint(0,140), wxSize(70,25), 0, _T("ID_M_PLBL11"));
	m_ptitleSteps->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleSteps->GetUiRect().SetGradient(wxWEST);
	m_ptitleSteps->SetForegroundColour(wxColour(255,255,255));
	m_ptitleSteps->SetBackgroundColour(wxColour(92,122,224));
	m_plblSteps = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL21, wxEmptyString, wxPoint(70,140), wxSize(70,25), 0, _T("ID_M_PLBL21"));
	m_plblSteps->SetBorderState(uiRect::BORDER_DOWN);
	m_plblSteps->GetUiRect().SetGradient(0);
	m_plblSteps->SetForegroundColour(wxColour(0,0,0));
	m_plblSteps->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleSource = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL10, _("Source"), wxPoint(140,140), wxSize(70,25), 0, _T("ID_M_PLBL10"));
	m_ptitleSource->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleSource->GetUiRect().SetGradient(wxWEST);
	m_ptitleSource->SetForegroundColour(wxColour(255,255,255));
	m_ptitleSource->SetBackgroundColour(wxColour(92,122,224));
	m_plblTitleMaster = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL35, _("Master"), wxPoint(0,168), wxSize(70,25), 0, _T("ID_M_PLBL35"));
	m_plblTitleMaster->SetBorderState(uiRect::BORDER_NONE);
	m_plblTitleMaster->GetUiRect().SetGradient(wxWEST);
	m_plblTitleMaster->SetForegroundColour(wxColour(255,255,255));
	m_plblTitleMaster->SetBackgroundColour(wxColour(92,122,224));
	m_plblMasterId = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL39, wxEmptyString, wxPoint(70,168), wxSize(210,25), 0, _T("ID_M_PLBL39"));
	m_plblMasterId->SetBorderState(uiRect::BORDER_DOWN);
	m_plblMasterId->GetUiRect().SetGradient(0);
	m_plblMasterId->SetForegroundColour(wxColour(0,0,0));
	m_plblMasterId->SetBackgroundColour(wxColour(255,255,255));
	m_plblSource = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL22, wxEmptyString, wxPoint(210,140), wxSize(70,25), 0, _T("ID_M_PLBL22"));
	m_plblSource->SetBorderState(uiRect::BORDER_DOWN);
	m_plblSource->GetUiRect().SetGradient(0);
	m_plblSource->SetForegroundColour(wxColour(0,0,0));
	m_plblSource->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlAnnounceFlags = new pnlFlags(m_ppnlAnnouncements, ID_PANEL9, wxPoint(0,196), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL9"));
	m_pswp = new wmSwitcherPanel(m_ppnlInfo, ID_M_PSWP1, wxPoint(295,5), wxSize(280,280), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswp->SetPageNameStyle(0);
	m_pswp->SetBackgroundColour(wxColour(0,0,0));
	m_ppnlMaster = new wxPanel(m_pswp, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
	m_ppnlMaster->SetBackgroundColour(wxColour(0,0,0));
	m_ppnlSync = new wxPanel(m_ppnlMaster, ID_PANEL2, wxPoint(0,0), wxSize(280,125), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	m_ppnlSync->SetBackgroundColour(wxColour(0,0,0));
	m_ptitleSync = new wmLabel(m_ppnlSync, ID_M_PLBL23, _("Sync"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL23"));
	m_ptitleSync->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleSync->GetUiRect().SetGradient(wxWEST);
	m_ptitleSync->SetForegroundColour(wxColour(255,255,255));
	m_ptitleSync->SetBackgroundColour(wxColour(98,219,101));
	wxFont m_ptitleSyncFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitleSync->SetFont(m_ptitleSyncFont);
	m_ptitleSyncRate = new wmLabel(m_ppnlSync, ID_M_PLBL31, _("Rate"), wxPoint(0,28), wxSize(50,25), 0, _T("ID_M_PLBL31"));
	m_ptitleSyncRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleSyncRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleSyncRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleSyncRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblSyncRate = new wmLabel(m_ppnlSync, ID_M_PLBL32, wxEmptyString, wxPoint(50,28), wxSize(50,25), 0, _T("ID_M_PLBL32"));
	m_plblSyncRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblSyncRate->GetUiRect().SetGradient(0);
	m_plblSyncRate->SetForegroundColour(wxColour(0,0,0));
	m_plblSyncRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleSyncCount = new wmLabel(m_ppnlSync, ID_M_PLBL33, _("Count"), wxPoint(100,28), wxSize(50,25), 0, _T("ID_M_PLBL33"));
	m_ptitleSyncCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleSyncCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleSyncCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleSyncCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblSyncCount = new wmLabel(m_ppnlSync, ID_M_PLBL34, wxEmptyString, wxPoint(150,28), wxSize(130,25), 0, _T("ID_M_PLBL34"));
	m_plblSyncCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblSyncCount->GetUiRect().SetGradient(0);
	m_plblSyncCount->SetForegroundColour(wxColour(0,0,0));
	m_plblSyncCount->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlSyncFlags = new pnlFlags(m_ppnlSync, ID_PANEL10, wxPoint(0,56), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL10"));
	m_ppnlFollowUp = new wxPanel(m_ppnlMaster, ID_PANEL3, wxPoint(0,130), wxSize(280,125), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	m_ppnlFollowUp->SetBackgroundColour(wxColour(0,0,0));
	m_ptitleFollowUp = new wmLabel(m_ppnlFollowUp, ID_M_PLBL28, _("Follow Up"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL28"));
	m_ptitleFollowUp->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleFollowUp->GetUiRect().SetGradient(wxWEST);
	m_ptitleFollowUp->SetForegroundColour(wxColour(255,255,255));
	m_ptitleFollowUp->SetBackgroundColour(wxColour(98,219,101));
	wxFont m_ptitleFollowUpFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitleFollowUp->SetFont(m_ptitleFollowUpFont);
	m_ptitleFollowRate = new wmLabel(m_ppnlFollowUp, ID_M_PLBL29, _("Rate"), wxPoint(0,28), wxSize(50,25), 0, _T("ID_M_PLBL29"));
	m_ptitleFollowRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleFollowRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleFollowRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleFollowRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblFollowRate = new wmLabel(m_ppnlFollowUp, ID_M_PLBL36, wxEmptyString, wxPoint(50,28), wxSize(50,25), 0, _T("ID_M_PLBL36"));
	m_plblFollowRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblFollowRate->GetUiRect().SetGradient(0);
	m_plblFollowRate->SetForegroundColour(wxColour(0,0,0));
	m_plblFollowRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleFollowCount = new wmLabel(m_ppnlFollowUp, ID_M_PLBL37, _("Count"), wxPoint(100,28), wxSize(50,25), 0, _T("ID_M_PLBL37"));
	m_ptitleFollowCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleFollowCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleFollowCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleFollowCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblFollowCount = new wmLabel(m_ppnlFollowUp, ID_M_PLBL38, wxEmptyString, wxPoint(150,28), wxSize(130,25), 0, _T("ID_M_PLBL38"));
	m_plblFollowCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblFollowCount->GetUiRect().SetGradient(0);
	m_plblFollowCount->SetForegroundColour(wxColour(0,0,0));
	m_plblFollowCount->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlFollowFlags = new pnlFlags(m_ppnlFollowUp, ID_PANEL11, wxPoint(0,56), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL11"));
	m_ppnlSlave = new wxPanel(m_pswp, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
	m_ppnlSlave->SetBackgroundColour(wxColour(0,0,0));
	m_ppnlRequests = new wxPanel(m_ppnlSlave, ID_PANEL4, wxPoint(0,0), wxSize(280,165), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	m_ppnlRequests->SetBackgroundColour(wxColour(0,0,0));
	m_ptitleDelay = new wmLabel(m_ppnlRequests, ID_M_PLBL40, _("Delay Requests"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL40"));
	m_ptitleDelay->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelay->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelay->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelay->SetBackgroundColour(wxColour(98,219,101));
	wxFont m_ptitleDelayFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitleDelay->SetFont(m_ptitleDelayFont);
	m_ptitleDelayRate = new wmLabel(m_ppnlRequests, ID_M_PLBL41, _("Rate"), wxPoint(0,28), wxSize(70,25), 0, _T("ID_M_PLBL41"));
	m_ptitleDelayRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayRate = new wmLabel(m_ppnlRequests, ID_M_PLBL42, wxEmptyString, wxPoint(70,28), wxSize(70,25), 0, _T("ID_M_PLBL42"));
	m_plblDelayRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayRate->GetUiRect().SetGradient(0);
	m_plblDelayRate->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleDelayCount = new wmLabel(m_ppnlRequests, ID_M_PLBL43, _("Count"), wxPoint(140,28), wxSize(70,25), 0, _T("ID_M_PLBL43"));
	m_ptitleDelayCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayCount = new wmLabel(m_ppnlRequests, ID_M_PLBL44, wxEmptyString, wxPoint(210,28), wxSize(70,25), 0, _T("ID_M_PLBL44"));
	m_plblDelayCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayCount->GetUiRect().SetGradient(0);
	m_plblDelayCount->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayCount->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleDelayMech = new wmLabel(m_ppnlRequests, ID_M_PLBL46, _("Mechanism"), wxPoint(0,56), wxSize(70,25), 0, _T("ID_M_PLBL46"));
	m_ptitleDelayMech->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayMech->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayMech->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayMech->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayMech = new wmLabel(m_ppnlRequests, ID_M_PLBL47, wxEmptyString, wxPoint(70,56), wxSize(210,25), 0, _T("ID_M_PLBL47"));
	m_plblDelayMech->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayMech->GetUiRect().SetGradient(0);
	m_plblDelayMech->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayMech->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlRequestFlags = new pnlFlags(m_ppnlRequests, ID_PANEL13, wxPoint(0,84), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL13"));
	m_ppnlResponses = new wxPanel(m_ppnlSlave, ID_PANEL7, wxPoint(0,152), wxSize(280,124), wxTAB_TRAVERSAL, _T("ID_PANEL7"));
	m_ppnlResponses->SetBackgroundColour(wxColour(0,0,0));
	m_ptitleDelayResponses = new wmLabel(m_ppnlResponses, ID_M_PLBL48, _("Delay Responses Received"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL48"));
	m_ptitleDelayResponses->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayResponses->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayResponses->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayResponses->SetBackgroundColour(wxColour(98,219,101));
	wxFont m_ptitleDelayResponsesFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitleDelayResponses->SetFont(m_ptitleDelayResponsesFont);
	m_ptitleResponseRate = new wmLabel(m_ppnlResponses, ID_M_PLBL49, _("Rate"), wxPoint(0,28), wxSize(70,25), 0, _T("ID_M_PLBL49"));
	m_ptitleResponseRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleResponseRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleResponseRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleResponseRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblResponseRate = new wmLabel(m_ppnlResponses, ID_M_PLBL50, wxEmptyString, wxPoint(70,28), wxSize(70,25), 0, _T("ID_M_PLBL50"));
	m_plblResponseRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblResponseRate->GetUiRect().SetGradient(0);
	m_plblResponseRate->SetForegroundColour(wxColour(0,0,0));
	m_plblResponseRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleResponseCount = new wmLabel(m_ppnlResponses, ID_M_PLBL51, _("Count"), wxPoint(140,28), wxSize(70,25), 0, _T("ID_M_PLBL51"));
	m_ptitleResponseCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleResponseCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleResponseCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleResponseCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblResponseCount = new wmLabel(m_ppnlResponses, ID_M_PLBL52, wxEmptyString, wxPoint(210,28), wxSize(70,25), 0, _T("ID_M_PLBL52"));
	m_plblResponseCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblResponseCount->GetUiRect().SetGradient(0);
	m_plblResponseCount->SetForegroundColour(wxColour(0,0,0));
	m_plblResponseCount->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlResponseFlags = new pnlFlags(m_ppnlResponses, ID_PANEL12, wxPoint(0,56), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL12"));
	m_pswp->AddPage(m_ppnlMaster, _("Master"), false);
	m_pswp->AddPage(m_ppnlSlave, _("Slave"), false);
	m_ppnlLocal = new wxPanel(m_ppnlInfo, ID_PANEL8, wxPoint(295,280), wxSize(280,205), wxTAB_TRAVERSAL, _T("ID_PANEL8"));
	m_ppnlLocal->SetBackgroundColour(wxColour(0,0,0));
	m_ptitlePam = new wmLabel(m_ppnlLocal, ID_M_PLBL54, _("PAM"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL54"));
	m_ptitlePam->SetBorderState(uiRect::BORDER_NONE);
	m_ptitlePam->GetUiRect().SetGradient(wxWEST);
	m_ptitlePam->SetForegroundColour(wxColour(255,255,255));
	m_ptitlePam->SetBackgroundColour(wxColour(217,138,100));
	wxFont m_ptitlePamFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitlePam->SetFont(m_ptitlePamFont);
	m_ptitleTime = new wmLabel(m_ppnlLocal, ID_M_PLBL55, _("Time"), wxPoint(0,27), wxSize(50,25), 0, _T("ID_M_PLBL55"));
	m_ptitleTime->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleTime->GetUiRect().SetGradient(wxWEST);
	m_ptitleTime->SetForegroundColour(wxColour(255,255,255));
	m_ptitleTime->SetBackgroundColour(wxColour(92,122,224));
	m_plblTime = new wmLabel(m_ppnlLocal, ID_M_PLBL56, wxEmptyString, wxPoint(50,27), wxSize(230,25), 0, _T("ID_M_PLBL56"));
	m_plblTime->SetBorderState(uiRect::BORDER_DOWN);
	m_plblTime->GetUiRect().SetGradient(0);
	m_plblTime->SetForegroundColour(wxColour(0,0,0));
	m_plblTime->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleOffsetAverage = new wmLabel(m_ppnlLocal, ID_M_PLBL57, _("Offset: Average"), wxPoint(0,56), wxSize(100,25), 0, _T("ID_M_PLBL57"));
	m_ptitleOffsetAverage->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleOffsetAverage->GetUiRect().SetGradient(wxWEST);
	m_ptitleOffsetAverage->SetForegroundColour(wxColour(255,255,255));
	m_ptitleOffsetAverage->SetBackgroundColour(wxColour(92,122,224));
	m_plblOffsetAverage = new wmLabel(m_ppnlLocal, ID_M_PLBL61, wxEmptyString, wxPoint(100,56), wxSize(180,25), 0, _T("ID_M_PLBL61"));
	m_plblOffsetAverage->SetBorderState(uiRect::BORDER_DOWN);
	m_plblOffsetAverage->GetUiRect().SetGradient(0);
	m_plblOffsetAverage->SetForegroundColour(wxColour(0,0,0));
	m_plblOffsetAverage->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleOffsetRange = new wmLabel(m_ppnlLocal, ID_M_PLBL58, _("Offset: Range"), wxPoint(0,84), wxSize(100,25), 0, _T("ID_M_PLBL58"));
	m_ptitleOffsetRange->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleOffsetRange->GetUiRect().SetGradient(wxWEST);
	m_ptitleOffsetRange->SetForegroundColour(wxColour(255,255,255));
	m_ptitleOffsetRange->SetBackgroundColour(wxColour(92,122,224));
	m_plblOffsetRange = new wmLabel(m_ppnlLocal, ID_M_PLBL62, wxEmptyString, wxPoint(100,84), wxSize(180,25), 0, _T("ID_M_PLBL62"));
	m_plblOffsetRange->SetBorderState(uiRect::BORDER_DOWN);
	m_plblOffsetRange->GetUiRect().SetGradient(0);
	m_plblOffsetRange->SetForegroundColour(wxColour(0,0,0));
	m_plblOffsetRange->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleDelayAverage = new wmLabel(m_ppnlLocal, ID_M_PLBL59, _("Delay: Average"), wxPoint(0,112), wxSize(100,25), 0, _T("ID_M_PLBL59"));
	m_ptitleDelayAverage->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayAverage->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayAverage->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayAverage->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayAverage = new wmLabel(m_ppnlLocal, ID_M_PLBL63, wxEmptyString, wxPoint(100,112), wxSize(180,25), 0, _T("ID_M_PLBL63"));
	m_plblDelayAverage->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayAverage->GetUiRect().SetGradient(0);
	m_plblDelayAverage->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayAverage->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleDelayRange = new wmLabel(m_ppnlLocal, ID_M_PLBL60, _("Delay: Range"), wxPoint(0,140), wxSize(100,25), 0, _T("ID_M_PLBL60"));
	m_ptitleDelayRange->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayRange->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayRange->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayRange->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayRange = new wmLabel(m_ppnlLocal, ID_M_PLBL64, wxEmptyString, wxPoint(100,140), wxSize(180,25), 0, _T("ID_M_PLBL64"));
	m_plblDelayRange->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayRange->GetUiRect().SetGradient(0);
	m_plblDelayRange->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayRange->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnGraphs = new wmButton(m_ppnlLocal, ID_M_PBTN29, _("Graphs"), wxPoint(5,168), wxSize(130,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN29"));
	m_pbtnGraphs->SetForegroundColour(wxColour(0,128,0));
	m_pbtnGraphs->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pbtnGraphsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_pbtnGraphs->SetFont(m_pbtnGraphsFont);
	m_pbtnGraphs->SetColourSelected(wxColour(wxT("#800000")));
	m_pbtnHistogram = new wmButton(m_ppnlLocal, ID_M_PBTN5, _("Histogram"), wxPoint(145,168), wxSize(130,30), wmButton::STYLE_NORMAL, wxDefaultValidator, _T("ID_M_PBTN5"));
	m_pbtnHistogram->SetForegroundColour(wxColour(0,128,0));
	m_pbtnHistogram->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pbtnHistogramFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_pbtnHistogram->SetFont(m_pbtnHistogramFont);
	m_pbtnHistogram->SetColourSelected(wxColour(wxT("#800000")));
	m_ppnlGraphs = new wxPanel(m_pSwpMain, ID_PANEL15, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL15"));
	m_ppnlGraphs->SetBackgroundColour(wxColour(0,0,0));
	m_plblGraphTitle = new wmLabel(m_ppnlGraphs, ID_M_PLBL45, wxEmptyString, wxPoint(0,0), wxSize(800,30), 0, _T("ID_M_PLBL45"));
	m_plblGraphTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblGraphTitle->GetUiRect().SetGradient(0);
	m_plblGraphTitle->SetForegroundColour(wxColour(255,255,255));
	m_plblGraphTitle->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_plblGraphTitleFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblGraphTitle->SetFont(m_plblGraphTitleFont);
	m_pLbl5 = new wmLabel(m_ppnlGraphs, ID_M_PLBL69, _("Current:"), wxPoint(0,30), wxSize(50,30), 0, _T("ID_M_PLBL69"));
	m_pLbl5->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl5->GetUiRect().SetGradient(0);
	m_pLbl5->SetForegroundColour(wxColour(255,255,255));
	m_pLbl5->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl5Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl5->SetFont(m_pLbl5Font);
	m_plblCurrent = new wmLabel(m_ppnlGraphs, ID_M_PLBL74, wxEmptyString, wxPoint(50,30), wxSize(80,30), wxALIGN_LEFT, _T("ID_M_PLBL74"));
	m_plblCurrent->SetBorderState(uiRect::BORDER_NONE);
	m_plblCurrent->GetUiRect().SetGradient(0);
	m_plblCurrent->SetForegroundColour(wxColour(186,254,167));
	m_plblCurrent->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblCurrentFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblCurrent->SetFont(m_plblCurrentFont);
	m_pLbl6 = new wmLabel(m_ppnlGraphs, ID_M_PLBL70, _("Mean:"), wxPoint(140,30), wxSize(50,30), 0, _T("ID_M_PLBL70"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->GetUiRect().SetGradient(0);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl6Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl6->SetFont(m_pLbl6Font);
	m_plblMean = new wmLabel(m_ppnlGraphs, ID_M_PLBL75, wxEmptyString, wxPoint(190,30), wxSize(80,30), wxALIGN_LEFT, _T("ID_M_PLBL75"));
	m_plblMean->SetBorderState(uiRect::BORDER_NONE);
	m_plblMean->GetUiRect().SetGradient(0);
	m_plblMean->SetForegroundColour(wxColour(186,254,167));
	m_plblMean->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblMeanFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblMean->SetFont(m_plblMeanFont);
	m_pLbl7 = new wmLabel(m_ppnlGraphs, ID_M_PLBL71, _("SD:"), wxPoint(280,30), wxSize(50,30), 0, _T("ID_M_PLBL71"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl7Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl7->SetFont(m_pLbl7Font);
	m_plblDeviation = new wmLabel(m_ppnlGraphs, ID_M_PLBL76, wxEmptyString, wxPoint(330,30), wxSize(80,30), wxALIGN_LEFT, _T("ID_M_PLBL76"));
	m_plblDeviation->SetBorderState(uiRect::BORDER_NONE);
	m_plblDeviation->GetUiRect().SetGradient(0);
	m_plblDeviation->SetForegroundColour(wxColour(186,254,167));
	m_plblDeviation->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblDeviationFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDeviation->SetFont(m_plblDeviationFont);
	m_pLbl8 = new wmLabel(m_ppnlGraphs, ID_M_PLBL72, _("Slope:"), wxPoint(420,30), wxSize(50,30), 0, _T("ID_M_PLBL72"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->GetUiRect().SetGradient(0);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl8Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl8->SetFont(m_pLbl8Font);
	m_plblSlope = new wmLabel(m_ppnlGraphs, ID_M_PLBL77, wxEmptyString, wxPoint(470,30), wxSize(80,30), wxALIGN_LEFT, _T("ID_M_PLBL77"));
	m_plblSlope->SetBorderState(uiRect::BORDER_NONE);
	m_plblSlope->GetUiRect().SetGradient(0);
	m_plblSlope->SetForegroundColour(wxColour(186,254,167));
	m_plblSlope->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblSlopeFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblSlope->SetFont(m_plblSlopeFont);
	m_pLbl9 = new wmLabel(m_ppnlGraphs, ID_M_PLBL73, _("Est:"), wxPoint(560,30), wxSize(50,30), 0, _T("ID_M_PLBL73"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl9Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl9->SetFont(m_pLbl9Font);
	m_plblPrediction = new wmLabel(m_ppnlGraphs, ID_M_PLBL78, wxEmptyString, wxPoint(610,30), wxSize(80,30), wxALIGN_LEFT, _T("ID_M_PLBL78"));
	m_plblPrediction->SetBorderState(uiRect::BORDER_NONE);
	m_plblPrediction->GetUiRect().SetGradient(0);
	m_plblPrediction->SetForegroundColour(wxColour(186,254,167));
	m_plblPrediction->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblPredictionFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblPrediction->SetFont(m_plblPredictionFont);
	m_pHistoryGraph = new HistoryGraph(m_ppnlGraphs,ID_HISTORY_GRAPH,wxPoint(0,60),wxSize(800,370));
	m_pLbl4 = new wmLabel(m_ppnlGraphs, ID_M_PLBL68, _("Data"), wxPoint(10,445), wxSize(60,30), 0, _T("ID_M_PLBL68"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl4Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl4->SetFont(m_pLbl4Font);
	m_plstGraphData = new wmList(m_ppnlGraphs, ID_M_PLST5, wxPoint(70,443), wxSize(180,34), wmList::STYLE_SELECT, 0, wxSize(-1,30), 3, wxSize(-1,-1));
	m_plstGraphData->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnClearStats = new wmButton(m_ppnlGraphs, ID_M_PBTN8, _("Clear All"), wxPoint(498,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
	m_pbtnGraphClear = new wmButton(m_ppnlGraphs, ID_M_PBTN3, _("Clear All"), wxPoint(570,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnGraphHisto = new wmButton(m_ppnlGraphs, ID_M_PBTN7, _("Histo"), wxPoint(649,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnGraphHisto->SetForegroundColour(wxColour(0,128,0));
	m_pbtnGraphHisto->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pbtnGraphHistoFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_pbtnGraphHisto->SetFont(m_pbtnGraphHistoFont);
	m_pbtnGraphOptions = new wmButton(m_ppnlGraphs, ID_M_PBTN4, _("Info"), wxPoint(720,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnGraphOptions->SetForegroundColour(wxColour(0,128,0));
	m_pbtnGraphOptions->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pbtnGraphOptionsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_pbtnGraphOptions->SetFont(m_pbtnGraphOptionsFont);
	m_ppnlHistograms = new wxPanel(m_pSwpMain, ID_PANEL16, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL16"));
	m_ppnlHistograms->SetBackgroundColour(wxColour(0,0,0));
	m_plblHistogramTitle = new wmLabel(m_ppnlHistograms, ID_M_PLBL53, wxEmptyString, wxPoint(0,0), wxSize(800,30), 0, _T("ID_M_PLBL53"));
	m_plblHistogramTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblHistogramTitle->GetUiRect().SetGradient(0);
	m_plblHistogramTitle->SetForegroundColour(wxColour(255,255,255));
	m_plblHistogramTitle->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_plblHistogramTitleFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_plblHistogramTitle->SetFont(m_plblHistogramTitleFont);
	m_pHistogram = new Histogram(m_ppnlHistograms,ID_CUSTOM1,wxPoint(0,30),wxSize(800,410));
	m_pLbl3 = new wmLabel(m_ppnlHistograms, ID_M_PLBL67, _("Data"), wxPoint(10,445), wxSize(60,30), 0, _T("ID_M_PLBL67"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl3Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_plstHistogramData = new wmList(m_ppnlHistograms, ID_M_PLST4, wxPoint(70,443), wxSize(180,34), wmList::STYLE_SELECT, 0, wxSize(-1,30), 3, wxSize(-1,-1));
	m_plstHistogramData->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(m_ppnlHistograms, ID_M_PLBL65, _("Granularity"), wxPoint(195,445), wxSize(60,30), 0, _T("ID_M_PLBL65"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl1Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_plstHistogramGranularity = new wmList(m_ppnlHistograms, ID_M_PLST2, wxPoint(255,443), wxSize(120,34), 0, 0, wxSize(-1,30), 3, wxSize(-1,-1));
	m_plstHistogramGranularity->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl2 = new wmLabel(m_ppnlHistograms, ID_M_PLBL66, _("Resolution"), wxPoint(380,445), wxSize(60,30), 0, _T("ID_M_PLBL66"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl2Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_plstHistogramResolution = new wmList(m_ppnlHistograms, ID_M_PLST3, wxPoint(440,443), wxSize(120,34), 0, 0, wxSize(-1,30), 3, wxSize(-1,-1));
	m_plstHistogramResolution->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnHistogramClear = new wmButton(m_ppnlHistograms, ID_M_PBTN1, _("Clear All"), wxPoint(570,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnHistGraphs = new wmButton(m_ppnlHistograms, ID_M_PBTN6, _("Graphs"), wxPoint(649,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnHistGraphs->SetForegroundColour(wxColour(0,128,0));
	m_pbtnHistGraphs->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pbtnHistGraphsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_pbtnHistGraphs->SetFont(m_pbtnHistGraphsFont);
	m_pbtnOptions = new wmButton(m_ppnlHistograms, ID_M_PBTN2, _("Info"), wxPoint(720,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pbtnOptions->SetForegroundColour(wxColour(0,128,0));
	m_pbtnOptions->SetBackgroundColour(wxColour(255,255,255));
	wxFont m_pbtnOptionsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_pbtnOptions->SetFont(m_pbtnOptionsFont);
	m_pSwpMain->AddPage(m_ppnlInfo, _("Info"), false);
	m_pSwpMain->AddPage(m_ppnlGraphs, _("Graphs"), false);
	m_pSwpMain->AddPage(m_ppnlHistograms, _("Histograms"), false);

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstClocksSelected);
	Connect(ID_M_PBTN29,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnGraphsClick);
	Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnHistogramClick);
	Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstDataSelected);
	Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnClearStatsClick);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnClearClick);
	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnHistogramClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnOptionsClick);
	Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstDataSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstHistogramGranularitySelected);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstHistogramResolutionSelected);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnClearClick);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnGraphsClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnOptionsClick);
	//*)

	m_ptitleOffsetAverage->SetLabel(L"Offset: x\u0304");
    m_ptitleOffsetRange->SetLabel(L"Offset: \u03c3 \u03c3\u2093\u0304");
    m_ptitleDelayAverage->SetLabel(L"Delay: x\u0304");
    m_ptitleDelayRange->SetLabel(L"Delay: \u03c3 \u03c3\u2093\u0304");
    m_pLbl6->SetLabel(L"y\u0304=");
    m_pLbl7->SetLabel(L"\u03c3=");
    m_pLbl8->SetLabel(L"\u03b2\u0302=");
    m_pLbl9->SetLabel(L"\u03b1\u0302+\u03b2\u0302x=");

	//m_pbtnClearStats = new wmButton(m_ppnlGraphs, wxNewId(), _("Clear Stats"), wxPoint(520,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
    //Connect(m_pbtnClearStats->GetId(),wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnClearStatsClick);

	m_ptitleOffsetAverage->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_ptitleOffsetRange->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_ptitleDelayAverage->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	m_ptitleDelayRange->SetTextAlign(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    m_plstHistogramGranularity->AddButton("-");
    m_plstHistogramGranularity->AddButton("0");
    m_plstHistogramGranularity->AddButton("+");

    m_plstHistogramResolution->AddButton("-");
    m_plstHistogramResolution->AddButton("0");
    m_plstHistogramResolution->AddButton("+");

    m_plstHistogramData->AddButton("Offset");
    m_plstHistogramData->AddButton("Delay");
    m_plstGraphData->AddButton("Offset");
    m_plstGraphData->AddButton("Mean");
	m_plstGraphData->AddButton("Delay");


    m_plstHistogramData->ConnectToSetting(m_pBuilder->GetSection(), "Data", "Offset");
    m_plstGraphData->ConnectToSetting(m_pBuilder->GetSection(), "Data", "Offset");

	m_plstClocks->SetTextSelectedButtonColour(*wxBLACK);

	m_plblCurrent->SetTextAlign(wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	m_plblMean->SetTextAlign(wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	m_plblDeviation->SetTextAlign(wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	m_plblSlope->SetTextAlign(wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);
	m_plblPrediction->SetTextAlign(wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL);

	m_offset = 0.0;

	m_plstClocks->SetBitmapAlign(wxALIGN_TOP | wxALIGN_RIGHT);

	m_timer.SetOwner(this, wxNewId());
	Connect(m_timer.GetId(), wxEVT_TIMER, (wxObjectEventFunction)&ptpPanel::OnTimer);
	m_timer.Start(500, true);

	m_dbMac.LoadXml();

	m_pHistoryGraph->SetFont(wxFont(7,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));
    m_pHistogram->SetFont(wxFont(7,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT));

	m_pHistoryGraph->AddGraph("Offset", wxColour(200,200,255), 2e6, true, false);
	m_pHistoryGraph->AddGraph("OffsetLR", wxColour(100,255,100), 2e6, true, true);
	m_pHistoryGraph->AddGraph("OffsetMean", wxColour(255,255,255), 2e6, true, true);
	m_pHistoryGraph->AddGraph("OffsetVarMin", wxColour(100,100,200), 2e6, true, true);
	m_pHistoryGraph->AddGraph("OffsetVarMax", wxColour(100,100,200), 2e6, true, true);

	m_pHistoryGraph->AddGraph("Mean", wxColour(200,200,255), 2e6, true, false);

	m_pHistoryGraph->AddGraph("Delay", wxColour(200,200,255), 2e6, true, false);
	m_pHistoryGraph->AddGraph("DelayLR", wxColour(100,255,100), 2e6, true, true);
    m_pHistoryGraph->AddGraph("DelayMean", wxColour(255,255,255), 2e6, true, true);
	m_pHistoryGraph->AddGraph("DelayVarMin", wxColour(100,100,200), 2e6, true, true);
	m_pHistoryGraph->AddGraph("DelayVarMax", wxColour(100,100,200), 2e6, true, true);

	m_pHistoryGraph->ShowBarGraph(false);
	m_pHistoryGraph->SetRightAxisWidth(100);

	m_pHistoryGraph->SetGraphUnits("Offset", L"\u00B5s");
	m_pHistoryGraph->SetGraphUnits("Mean", L"\u00B5s");
	m_pHistoryGraph->SetGraphUnits("OffsetLR", L"\u00B5s");
	m_pHistoryGraph->SetGraphUnits("Delay", L"\u00B5s");
	m_pHistoryGraph->SetGraphUnits("DelayLR", L"\u00B5s");

	m_pHistogram->AddGraph("Offset", wxColour(100,255,100), 1);
	m_pHistogram->AddGraph("Delay", wxColour(255,100,100), 1);

    ChangeView(m_pBuilder->ReadSetting("Window", "Info"));

	m_plstGraphData->SelectButton(m_pBuilder->ReadSetting("Graph", "Offset"));

	SetSize(size);
	SetPosition(pos);

    Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&ptpPanel::OnLeftUp);
	m_pHistoryGraph->Bind(wxEVT_LEFT_UP, &ptpPanel::OnLeftUp, this);\
    ConnectLeftUp(this);

    m_plblDomain->SetLabel(wxString::Format("Domain: %u", m_nDomain));

}

void ptpPanel::ConnectLeftUp(wxWindow* pParent)
{
    wxWindowList lst = pParent->GetChildren();
	for(auto pWnd : lst)
    {
        wxPanel* pPanel = dynamic_cast<wxPanel*>(pWnd);
        if(pPanel)
        {
            pPanel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&ptpPanel::OnInfoLeftUp,0,this);
            ConnectLeftUp(pPanel);
        }
        else
        {
            wmLabel* pLabel = dynamic_cast<wmLabel*>(pWnd);
            if(pLabel)
            {
                pLabel->Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&ptpPanel::OnInfoLeftUp,0,this);
            }
        }
    }
}

void ptpPanel::OnInfoLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}


ptpPanel::~ptpPanel()
{
	//(*Destroy(ptpPanel)
	//*)
}

void ptpPanel::SetAudioData(const timedbuffer* pBuffer)
{

}

void ptpPanel::InputSession(const session& aSession)
{

}

void ptpPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void ptpPanel::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}



void ptpPanel::OnlstClocksSelected(wxCommandEvent& event)
{
    m_sSelectedClock = event.GetString();
    m_pClock = wxPtp::Get().GetPtpClock(m_sSelectedClock.BeforeFirst('\n'));
    ShowClockDetails();
}


void ptpPanel::OnClockAdded(wxCommandEvent& event)
{
    AddClock(event.GetString());

    ClockWebsocketMessage(event.GetString(), "Added");

}

void ptpPanel::OnClockUpdated(wxCommandEvent& event)
{
    if(event.GetString() == m_sSelectedClock.BeforeFirst('\n'))
    {
        ShowClockDetails();
    }
    ClockWebsocketMessage(event.GetString(), "Updated");
}

void ptpPanel::OnClockRemoved(wxCommandEvent& event)
{
    if(event.GetString() == m_sSelectedClock.BeforeFirst('\n'))
	{
        ClearClockDetails();
    }
    m_plstClocks->DeleteButton(m_plstClocks->FindButton(m_dbMac.CreateClockText(event.GetString())));

    ClockWebsocketMessage(event.GetString(), "Removed");
}

void ptpPanel::OnClockTime(wxCommandEvent& event)
{
    ShowTime();
    TimeWebsocketMessage();
}

void ptpPanel::OnClockMaster(wxCommandEvent& event)
{
    if(event.GetString() == m_sSelectedClock.BeforeFirst('\n'))
    {
        m_pswp->ChangeSelection("Master");
        m_plblState->SetLabel("Master");
    }

	auto sClock = m_dbMac.CreateClockText(event.GetString());
    size_t nButton = m_plstClocks->FindButton(sClock);
    if(nButton != wmList::NOT_FOUND)
    {
		auto pSyncMaster = wxPtp::Get().GetSyncMasterClock();
		if(pSyncMaster && pSyncMaster->GetId() == sClock)
		{
			m_plstClocks->SetButtonBitmap(nButton, BMP_SYNC);
		}
		else
		{
			m_plstClocks->SetButtonBitmap(nButton, BMP_GRAND);
		}
		m_plstClocks->Update();
    }
    ClockWebsocketMessage(event.GetString(), "Master");
}

void ptpPanel::OnClockSlave(wxCommandEvent& event)
{
    if(event.GetString() == m_sSelectedClock.BeforeFirst('\n'))
    {
        m_pswp->ChangeSelection("Slave");
        m_plblState->SetLabel("Slave");
    }
	auto sClock = m_dbMac.GetVendor(event.GetString())+event.GetString();
    size_t nButton = m_plstClocks->FindButton(sClock);
    if(nButton != wmList::NOT_FOUND)
    {
        m_plstClocks->SetButtonBitmap(nButton, BMP_SLAVE);
    }
    ClockWebsocketMessage(event.GetString(), "Slave");
}


void ptpPanel::ShowClockDetails()
{

    if(!m_pClock) 
	{
		return;
	}

    m_pblAddress->SetLabel(wxString(m_pClock->GetIpAddress()));
    m_plblIdentity->SetLabel(wxString(m_pClock->GetClockId()));

    m_ppnlAnnouncements->Show((m_pClock->GetCount(hdr::enumType::kAnnounce) != 0));

    m_pswp->ChangeSelection(m_pClock->IsSyncMaster() ? "Master" : "Slave");

	size_t nButton = m_plstClocks->FindButton(m_sSelectedClock);
	if(nButton != wmList::NOT_FOUND)
	{
		if(m_pClock->IsGrandMaster())
		{
			m_plblState->SetLabel("Grand Master");
			m_plstClocks->SetButtonBitmap(nButton, BMP_GRAND);
		}
		else if(m_pClock->IsSyncMaster())
		{
			m_plblState->SetLabel("Sync Master");
			m_plstClocks->SetButtonBitmap(nButton, BMP_SYNC);
		}
		else
		{
			m_plblState->SetLabel("Slave");
			m_plstClocks->SetButtonBitmap(nButton, BMP_SLAVE);
		}
    }


    if((m_pClock->GetCount(hdr::enumType::kAnnounce) != 0))
    {
        auto itAccuracy = kAccuracy.find(m_pClock->GetAccuracy());
        if(itAccuracy != kAccuracy.end())
        {
            m_plblAccuracy->SetLabel(itAccuracy->second);
        }
        else
        {
            m_plblAccuracy->SetLabel("Unknown");
        }
        m_plblAnnCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(hdr::enumType::kAnnounce)));
        m_plblAnnRate->SetLabel(ConvertRate(m_pClock->GetInterval(hdr::enumType::kAnnounce)));

        m_plblClass->SetLabel(wxString::Format("%u", static_cast<unsigned int>(m_pClock->GetClass())));

        auto itSource = kTimeSource.find(m_pClock->GetTimeSource());
        if(itSource != kTimeSource.end())
        {
            m_plblSource->SetLabel(itSource->second);
        }
        else
        {
            m_plblSource->SetLabel(wxString::Format("Unknown 0x%X", static_cast<unsigned int>(m_pClock->GetTimeSource())));
        }

        m_plblSteps->SetLabel(wxString::Format("%u", static_cast<unsigned int>(m_pClock->GetStepsRemoved())));
        m_plblUTC->SetLabel(wxString::Format("%u", static_cast<unsigned int>(m_pClock->GetUtcOffset())));
        m_plblVariance->SetLabel(wxString::Format("%u", static_cast<unsigned int>(m_pClock->GetVariance())));
        m_plblPriority1->SetLabel(wxString::Format("%u", static_cast<unsigned int>(m_pClock->GetPriority1())));
        m_plblPriority2->SetLabel(wxString::Format("%u", static_cast<unsigned int>(m_pClock->GetPriority2())));
    }

    if(m_pClock->IsGrandMaster() || m_pClock->IsSyncMaster())
    {
        m_plblSyncCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(hdr::enumType::kSync)));
        m_plblSyncRate->SetLabel(ConvertRate(m_pClock->GetInterval(hdr::enumType::kSync)));

        m_plblFollowCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(hdr::enumType::kFollowUp)));
        m_plblFollowRate->SetLabel(ConvertRate(m_pClock->GetInterval(hdr::enumType::kFollowUp)));
        m_ppnlAnnounceFlags->ShowFlags(m_pClock->GetFlags(hdr::enumType::kAnnounce));
        m_ppnlSyncFlags->ShowFlags(m_pClock->GetFlags(hdr::enumType::kSync));
        m_ppnlFollowFlags->ShowFlags(m_pClock->GetFlags(hdr::enumType::kFollowUp));

        m_plblMasterId->SetLabel(wxPtp::Get().GetMasterClockId());
    }
    else
    {
        m_plblDelayMech->SetLabel("E-E");

        //m_plblDelayMessage->SetLabel
        m_plblDelayCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(hdr::enumType::kDelayReq)));
        m_plblDelayRate->SetLabel(ConvertRate(m_pClock->GetInterval(hdr::enumType::kDelayReq)));


        m_plblResponseCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(hdr::enumType::kDelayResp)));
        m_plblResponseRate->SetLabel(ConvertRate(m_pClock->GetInterval(hdr::enumType::kDelayResp)));

        m_ppnlRequestFlags->ShowFlags(m_pClock->GetFlags(hdr::enumType::kDelayReq));
        m_ppnlResponseFlags->ShowFlags(m_pClock->GetFlags(hdr::enumType::kDelayResp));
    }

}

void ptpPanel::ShowTime()
{
    if(m_pLocalClock)
    {
        auto pSyncMaster = wxPtp::Get().GetSyncMasterClock();

        if(pSyncMaster)
        {
            auto dUTCOffset = static_cast<double>(pSyncMaster->GetUtcOffset());

            m_plblTime->SetLabel(wxString(TimeToIsoString(m_pLocalClock->GetPtpTime())));
            m_plblTime->SetBackgroundColour(TimeManager::Get().IsPtpFrequencyLocked() ? wxColour(150,255,150) : wxColour(255,150,150));

            m_offset  = TimeToDouble(m_pLocalClock->GetOffset(pml::ptpmonkey::PtpV2Clock::MEAN))+dUTCOffset;
            auto dDelay = TimeToDouble(m_pLocalClock->GetDelay(pml::ptpmonkey::PtpV2Clock::MEAN));

            m_plblOffsetAverage->SetLabel(wxString::Format("%f", m_offset));
            m_plblDelayAverage->SetLabel(wxString::Format("%f", dDelay));

            m_plblOffsetRange->SetLabel(wxString::Format("%f, %f", TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SD)), TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SE))));
            m_plblDelayRange->SetLabel(wxString::Format("%f, %f", TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SD)), TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SE))));

            if(m_pLocalClock->IsSynced())
            {

                auto dPeak = (TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::CURRENT))+dUTCOffset)*1e6;
                m_pHistoryGraph->AddPeak("Offset", dPeak);
                m_pHistogram->AddPeak("Offset", dPeak);

				m_pHistoryGraph->AddPeak("Mean", (TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::MEAN))+dUTCOffset)*1e6);

                double m = m_pLocalClock->GetOffsetSlope();
                double c = m_pLocalClock->GetOffsetIntersection();
                auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
                double dEstimate = (dUTCOffset+c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()))*1e6;

                m_pHistoryGraph->SetLine("OffsetLR", (c+dUTCOffset)*1e6, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dEstimate, std::chrono::system_clock::now());

                dPeak = (TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::MEAN))+dUTCOffset)*1e6;

                m_pHistoryGraph->SetLine("OffsetMean", dPeak, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dPeak, std::chrono::system_clock::now());
                dPeak -= TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SD))*1e6;
                m_pHistoryGraph->SetLine("OffsetVarMin", dPeak, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dPeak, std::chrono::system_clock::now());
                dPeak += TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SD))*2e6;
                m_pHistoryGraph->SetLine("OffsetVarMax", dPeak, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dPeak, std::chrono::system_clock::now());

                auto dDelay = TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::CURRENT))*1e6;
                m_pHistoryGraph->AddPeak("Delay", dDelay);
                m_pHistogram->AddPeak("Delay", dDelay);

                m = m_pLocalClock->GetDelaySlope();
                c = m_pLocalClock->GetDelayIntersection();
                now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
                dEstimate = (c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()))*1e6;

                m_pHistoryGraph->SetLine("DelayLR", c*1e6, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dEstimate, std::chrono::system_clock::now());

                dDelay = TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::MEAN))*1e6;
                m_pHistoryGraph->SetLine("DelayMean", dDelay, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dDelay, std::chrono::system_clock::now());
                dDelay -= TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SD))*1e6;
                m_pHistoryGraph->SetLine("DelayVarMin", dDelay, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dDelay, std::chrono::system_clock::now());
                dDelay += TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SD))*2e6;
                m_pHistoryGraph->SetLine("DelayVarMax", dDelay, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dDelay, std::chrono::system_clock::now());

                UpdateGraphLabels();

            }
        }
    }
}

void ptpPanel::UpdateGraphLabels()
{
    auto pSyncMaster = wxPtp::Get().GetSyncMasterClock();
    if(pSyncMaster && m_pLocalClock)
    {

        if(m_sGraph == "Offset" || m_sGraph == "Mean")
        {
            auto dUTCOffset = static_cast<double>(pSyncMaster->GetUtcOffset());
            auto dCurrent = (TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::CURRENT))+dUTCOffset)*1e6;
            auto dMean = (TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::MEAN))+dUTCOffset)*1e6;
            auto dSD = TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SD))*1e6;
            auto m = m_pLocalClock->GetOffsetSlope();
            auto c = m_pLocalClock->GetOffsetIntersection();
            auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            auto dEstimate = (c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()));
            dEstimate = (dEstimate+dUTCOffset)*1e6;


            m_plblCurrent->SetLabel(wxString::Format(L"%.2f\u00B5s", dCurrent));
            m_plblMean->SetLabel(wxString::Format(L"%.2f\u00B5s", dMean));
            m_plblDeviation->SetLabel(wxString::Format(L"%.2f\u00B5s", dSD));
            m_plblSlope->SetLabel(wxString::Format(L"%.2f\u00B5s/s", m*1e6));
            m_plblPrediction->SetLabel(wxString::Format(L"%.2f\u00B5s", dEstimate));
        }
        else if(m_sGraph == "Delay")
        {
            auto dCurrent = (TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::CURRENT)))*1e6;
            auto dMean = (TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::MEAN)))*1e6;
            auto dSD = TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SD))*1e6;
            auto m = m_pLocalClock->GetDelaySlope();
            auto c = m_pLocalClock->GetDelayIntersection();
            auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            auto dEstimate = (c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()))*1e6;

            m_plblCurrent->SetLabel(wxString::Format(L"%.2f\u00B5s", dCurrent));
            m_plblMean->SetLabel(wxString::Format(L"%.2f\u00B5s", dMean));
            m_plblDeviation->SetLabel(wxString::Format(L"%.2f\u00B5s", dSD));
            m_plblSlope->SetLabel(wxString::Format(L"%.2f\u00B5s/s", m*1e6));
            m_plblPrediction->SetLabel(wxString::Format(L"%.2f\u00B5s", dEstimate));
        }
    }
}

void ptpPanel::ClearClockDetails()
{
    m_pblAddress->SetLabel(wxEmptyString);
    m_plblAccuracy->SetLabel(wxEmptyString);
    m_plblAnnCount->SetLabel(wxEmptyString);
    m_plblAnnRate->SetLabel(wxEmptyString);
    m_plblClass->SetLabel(wxEmptyString);
    m_plblDelayAverage->SetLabel(wxEmptyString);
    m_plblDelayCount->SetLabel(wxEmptyString);
    m_plblDelayMech->SetLabel(wxEmptyString);

    m_plblDelayRange->SetLabel(wxEmptyString);
    m_plblDelayRate->SetLabel(wxEmptyString);
    m_plblFollowCount->SetLabel(wxEmptyString);

    m_plblFollowRate->SetLabel(wxEmptyString);
    m_plblIdentity->SetLabel(wxEmptyString);
    m_plblOffsetAverage->SetLabel(wxEmptyString);
    m_plblOffsetRange->SetLabel(wxEmptyString);
    m_plblPriority1->SetLabel(wxEmptyString);
    m_plblPriority2->SetLabel(wxEmptyString);
    m_plblResponseCount->SetLabel(wxEmptyString);

    m_plblResponseRate->SetLabel(wxEmptyString);
    m_plblSource->SetLabel(wxEmptyString);
    m_plblState->SetLabel(wxEmptyString);
    m_plblSteps->SetLabel(wxEmptyString);
    m_plblSyncCount->SetLabel(wxEmptyString);

    m_plblSyncRate->SetLabel(wxEmptyString);
    m_plblTime->SetLabel(wxEmptyString);
    m_plblUTC->SetLabel(wxEmptyString);
    m_plblVariance->SetLabel(wxEmptyString);

}

wxString ptpPanel::ConvertRate(unsigned char nRate)
{
    auto itRate = kRate.find(nRate);
    if(itRate != kRate.end())
    {
        return itRate->second;
    }
    else
    {
        return wxString::Format("0x%X", nRate);
    }
}

void ptpPanel::OnTimer(wxTimerEvent& event)
{
    if(m_bRunning == false)
    {
        m_bRunning = true;
        wxPtp::Get().AddHandler(this);

        Connect(wxID_ANY, wxEVT_CLOCK_ADDED,(wxObjectEventFunction)&ptpPanel::OnClockAdded);
        Connect(wxID_ANY, wxEVT_CLOCK_UPDATED,(wxObjectEventFunction)&ptpPanel::OnClockUpdated);
        Connect(wxID_ANY, wxEVT_CLOCK_REMOVED,(wxObjectEventFunction)&ptpPanel::OnClockRemoved);
        Connect(wxID_ANY, wxEVT_CLOCK_TIME,(wxObjectEventFunction)&ptpPanel::OnClockTime);
        Connect(wxID_ANY, wxEVT_CLOCK_MASTER,(wxObjectEventFunction)&ptpPanel::OnClockMaster);
        Connect(wxID_ANY, wxEVT_CLOCK_SLAVE,(wxObjectEventFunction)&ptpPanel::OnClockSlave);
        Connect(wxID_ANY, wxEVT_CLOCK_MSG_ANNOUNCE,(wxObjectEventFunction)&ptpPanel::OnClockMessage);
        Connect(wxID_ANY, wxEVT_CLOCK_MSG_FOLLOWUP,(wxObjectEventFunction)&ptpPanel::OnClockMessage);
        Connect(wxID_ANY, wxEVT_CLOCK_MSG_SYNC,(wxObjectEventFunction)&ptpPanel::OnClockMessage);
        Connect(wxID_ANY, wxEVT_CLOCK_MSG_DELAY_REQUEST,(wxObjectEventFunction)&ptpPanel::OnClockMessage);
        Connect(wxID_ANY, wxEVT_CLOCK_MSG_DELAY_RESPONSE,(wxObjectEventFunction)&ptpPanel::OnClockMessage);

        wxPtp::Get().Run(Settings::Get().Read("AoIP_Settings", "Interface", "eth0"), m_nDomain, Settings::Get().Read("Time", "Ptp_Mode", 0) ? pml::ptpmonkey::Mode::kHybrid : pml::ptpmonkey::Mode::kMulticast);


        for(const auto& [sClock, pClock] : wxPtp::Get().GetClocks())
		{
            AddClock(wxString(sClock));
        }
    }
}

void ptpPanel::AddClock(wxString sClock)
{
    sClock = m_dbMac.CreateClockText(sClock);

    if(m_plstClocks->FindButton(sClock) == wmList::NOT_FOUND)
    {
		wxBitmap bmp(slave_xpm);
        if(wxPtp::Get().GetMasterClockId() == sClock)
        {
			bmp = BMP_GRAND;
        }
        else
        {
            auto pSyncMaster = wxPtp::Get().GetSyncMasterClock();
            if(pSyncMaster && pSyncMaster->GetId() == sClock)
            {
                bmp = BMP_SYNC;
            }
        }
        size_t nButton = m_plstClocks->AddButton(sClock, bmp, 0, wmList::wmENABLED, CLR_CLOCK);
        m_plstClocks->SetSelectedButtonColour(nButton, CLR_CLOCK_SELECTED);

    }
    if(m_pLocalClock == nullptr)
    {
        m_pLocalClock = wxPtp::Get().GetLocalClock();
    }
}

void ptpPanel::OnClockMessage(wxCommandEvent& event)
{
	ShowClockDetails();
    
	UpdateListBitmaps();
    ClockMessageWebsocketMessage(event.GetString());
}

void ptpPanel::ChangeView(const wxString& sWindow)
{
    m_pSwpMain->ChangeSelection(sWindow);

}

void ptpPanel::OnlstDataSelected(wxCommandEvent& event)
{

}

void ptpPanel::SetData(const wxString& sData)
{
    m_sGraph = sData;
    m_plstHistogramData->SelectButton(m_sGraph, false);
    m_plstGraphData->SelectButton(m_sGraph, false);

    m_plblGraphTitle->SetLabel("PTP "+m_sGraph);
    m_pHistoryGraph->HideAllGraphs();
    m_pHistoryGraph->ShowGraph(m_sGraph, true);
	if(m_sGraph != "Mean")
    {
		m_pHistoryGraph->ShowGraph(m_sGraph+"LR", true);
		m_pHistoryGraph->ShowGraph(m_sGraph+"Mean", true);
		m_pHistoryGraph->ShowGraph(m_sGraph+"VarMin", true);
		m_pHistoryGraph->ShowGraph(m_sGraph+"VarMax", true);
	}
    
	m_pHistoryGraph->SetMasterGraph(m_sGraph);
    m_plblHistogramTitle->SetLabel("PTP "+m_sGraph);
    m_pHistogram->HideAllGraphs();
    m_pHistogram->ShowGraph(m_sGraph);

    m_pBuilder->WriteSetting("Graph", m_sGraph);

    UpdateGraphLabels();
}

void ptpPanel::OnbtnClearClick(wxCommandEvent& event)
{
     m_pHistoryGraph->ClearGraphs();
    m_pHistogram->ClearGraphs();
}

void ptpPanel::OnbtnOptionsClick(wxCommandEvent& event)
{
    ChangeView("Info");
}

void ptpPanel::OnlstHistogramGranularitySelected(wxCommandEvent& event)
{
    m_pHistogram->ChangeGranularity(m_sGraph, event.GetInt());
}

void ptpPanel::OnlstHistogramResolutionSelected(wxCommandEvent& event)
{
    m_pHistogram->ChangeResolution(m_sGraph, event.GetInt());
}

void ptpPanel::OnbtnClearStatsClick(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting("reset", 1);
}

void ptpPanel::ResetStats()
{
    if(m_pLocalClock)
    {
        wxPtp::Get().ResetLocalClockStats();
    }
    m_pBuilder->WriteSetting("reset",0);
}


Json::Value FlagsWebsocketMessage(unsigned int nFlags)
{
    Json::Value jsFlags;
    jsFlags["alternate_master"] =    (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kAlternateMaster));
    jsFlags["two_step"] = 		     (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kTwoStep));
    jsFlags["unicast"] = 		     (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kUnicast));
    jsFlags["profile1"] = 		     (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kProfile1));
    jsFlags["profile2"] = 		     (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kProfile2));
    jsFlags["LI_61"] = 			     (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kLI_61));
    jsFlags["LI_59"] = 			     (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kLI_59));
    jsFlags["UTC_Offset"] = 	     (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kUtcOffsetValid));
    jsFlags["timescale"] = 		     (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kTimescale));
    jsFlags["time_traceable"] =      (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kTimeTraceable));
    jsFlags["frequency_traceable"] = (nFlags & static_cast<unsigned int>(pml::ptpmonkey::hdr::enumFlags::kFreqTraceable));
    return jsFlags;
}

void ptpPanel::ClockMessageWebsocketMessage(const wxString& sClock)
{
    if(m_pBuilder->WebsocketsActive())
    {
        Json::Value jsClock;
        jsClock["action"] = "Message";
        auto pClock = wxPtp::Get().GetPtpClock(sClock);
        if(pClock)
        {
            jsClock["id"] = pClock->GetClockId();
            jsClock["address"] = pClock->GetIpAddress();
            jsClock["vendor"] = m_dbMac.GetVendor(sClock).ToStdString();

            std::shared_ptr<const pml::ptpmonkey::PtpV2Clock> pSyncMaster = wxPtp::Get().GetSyncMasterClock();
            if(wxPtp::Get().GetMasterClockId() == sClock || (pSyncMaster && pSyncMaster->GetId() == sClock))
            {
                jsClock["master"] = true;
                jsClock["sync"]["count"] = static_cast<Json::UInt64>(pClock->GetCount(hdr::enumType::kSync));
                jsClock["sync"]["rate"] = ConvertRate(pClock->GetInterval(hdr::enumType::kSync)).ToStdString();
                jsClock["sync"]["flags"] = FlagsWebsocketMessage(pClock->GetFlags(hdr::enumType::kSync));

                jsClock["followup"]["count"] = static_cast<Json::UInt64>(pClock->GetCount(hdr::enumType::kFollowUp));
                jsClock["followup"]["rate"] = ConvertRate(pClock->GetInterval(hdr::enumType::kFollowUp)).ToStdString();
                jsClock["followup"]["flags"] = FlagsWebsocketMessage(pClock->GetFlags(hdr::enumType::kFollowUp));

            }
            else
            {
                jsClock["master"] = false;
                //jsClock["delay"]["type"] = "E-E";
                jsClock["delay_request"]["count"] = Json::UInt64(pClock->GetCount(hdr::enumType::kDelayReq));
                jsClock["delay_request"]["rate"] = ConvertRate(pClock->GetInterval(hdr::enumType::kDelayReq)).ToStdString();
                jsClock["delay_response"]["count"] = Json::UInt64(pClock->GetCount(hdr::enumType::kDelayResp));
                jsClock["delay_response"]["rate"] = ConvertRate(pClock->GetInterval(hdr::enumType::kDelayResp)).ToStdString();

                jsClock["delay_request"]["flags"] = FlagsWebsocketMessage(pClock->GetFlags(hdr::enumType::kDelayReq));
                jsClock["delay_response"]["flags"] = FlagsWebsocketMessage(pClock->GetFlags(hdr::enumType::kDelayResp));
            }

            if(pClock->GetCount(hdr::enumType::kAnnounce) != 0)
            {
                auto itAccuracy = kAccuracy.find(pClock->GetAccuracy());
                if(itAccuracy != kAccuracy.end())
                {
                    jsClock["announcement"]["accuracy"] = itAccuracy->second.ToStdString();
                }
                else
                {
                    jsClock["announcement"]["accuracy"] = "Unknown";
                }
                jsClock["announcement"]["count"] = Json::UInt64(pClock->GetCount(hdr::enumType::kAnnounce));
                jsClock["announcement"]["rate"] = ConvertRate(pClock->GetInterval(hdr::enumType::kAnnounce)).ToStdString();
                jsClock["announcement"]["class"] = pClock->GetClass();


                auto itSource = kTimeSource.find(pClock->GetTimeSource());
                if(itSource != kTimeSource.end())
                {
                    jsClock["announcement"]["source"] = itSource->second.ToStdString();
                }
                else
                {
                    jsClock["announcement"]["source"] = static_cast<unsigned int>(pClock->GetTimeSource());
                }
                jsClock["announcement"]["steps"] = pClock->GetStepsRemoved();
                jsClock["announcement"]["UTC_Offset"] = pClock->GetUtcOffset();
                jsClock["announcement"]["variance"] = pClock->GetVariance();
                jsClock["announcement"]["priority_1"] = pClock->GetPriority1();
                jsClock["announcement"]["priority_2"] = pClock->GetPriority2();

                jsClock["announcement"]["flags"] = FlagsWebsocketMessage(pClock->GetFlags(hdr::enumType::kAnnounce));
            }
        }
        m_pBuilder->SendWebsocketMessage(jsClock);
    }
}



void ptpPanel::ClockWebsocketMessage(const wxString& sClock, const wxString& sType)
{
    if(m_pBuilder->WebsocketsActive())
    {
        Json::Value jsClock;
        jsClock["action"] = sType.ToStdString();
        jsClock["id"] = sClock.ToStdString();
        jsClock["vendor"] = m_dbMac.GetVendor(sClock).ToStdString();
        m_pBuilder->SendWebsocketMessage(jsClock);
    }
}

void ptpPanel::TimeWebsocketMessage()
{
    auto pSyncMaster = wxPtp::Get().GetSyncMasterClock();

    if(m_pLocalClock && m_pBuilder->WebsocketsActive() && pSyncMaster)
    {
        auto dUTCOffset = static_cast<double>(pSyncMaster->GetUtcOffset());

        Json::Value jsTime;
        jsTime["action"] = "Time";
        jsTime["time"] = TimeToIsoString(m_pLocalClock->GetPtpTime());
        jsTime["frequency_locke"] = TimeManager::Get().IsPtpFrequencyLocked();
        jsTime["offset"]["mean"] = TimeToDouble(m_pLocalClock->GetOffset(pml::ptpmonkey::PtpV2Clock::MEAN))+dUTCOffset;
        jsTime["offset"]["standard deviation"] = TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SD));
        jsTime["offset"]["standard error"] = TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SE));

        jsTime["delay"]["mean"] = TimeToDouble(m_pLocalClock->GetDelay(pml::ptpmonkey::PtpV2Clock::MEAN));
        jsTime["delay"]["standard deviation"] = TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SD));
        jsTime["delay"]["standard error"] = TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SE));

        if(m_pLocalClock->IsSynced())
        {
            double m = m_pLocalClock->GetOffsetSlope();
            double c = m_pLocalClock->GetOffsetIntersection();
            auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

            jsTime["offset"]["linear regression"]["slope"] = m;
            jsTime["offset"]["linear regression"]["intersection"] = c;
            jsTime["offset"]["linear regression"]["estimate"] = (dUTCOffset+c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()))*1e6;

            m = m_pLocalClock->GetDelaySlope();
            c = m_pLocalClock->GetDelayIntersection();
            now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());

            jsTime["delay"]["linear regression"]["slope"] = m;
            jsTime["delay"]["linear regression"]["intersection"] = c;
            jsTime["delay"]["linear regression"]["estimate"] = (c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()))*1e6;

        }
        m_pBuilder->SendWebsocketMessage(jsTime);
    }
}

void ptpPanel::SetDomain(unsigned char nDomain)
{
    if(m_nDomain != nDomain)
    {
	    m_nDomain = nDomain;
        m_plblDomain->SetLabel(wxString::Format("Domain: %u", m_nDomain));
    }
}


void ptpPanel::OnbtnGraphsClick(wxCommandEvent& event)
{
    ChangeView("Graphs");
}

void ptpPanel::OnbtnHistogramClick(wxCommandEvent& event)
{
    ChangeView("Histograms");
}

void ptpPanel::UpdateListBitmaps()
{
	for(size_t nButton = 0; nButton < m_plstClocks->GetItemCount(); nButton++)
	{
		auto pClock = wxPtp::Get().GetPtpClock(m_plstClocks->GetButtonText(nButton).BeforeFirst('\n'));
		if(pClock)
		{
			if(pClock->IsSyncMaster())
			{
				if(pClock->IsGrandMaster())
				{
					m_plstClocks->SetButtonBitmap(nButton, BMP_GRAND);
				}
				else
				{	
					m_plstClocks->SetButtonBitmap(nButton, BMP_SYNC);
				}
			}
			else
			{
				m_plstClocks->SetButtonBitmap(nButton, BMP_SLAVE);
			}
		}
	}
}
