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


//(*InternalHeaders(ptpPanel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//(*IdInit(ptpPanel)
const long ptpPanel::ID_M_PLST1 = wxNewId();
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
const long ptpPanel::ID_M_PBTN3 = wxNewId();
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
const long ptpPanel::ID_M_PBTN2 = wxNewId();
const long ptpPanel::ID_PANEL16 = wxNewId();
const long ptpPanel::ID_M_PSWP2 = wxNewId();
//*)

const wxColour ptpPanel::CLR_MASTER           = wxColour(50,128,50);
const wxColour ptpPanel::CLR_MASTER_SELECTED  = wxColour(150,228,150);
const wxColour ptpPanel::CLR_SYNC_MASTER           = wxColour(50,50,128);
const wxColour ptpPanel::CLR_SYNC_MASTER_SELECTED  = wxColour(150,150,228);
const wxColour ptpPanel::CLR_SLAVE            = wxColour(50,50,50);
const wxColour ptpPanel::CLR_SLAVE_SELECTED   = wxColour(150,150,150);


using namespace ptpmonkey;


wxIMPLEMENT_DYNAMIC_CLASS(ptpPanel, pmPanel)

BEGIN_EVENT_TABLE(ptpPanel,pmPanel)
	//(*EventTable(ptpPanel)
	//*)
END_EVENT_TABLE()


ptpPanel::ptpPanel(wxWindow* parent, ptpBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder),
    m_nDomain(0),
    m_pClock(nullptr),
    m_pLocalClock(nullptr),
    m_mAccuracy({ { ptpAnnounce::ACC_25NS,  "Within 25ns"},
                  { ptpAnnounce::ACC_100NS, "Within 100ns"},
                  { ptpAnnounce::ACC_250NS, "Within 250ns" },
                  { ptpAnnounce::ACC_1US,   "Within 1us"},
                  { ptpAnnounce::ACC_2_5US, "Within 2.5us"},
                  { ptpAnnounce::ACC_10US,  "Within 10us"},
                  { ptpAnnounce::ACC_25US,  "Within 25us"},
                  { ptpAnnounce::ACC_100US, "Within 100us"},
                  { ptpAnnounce::ACC_250US, "Within 250us"},
                  { ptpAnnounce::ACC_1MS,   "Within 1ms"},
                  { ptpAnnounce::ACC_2_5MS, "Within 2.5ms"},
                  { ptpAnnounce::ACC_10MS,  "Within 10ms"},
                  { ptpAnnounce::ACC_25MS,  "Within 25ms"},
                  { ptpAnnounce::ACC_100MS, "Within 100ms"},
                  { ptpAnnounce::ACC_250MS, "Within 250ms"},
                  { ptpAnnounce::ACC_1S,    "Within 1s"},
                  { ptpAnnounce::ACC_10S,   "Within 10s"},
                  { ptpAnnounce::ACC_OVER,  "Over 10ss"},
                  { ptpAnnounce::ACC_UNKNOWN,"Unknown" }}),
    m_mTimeSource({ { ptpAnnounce::ATOMIC, "Atomic"},
                    { ptpAnnounce::GPS, "GPS"},
                    { ptpAnnounce::RADIO,"Radio"},
                    { ptpAnnounce::PTP, "PTP"},
                    { ptpAnnounce::NTP, "NTP"},
                    { ptpAnnounce::HANDSET, "Handset"},
                    { ptpAnnounce::OTHER, "Other"},
                    { ptpAnnounce::INTERNAL, "Internal"}}),
    m_mRate({{static_cast<std::int8_t>(Rate::PER_SEC_128), "128/s"},
             {static_cast<std::int8_t>(Rate::PER_SEC_64), "64/s"},
             {static_cast<std::int8_t>(Rate::PER_SEC_32), "32/s"},
             {static_cast<std::int8_t>(Rate::PER_SEC_16), "16/s"},
             {static_cast<std::int8_t>(Rate::PER_SEC_8), "8/s"},
             {static_cast<std::int8_t>(Rate::PER_SEC_4), "4/s"},
             {static_cast<std::int8_t>(Rate::PER_SEC_2), "2/s"},
             {static_cast<std::int8_t>(Rate::EVERY_1_SEC), "1/s"},
             {static_cast<std::int8_t>(Rate::EVERY_2_SEC), "1/2s"},
             {static_cast<std::int8_t>(Rate::EVERY_4_SEC), "1/4s"},
             {static_cast<std::int8_t>(Rate::EVERY_8_SEC), "1/8s"},
             {static_cast<std::int8_t>(Rate::EVERY_16_SEC), "1/16s"}}),
    m_bRunning(false)
{
	//(*Initialize(ptpPanel)
	Create(parent, wxID_ANY, wxDefaultPosition, wxSize(800,480), wxTAB_TRAVERSAL, _T("wxID_ANY"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pSwpMain = new wmSwitcherPanel(this, ID_M_PSWP2, wxDefaultPosition, wxSize(800,480), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
	m_pSwpMain->SetPageNameStyle(0);
	m_ppnlInfo = new wxPanel(m_pSwpMain, ID_PANEL14, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL14"));
	m_ppnlInfo->SetBackgroundColour(wxColour(0,0,0));
	m_plstClocks = new wmList(m_ppnlInfo, ID_M_PLST1, wxPoint(605,5), wxSize(190,470), wmList::STYLE_SELECT, 1, wxSize(-1,-1), 1, wxSize(-1,-1));
	m_plstClocks->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
	m_plstClocks->SetButtonColour(wxColour(wxT("#000000")));
	m_plstClocks->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_ptitleIdentity = new wmLabel(m_ppnlInfo, ID_M_PLBL1, _("Port Identitiy"), wxPoint(5,5), wxSize(100,25), 0, _T("ID_M_PLBL1"));
	m_ptitleIdentity->SetBorderState(uiRect::BORDER_FLAT);
	m_ptitleIdentity->GetUiRect().SetGradient(wxWEST);
	m_ptitleIdentity->SetForegroundColour(wxColour(255,255,255));
	m_ptitleIdentity->SetBackgroundColour(wxColour(92,122,224));
	m_ptitleAddress = new wmLabel(m_ppnlInfo, ID_M_PLBL2, _("Address"), wxPoint(5,35), wxSize(100,25), 0, _T("ID_M_PLBL2"));
	m_ptitleAddress->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleAddress->GetUiRect().SetGradient(wxWEST);
	m_ptitleAddress->SetForegroundColour(wxColour(255,255,255));
	m_ptitleAddress->SetBackgroundColour(wxColour(92,122,224));
	m_ptitleState = new wmLabel(m_ppnlInfo, ID_M_PLBL3, _("State"), wxPoint(5,65), wxSize(100,25), 0, _T("ID_M_PLBL3"));
	m_ptitleState->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleState->GetUiRect().SetGradient(wxWEST);
	m_ptitleState->SetForegroundColour(wxColour(255,255,255));
	m_ptitleState->SetBackgroundColour(wxColour(92,122,224));
	m_plblIdentity = new wmLabel(m_ppnlInfo, ID_M_PLBL12, wxEmptyString, wxPoint(105,5), wxSize(180,25), 0, _T("ID_M_PLBL12"));
	m_plblIdentity->SetBorderState(uiRect::BORDER_DOWN);
	m_plblIdentity->GetUiRect().SetGradient(0);
	m_plblIdentity->SetForegroundColour(wxColour(0,0,0));
	m_plblIdentity->SetBackgroundColour(wxColour(255,255,255));
	m_pblAddress = new wmLabel(m_ppnlInfo, ID_M_PLBL13, wxEmptyString, wxPoint(105,35), wxSize(180,25), 0, _T("ID_M_PLBL13"));
	m_pblAddress->SetBorderState(uiRect::BORDER_DOWN);
	m_pblAddress->GetUiRect().SetGradient(0);
	m_pblAddress->SetForegroundColour(wxColour(0,0,0));
	m_pblAddress->SetBackgroundColour(wxColour(255,255,255));
	m_plblState = new wmLabel(m_ppnlInfo, ID_M_PLBL14, wxEmptyString, wxPoint(105,65), wxSize(180,25), 0, _T("ID_M_PLBL14"));
	m_plblState->SetBorderState(uiRect::BORDER_DOWN);
	m_plblState->GetUiRect().SetGradient(0);
	m_plblState->SetForegroundColour(wxColour(0,0,0));
	m_plblState->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlAnnouncements = new wxPanel(m_ppnlInfo, ID_PANEL1, wxPoint(5,95), wxSize(280,275), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_ppnlAnnouncements->SetBackgroundColour(wxColour(0,0,0));
	m_ptitleAnnouncements = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL30, _("Announcements"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL30"));
	m_ptitleAnnouncements->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleAnnouncements->GetUiRect().SetGradient(wxWEST);
	m_ptitleAnnouncements->SetForegroundColour(wxColour(255,255,255));
	m_ptitleAnnouncements->SetBackgroundColour(wxColour(98,219,101));
	wxFont m_ptitleAnnouncementsFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitleAnnouncements->SetFont(m_ptitleAnnouncementsFont);
	m_ptitleAnnRate = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL24, _("Rate"), wxPoint(0,30), wxSize(70,25), 0, _T("ID_M_PLBL24"));
	m_ptitleAnnRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleAnnRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleAnnRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleAnnRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblAnnRate = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL25, wxEmptyString, wxPoint(70,30), wxSize(70,25), 0, _T("ID_M_PLBL25"));
	m_plblAnnRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblAnnRate->GetUiRect().SetGradient(0);
	m_plblAnnRate->SetForegroundColour(wxColour(0,0,0));
	m_plblAnnRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleCount = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL26, _("Count"), wxPoint(140,30), wxSize(70,25), 0, _T("ID_M_PLBL26"));
	m_ptitleCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblAnnCount = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL27, wxEmptyString, wxPoint(210,30), wxSize(70,25), 0, _T("ID_M_PLBL27"));
	m_plblAnnCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblAnnCount->GetUiRect().SetGradient(0);
	m_plblAnnCount->SetForegroundColour(wxColour(0,0,0));
	m_plblAnnCount->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleUtc = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL4, _("UTC Offset"), wxPoint(140,90), wxSize(70,25), 0, _T("ID_M_PLBL4"));
	m_ptitleUtc->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleUtc->GetUiRect().SetGradient(wxWEST);
	m_ptitleUtc->SetForegroundColour(wxColour(255,255,255));
	m_ptitleUtc->SetBackgroundColour(wxColour(92,122,224));
	m_plblUTC = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL15, wxEmptyString, wxPoint(210,90), wxSize(70,25), 0, _T("ID_M_PLBL15"));
	m_plblUTC->SetBorderState(uiRect::BORDER_DOWN);
	m_plblUTC->GetUiRect().SetGradient(0);
	m_plblUTC->SetForegroundColour(wxColour(0,0,0));
	m_plblUTC->SetBackgroundColour(wxColour(255,255,255));
	m_ptitlePriority1 = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL5, _("Priority 1"), wxPoint(0,60), wxSize(70,25), 0, _T("ID_M_PLBL5"));
	m_ptitlePriority1->SetBorderState(uiRect::BORDER_NONE);
	m_ptitlePriority1->GetUiRect().SetGradient(wxWEST);
	m_ptitlePriority1->SetForegroundColour(wxColour(255,255,255));
	m_ptitlePriority1->SetBackgroundColour(wxColour(92,122,224));
	m_plblPriority1 = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL16, wxEmptyString, wxPoint(70,60), wxSize(70,25), 0, _T("ID_M_PLBL16"));
	m_plblPriority1->SetBorderState(uiRect::BORDER_DOWN);
	m_plblPriority1->GetUiRect().SetGradient(0);
	m_plblPriority1->SetForegroundColour(wxColour(0,0,0));
	m_plblPriority1->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleClass = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL7, _("Class"), wxPoint(140,60), wxSize(70,25), 0, _T("ID_M_PLBL7"));
	m_ptitleClass->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleClass->GetUiRect().SetGradient(wxWEST);
	m_ptitleClass->SetForegroundColour(wxColour(255,255,255));
	m_ptitleClass->SetBackgroundColour(wxColour(92,122,224));
	m_plblClass = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL17, wxEmptyString, wxPoint(210,60), wxSize(70,25), 0, _T("ID_M_PLBL17"));
	m_plblClass->SetBorderState(uiRect::BORDER_DOWN);
	m_plblClass->GetUiRect().SetGradient(0);
	m_plblClass->SetForegroundColour(wxColour(0,0,0));
	m_plblClass->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleAccuracy = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL8, _("Accuracy"), wxPoint(0,90), wxSize(70,25), 0, _T("ID_M_PLBL8"));
	m_ptitleAccuracy->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleAccuracy->GetUiRect().SetGradient(wxWEST);
	m_ptitleAccuracy->SetForegroundColour(wxColour(255,255,255));
	m_ptitleAccuracy->SetBackgroundColour(wxColour(92,122,224));
	m_plblAccuracy = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL18, _("Unknown"), wxPoint(70,90), wxSize(70,25), 0, _T("ID_M_PLBL18"));
	m_plblAccuracy->SetBorderState(uiRect::BORDER_DOWN);
	m_plblAccuracy->GetUiRect().SetGradient(0);
	m_plblAccuracy->SetForegroundColour(wxColour(0,0,0));
	m_plblAccuracy->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleVariance = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL9, _("Variance"), wxPoint(0,120), wxSize(70,25), 0, _T("ID_M_PLBL9"));
	m_ptitleVariance->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleVariance->GetUiRect().SetGradient(wxWEST);
	m_ptitleVariance->SetForegroundColour(wxColour(255,255,255));
	m_ptitleVariance->SetBackgroundColour(wxColour(92,122,224));
	m_plblVariance = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL19, wxEmptyString, wxPoint(70,120), wxSize(70,25), 0, _T("ID_M_PLBL19"));
	m_plblVariance->SetBorderState(uiRect::BORDER_DOWN);
	m_plblVariance->GetUiRect().SetGradient(0);
	m_plblVariance->SetForegroundColour(wxColour(0,0,0));
	m_plblVariance->SetBackgroundColour(wxColour(255,255,255));
	m_ptitlePriority2 = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL6, _("Priority 2"), wxPoint(140,120), wxSize(70,25), 0, _T("ID_M_PLBL6"));
	m_ptitlePriority2->SetBorderState(uiRect::BORDER_NONE);
	m_ptitlePriority2->GetUiRect().SetGradient(wxWEST);
	m_ptitlePriority2->SetForegroundColour(wxColour(255,255,255));
	m_ptitlePriority2->SetBackgroundColour(wxColour(92,122,224));
	m_plblPriority2 = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL20, wxEmptyString, wxPoint(210,120), wxSize(70,25), 0, _T("ID_M_PLBL20"));
	m_plblPriority2->SetBorderState(uiRect::BORDER_DOWN);
	m_plblPriority2->GetUiRect().SetGradient(0);
	m_plblPriority2->SetForegroundColour(wxColour(0,0,0));
	m_plblPriority2->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleSteps = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL11, _("Steps"), wxPoint(0,150), wxSize(70,25), 0, _T("ID_M_PLBL11"));
	m_ptitleSteps->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleSteps->GetUiRect().SetGradient(wxWEST);
	m_ptitleSteps->SetForegroundColour(wxColour(255,255,255));
	m_ptitleSteps->SetBackgroundColour(wxColour(92,122,224));
	m_plblSteps = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL21, wxEmptyString, wxPoint(70,150), wxSize(70,25), 0, _T("ID_M_PLBL21"));
	m_plblSteps->SetBorderState(uiRect::BORDER_DOWN);
	m_plblSteps->GetUiRect().SetGradient(0);
	m_plblSteps->SetForegroundColour(wxColour(0,0,0));
	m_plblSteps->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleSource = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL10, _("Source"), wxPoint(140,150), wxSize(70,25), 0, _T("ID_M_PLBL10"));
	m_ptitleSource->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleSource->GetUiRect().SetGradient(wxWEST);
	m_ptitleSource->SetForegroundColour(wxColour(255,255,255));
	m_ptitleSource->SetBackgroundColour(wxColour(92,122,224));
	m_plblTitleMaster = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL35, _("Master"), wxPoint(0,180), wxSize(70,25), 0, _T("ID_M_PLBL35"));
	m_plblTitleMaster->SetBorderState(uiRect::BORDER_NONE);
	m_plblTitleMaster->GetUiRect().SetGradient(wxWEST);
	m_plblTitleMaster->SetForegroundColour(wxColour(255,255,255));
	m_plblTitleMaster->SetBackgroundColour(wxColour(92,122,224));
	m_plblMasterId = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL39, wxEmptyString, wxPoint(70,180), wxSize(210,25), 0, _T("ID_M_PLBL39"));
	m_plblMasterId->SetBorderState(uiRect::BORDER_DOWN);
	m_plblMasterId->GetUiRect().SetGradient(0);
	m_plblMasterId->SetForegroundColour(wxColour(0,0,0));
	m_plblMasterId->SetBackgroundColour(wxColour(255,255,255));
	m_plblSource = new wmLabel(m_ppnlAnnouncements, ID_M_PLBL22, wxEmptyString, wxPoint(210,150), wxSize(70,25), 0, _T("ID_M_PLBL22"));
	m_plblSource->SetBorderState(uiRect::BORDER_DOWN);
	m_plblSource->GetUiRect().SetGradient(0);
	m_plblSource->SetForegroundColour(wxColour(0,0,0));
	m_plblSource->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlAnnounceFlags = new pnlFlags(m_ppnlAnnouncements, ID_PANEL9, wxPoint(0,210), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL9"));
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
	m_ptitleSyncRate = new wmLabel(m_ppnlSync, ID_M_PLBL31, _("Rate"), wxPoint(0,30), wxSize(50,25), 0, _T("ID_M_PLBL31"));
	m_ptitleSyncRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleSyncRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleSyncRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleSyncRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblSyncRate = new wmLabel(m_ppnlSync, ID_M_PLBL32, wxEmptyString, wxPoint(50,30), wxSize(50,25), 0, _T("ID_M_PLBL32"));
	m_plblSyncRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblSyncRate->GetUiRect().SetGradient(0);
	m_plblSyncRate->SetForegroundColour(wxColour(0,0,0));
	m_plblSyncRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleSyncCount = new wmLabel(m_ppnlSync, ID_M_PLBL33, _("Count"), wxPoint(100,30), wxSize(50,25), 0, _T("ID_M_PLBL33"));
	m_ptitleSyncCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleSyncCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleSyncCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleSyncCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblSyncCount = new wmLabel(m_ppnlSync, ID_M_PLBL34, wxEmptyString, wxPoint(150,30), wxSize(130,25), 0, _T("ID_M_PLBL34"));
	m_plblSyncCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblSyncCount->GetUiRect().SetGradient(0);
	m_plblSyncCount->SetForegroundColour(wxColour(0,0,0));
	m_plblSyncCount->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlSyncFlags = new pnlFlags(m_ppnlSync, ID_PANEL10, wxPoint(0,60), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL10"));
	m_ppnlFollowUp = new wxPanel(m_ppnlMaster, ID_PANEL3, wxPoint(0,135), wxSize(280,125), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	m_ppnlFollowUp->SetBackgroundColour(wxColour(0,0,0));
	m_ptitleFollowUp = new wmLabel(m_ppnlFollowUp, ID_M_PLBL28, _("Follow Up"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL28"));
	m_ptitleFollowUp->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleFollowUp->GetUiRect().SetGradient(wxWEST);
	m_ptitleFollowUp->SetForegroundColour(wxColour(255,255,255));
	m_ptitleFollowUp->SetBackgroundColour(wxColour(98,219,101));
	wxFont m_ptitleFollowUpFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitleFollowUp->SetFont(m_ptitleFollowUpFont);
	m_ptitleFollowRate = new wmLabel(m_ppnlFollowUp, ID_M_PLBL29, _("Rate"), wxPoint(0,30), wxSize(50,25), 0, _T("ID_M_PLBL29"));
	m_ptitleFollowRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleFollowRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleFollowRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleFollowRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblFollowRate = new wmLabel(m_ppnlFollowUp, ID_M_PLBL36, wxEmptyString, wxPoint(50,30), wxSize(50,25), 0, _T("ID_M_PLBL36"));
	m_plblFollowRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblFollowRate->GetUiRect().SetGradient(0);
	m_plblFollowRate->SetForegroundColour(wxColour(0,0,0));
	m_plblFollowRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleFollowCount = new wmLabel(m_ppnlFollowUp, ID_M_PLBL37, _("Count"), wxPoint(100,30), wxSize(50,25), 0, _T("ID_M_PLBL37"));
	m_ptitleFollowCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleFollowCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleFollowCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleFollowCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblFollowCount = new wmLabel(m_ppnlFollowUp, ID_M_PLBL38, wxEmptyString, wxPoint(150,30), wxSize(130,25), 0, _T("ID_M_PLBL38"));
	m_plblFollowCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblFollowCount->GetUiRect().SetGradient(0);
	m_plblFollowCount->SetForegroundColour(wxColour(0,0,0));
	m_plblFollowCount->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlFollowFlags = new pnlFlags(m_ppnlFollowUp, ID_PANEL11, wxPoint(0,60), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL11"));
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
	m_ptitleDelayRate = new wmLabel(m_ppnlRequests, ID_M_PLBL41, _("Rate"), wxPoint(0,30), wxSize(70,25), 0, _T("ID_M_PLBL41"));
	m_ptitleDelayRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayRate = new wmLabel(m_ppnlRequests, ID_M_PLBL42, wxEmptyString, wxPoint(70,30), wxSize(70,25), 0, _T("ID_M_PLBL42"));
	m_plblDelayRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayRate->GetUiRect().SetGradient(0);
	m_plblDelayRate->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleDelayCount = new wmLabel(m_ppnlRequests, ID_M_PLBL43, _("Count"), wxPoint(140,30), wxSize(70,25), 0, _T("ID_M_PLBL43"));
	m_ptitleDelayCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayCount = new wmLabel(m_ppnlRequests, ID_M_PLBL44, wxEmptyString, wxPoint(210,30), wxSize(70,25), 0, _T("ID_M_PLBL44"));
	m_plblDelayCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayCount->GetUiRect().SetGradient(0);
	m_plblDelayCount->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayCount->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleDelayMech = new wmLabel(m_ppnlRequests, ID_M_PLBL46, _("Mechanism"), wxPoint(0,60), wxSize(70,25), 0, _T("ID_M_PLBL46"));
	m_ptitleDelayMech->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayMech->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayMech->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayMech->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayMech = new wmLabel(m_ppnlRequests, ID_M_PLBL47, wxEmptyString, wxPoint(70,60), wxSize(210,25), 0, _T("ID_M_PLBL47"));
	m_plblDelayMech->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayMech->GetUiRect().SetGradient(0);
	m_plblDelayMech->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayMech->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlRequestFlags = new pnlFlags(m_ppnlRequests, ID_PANEL13, wxPoint(0,90), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL13"));
	m_ppnlResponses = new wxPanel(m_ppnlSlave, ID_PANEL7, wxPoint(0,160), wxSize(280,124), wxTAB_TRAVERSAL, _T("ID_PANEL7"));
	m_ppnlResponses->SetBackgroundColour(wxColour(0,0,0));
	m_ptitleDelayResponses = new wmLabel(m_ppnlResponses, ID_M_PLBL48, _("Delay Responses Received"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL48"));
	m_ptitleDelayResponses->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayResponses->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayResponses->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayResponses->SetBackgroundColour(wxColour(98,219,101));
	wxFont m_ptitleDelayResponsesFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitleDelayResponses->SetFont(m_ptitleDelayResponsesFont);
	m_ptitleResponseRate = new wmLabel(m_ppnlResponses, ID_M_PLBL49, _("Rate"), wxPoint(0,30), wxSize(70,25), 0, _T("ID_M_PLBL49"));
	m_ptitleResponseRate->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleResponseRate->GetUiRect().SetGradient(wxWEST);
	m_ptitleResponseRate->SetForegroundColour(wxColour(255,255,255));
	m_ptitleResponseRate->SetBackgroundColour(wxColour(92,122,224));
	m_plblResponseRate = new wmLabel(m_ppnlResponses, ID_M_PLBL50, wxEmptyString, wxPoint(70,30), wxSize(70,25), 0, _T("ID_M_PLBL50"));
	m_plblResponseRate->SetBorderState(uiRect::BORDER_DOWN);
	m_plblResponseRate->GetUiRect().SetGradient(0);
	m_plblResponseRate->SetForegroundColour(wxColour(0,0,0));
	m_plblResponseRate->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleResponseCount = new wmLabel(m_ppnlResponses, ID_M_PLBL51, _("Count"), wxPoint(140,30), wxSize(70,25), 0, _T("ID_M_PLBL51"));
	m_ptitleResponseCount->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleResponseCount->GetUiRect().SetGradient(wxWEST);
	m_ptitleResponseCount->SetForegroundColour(wxColour(255,255,255));
	m_ptitleResponseCount->SetBackgroundColour(wxColour(92,122,224));
	m_plblResponseCount = new wmLabel(m_ppnlResponses, ID_M_PLBL52, wxEmptyString, wxPoint(210,30), wxSize(70,25), 0, _T("ID_M_PLBL52"));
	m_plblResponseCount->SetBorderState(uiRect::BORDER_DOWN);
	m_plblResponseCount->GetUiRect().SetGradient(0);
	m_plblResponseCount->SetForegroundColour(wxColour(0,0,0));
	m_plblResponseCount->SetBackgroundColour(wxColour(255,255,255));
	m_ppnlResponseFlags = new pnlFlags(m_ppnlResponses, ID_PANEL12, wxPoint(0,60), wxSize(280,64), wxTAB_TRAVERSAL, _T("ID_PANEL12"));
	m_pswp->AddPage(m_ppnlMaster, _("Master"), false);
	m_pswp->AddPage(m_ppnlSlave, _("Slave"), false);
	m_ppnlLocal = new wxPanel(m_ppnlInfo, ID_PANEL8, wxPoint(295,290), wxSize(280,175), wxTAB_TRAVERSAL, _T("ID_PANEL8"));
	m_ppnlLocal->SetBackgroundColour(wxColour(0,0,0));
	m_ptitlePam = new wmLabel(m_ppnlLocal, ID_M_PLBL54, _("PAM"), wxPoint(0,0), wxSize(280,25), 0, _T("ID_M_PLBL54"));
	m_ptitlePam->SetBorderState(uiRect::BORDER_NONE);
	m_ptitlePam->GetUiRect().SetGradient(wxWEST);
	m_ptitlePam->SetForegroundColour(wxColour(255,255,255));
	m_ptitlePam->SetBackgroundColour(wxColour(217,138,100));
	wxFont m_ptitlePamFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,wxEmptyString,wxFONTENCODING_DEFAULT);
	m_ptitlePam->SetFont(m_ptitlePamFont);
	m_ptitleTime = new wmLabel(m_ppnlLocal, ID_M_PLBL55, _("Time"), wxPoint(0,30), wxSize(50,25), 0, _T("ID_M_PLBL55"));
	m_ptitleTime->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleTime->GetUiRect().SetGradient(wxWEST);
	m_ptitleTime->SetForegroundColour(wxColour(255,255,255));
	m_ptitleTime->SetBackgroundColour(wxColour(92,122,224));
	m_plblTime = new wmLabel(m_ppnlLocal, ID_M_PLBL56, wxEmptyString, wxPoint(50,30), wxSize(230,25), 0, _T("ID_M_PLBL56"));
	m_plblTime->SetBorderState(uiRect::BORDER_DOWN);
	m_plblTime->GetUiRect().SetGradient(0);
	m_plblTime->SetForegroundColour(wxColour(0,0,0));
	m_plblTime->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleOffsetAverage = new wmLabel(m_ppnlLocal, ID_M_PLBL57, _("Offset: Average"), wxPoint(0,60), wxSize(100,25), 0, _T("ID_M_PLBL57"));
	m_ptitleOffsetAverage->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleOffsetAverage->GetUiRect().SetGradient(wxWEST);
	m_ptitleOffsetAverage->SetForegroundColour(wxColour(255,255,255));
	m_ptitleOffsetAverage->SetBackgroundColour(wxColour(92,122,224));
	m_plblOffsetAverage = new wmLabel(m_ppnlLocal, ID_M_PLBL61, wxEmptyString, wxPoint(100,60), wxSize(180,25), 0, _T("ID_M_PLBL61"));
	m_plblOffsetAverage->SetBorderState(uiRect::BORDER_DOWN);
	m_plblOffsetAverage->GetUiRect().SetGradient(0);
	m_plblOffsetAverage->SetForegroundColour(wxColour(0,0,0));
	m_plblOffsetAverage->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleOffsetRange = new wmLabel(m_ppnlLocal, ID_M_PLBL58, _("Offset: Range"), wxPoint(0,90), wxSize(100,25), 0, _T("ID_M_PLBL58"));
	m_ptitleOffsetRange->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleOffsetRange->GetUiRect().SetGradient(wxWEST);
	m_ptitleOffsetRange->SetForegroundColour(wxColour(255,255,255));
	m_ptitleOffsetRange->SetBackgroundColour(wxColour(92,122,224));
	m_plblOffsetRange = new wmLabel(m_ppnlLocal, ID_M_PLBL62, wxEmptyString, wxPoint(100,90), wxSize(180,25), 0, _T("ID_M_PLBL62"));
	m_plblOffsetRange->SetBorderState(uiRect::BORDER_DOWN);
	m_plblOffsetRange->GetUiRect().SetGradient(0);
	m_plblOffsetRange->SetForegroundColour(wxColour(0,0,0));
	m_plblOffsetRange->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleDelayAverage = new wmLabel(m_ppnlLocal, ID_M_PLBL59, _("Delay: Average"), wxPoint(0,120), wxSize(100,25), 0, _T("ID_M_PLBL59"));
	m_ptitleDelayAverage->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayAverage->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayAverage->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayAverage->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayAverage = new wmLabel(m_ppnlLocal, ID_M_PLBL63, wxEmptyString, wxPoint(100,120), wxSize(180,25), 0, _T("ID_M_PLBL63"));
	m_plblDelayAverage->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayAverage->GetUiRect().SetGradient(0);
	m_plblDelayAverage->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayAverage->SetBackgroundColour(wxColour(255,255,255));
	m_ptitleDelayRange = new wmLabel(m_ppnlLocal, ID_M_PLBL60, _("Delay: Range"), wxPoint(0,150), wxSize(100,25), 0, _T("ID_M_PLBL60"));
	m_ptitleDelayRange->SetBorderState(uiRect::BORDER_NONE);
	m_ptitleDelayRange->GetUiRect().SetGradient(wxWEST);
	m_ptitleDelayRange->SetForegroundColour(wxColour(255,255,255));
	m_ptitleDelayRange->SetBackgroundColour(wxColour(92,122,224));
	m_plblDelayRange = new wmLabel(m_ppnlLocal, ID_M_PLBL64, wxEmptyString, wxPoint(100,150), wxSize(180,25), 0, _T("ID_M_PLBL64"));
	m_plblDelayRange->SetBorderState(uiRect::BORDER_DOWN);
	m_plblDelayRange->GetUiRect().SetGradient(0);
	m_plblDelayRange->SetForegroundColour(wxColour(0,0,0));
	m_plblDelayRange->SetBackgroundColour(wxColour(255,255,255));
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
	m_plblCurrent = new wmLabel(m_ppnlGraphs, ID_M_PLBL74, wxEmptyString, wxPoint(50,30), wxSize(100,30), wxALIGN_LEFT, _T("ID_M_PLBL74"));
	m_plblCurrent->SetBorderState(uiRect::BORDER_NONE);
	m_plblCurrent->GetUiRect().SetGradient(0);
	m_plblCurrent->SetForegroundColour(wxColour(186,254,167));
	m_plblCurrent->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblCurrentFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblCurrent->SetFont(m_plblCurrentFont);
	m_pLbl6 = new wmLabel(m_ppnlGraphs, ID_M_PLBL70, _("Mean:"), wxPoint(160,30), wxSize(50,30), 0, _T("ID_M_PLBL70"));
	m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl6->GetUiRect().SetGradient(0);
	m_pLbl6->SetForegroundColour(wxColour(255,255,255));
	m_pLbl6->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl6Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl6->SetFont(m_pLbl6Font);
	m_plblMean = new wmLabel(m_ppnlGraphs, ID_M_PLBL75, wxEmptyString, wxPoint(210,30), wxSize(100,30), wxALIGN_LEFT, _T("ID_M_PLBL75"));
	m_plblMean->SetBorderState(uiRect::BORDER_NONE);
	m_plblMean->GetUiRect().SetGradient(0);
	m_plblMean->SetForegroundColour(wxColour(186,254,167));
	m_plblMean->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblMeanFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblMean->SetFont(m_plblMeanFont);
	m_pLbl7 = new wmLabel(m_ppnlGraphs, ID_M_PLBL71, _("SD:"), wxPoint(320,30), wxSize(50,30), 0, _T("ID_M_PLBL71"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl7Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl7->SetFont(m_pLbl7Font);
	m_plblDeviation = new wmLabel(m_ppnlGraphs, ID_M_PLBL76, wxEmptyString, wxPoint(370,30), wxSize(100,30), wxALIGN_LEFT, _T("ID_M_PLBL76"));
	m_plblDeviation->SetBorderState(uiRect::BORDER_NONE);
	m_plblDeviation->GetUiRect().SetGradient(0);
	m_plblDeviation->SetForegroundColour(wxColour(186,254,167));
	m_plblDeviation->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblDeviationFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblDeviation->SetFont(m_plblDeviationFont);
	m_pLbl8 = new wmLabel(m_ppnlGraphs, ID_M_PLBL72, _("LR Slope:"), wxPoint(480,30), wxSize(50,30), 0, _T("ID_M_PLBL72"));
	m_pLbl8->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl8->GetUiRect().SetGradient(0);
	m_pLbl8->SetForegroundColour(wxColour(255,255,255));
	m_pLbl8->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl8Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl8->SetFont(m_pLbl8Font);
	m_plblSlope = new wmLabel(m_ppnlGraphs, ID_M_PLBL77, wxEmptyString, wxPoint(530,30), wxSize(100,30), wxALIGN_LEFT, _T("ID_M_PLBL77"));
	m_plblSlope->SetBorderState(uiRect::BORDER_NONE);
	m_plblSlope->GetUiRect().SetGradient(0);
	m_plblSlope->SetForegroundColour(wxColour(186,254,167));
	m_plblSlope->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblSlopeFont(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_plblSlope->SetFont(m_plblSlopeFont);
	m_pLbl9 = new wmLabel(m_ppnlGraphs, ID_M_PLBL73, _("LR Pred:"), wxPoint(640,30), wxSize(50,30), 0, _T("ID_M_PLBL73"));
	m_pLbl9->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl9->GetUiRect().SetGradient(0);
	m_pLbl9->SetForegroundColour(wxColour(255,255,255));
	m_pLbl9->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl9Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl9->SetFont(m_pLbl9Font);
	m_plblPrediction = new wmLabel(m_ppnlGraphs, ID_M_PLBL78, wxEmptyString, wxPoint(690,30), wxSize(100,30), wxALIGN_LEFT, _T("ID_M_PLBL78"));
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
	m_plstGraphData = new wmList(m_ppnlGraphs, ID_M_PLST5, wxPoint(70,443), wxSize(120,34), wmList::STYLE_SELECT, 0, wxSize(-1,30), 2, wxSize(-1,-1));
	m_plstGraphData->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnGraphClear = new wmButton(m_ppnlGraphs, ID_M_PBTN3, _("Clear All"), wxPoint(600,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN3"));
	m_pbtnGraphOptions = new wmButton(m_ppnlGraphs, ID_M_PBTN4, _("Options"), wxPoint(720,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
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
	m_plstHistogramData = new wmList(m_ppnlHistograms, ID_M_PLST4, wxPoint(70,443), wxSize(120,34), wmList::STYLE_SELECT, 0, wxSize(-1,30), 2, wxSize(-1,-1));
	m_plstHistogramData->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(m_ppnlHistograms, ID_M_PLBL65, _("Granularity"), wxPoint(200,445), wxSize(60,30), 0, _T("ID_M_PLBL65"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl1Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_plstHistogramGranularity = new wmList(m_ppnlHistograms, ID_M_PLST2, wxPoint(260,443), wxSize(120,34), 0, 0, wxSize(-1,30), 3, wxSize(-1,-1));
	m_plstHistogramGranularity->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl2 = new wmLabel(m_ppnlHistograms, ID_M_PLBL66, _("Resolution"), wxPoint(400,445), wxSize(60,30), 0, _T("ID_M_PLBL66"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl2Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_plstHistogramResolution = new wmList(m_ppnlHistograms, ID_M_PLST3, wxPoint(460,443), wxSize(120,34), 0, 0, wxSize(-1,30), 3, wxSize(-1,-1));
	m_plstHistogramResolution->SetBackgroundColour(wxColour(0,0,0));
	m_pbtnHistogramClear = new wmButton(m_ppnlHistograms, ID_M_PBTN1, _("Clear All"), wxPoint(600,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnOptions = new wmButton(m_ppnlHistograms, ID_M_PBTN2, _("Options"), wxPoint(720,445), wxSize(70,30), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pSwpMain->AddPage(m_ppnlInfo, _("Info"), false);
	m_pSwpMain->AddPage(m_ppnlGraphs, _("Graphs"), false);
	m_pSwpMain->AddPage(m_ppnlHistograms, _("Histograms"), false);

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstClocksSelected);
	Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstDataSelected);
	Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnClearClick);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnOptionsClick);
	Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstDataSelected);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstHistogramGranularitySelected);
	Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&ptpPanel::OnlstHistogramResolutionSelected);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnClearClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ptpPanel::OnbtnOptionsClick);
	//*)

    m_plstHistogramGranularity->AddButton("-");
    m_plstHistogramGranularity->AddButton("0");
    m_plstHistogramGranularity->AddButton("+");

    m_plstHistogramResolution->AddButton("-");
    m_plstHistogramResolution->AddButton("0");
    m_plstHistogramResolution->AddButton("+");

    m_plstHistogramData->AddButton("Offset");
    m_plstHistogramData->AddButton("Delay");
    m_plstGraphData->AddButton("Offset");
    m_plstGraphData->AddButton("Delay");

	m_plstClocks->SetTextSelectedButtonColour(*wxBLACK);

	m_offset = 0.0;



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

	m_pHistoryGraph->AddGraph("Delay", wxColour(200,200,255), 2e6, true, false);
	m_pHistoryGraph->AddGraph("DelayLR", wxColour(100,255,100), 2e6, true, true);
    m_pHistoryGraph->AddGraph("DelayMean", wxColour(255,255,255), 2e6, true, true);
	m_pHistoryGraph->AddGraph("DelayVarMin", wxColour(100,100,200), 2e6, true, true);
	m_pHistoryGraph->AddGraph("DelayVarMax", wxColour(100,100,200), 2e6, true, true);

	m_pHistoryGraph->ShowBarGraph(false);
	m_pHistoryGraph->SetRightAxisWidth(100);

	m_pHistoryGraph->SetGraphUnits("Offset", "ms");
	m_pHistoryGraph->SetGraphUnits("OffsetLR", "ms");
	m_pHistoryGraph->SetGraphUnits("Delay", "ms");
	m_pHistoryGraph->SetGraphUnits("DelayLR", "ms");

	m_pHistogram->AddGraph("Offset", wxColour(100,255,100), 0.001);
	m_pHistogram->AddGraph("Delay", wxColour(255,100,100), 0.001);

    ChangeView(m_pBuilder->ReadSetting("Window", "Info"));

	m_plstGraphData->SelectButton(m_pBuilder->ReadSetting("Graph", "Offset"));

	SetSize(size);
	SetPosition(pos);

    Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&ptpPanel::OnLeftUp);
	m_pHistoryGraph->Bind(wxEVT_LEFT_UP, &ptpPanel::OnLeftUp, this);\
    ConnectLeftUp(this);

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
    m_pClock = wxPtp::Get().GetPtpClock(m_nDomain, m_sSelectedClock.AfterFirst('\n'));
    ShowClockDetails();
}


void ptpPanel::OnClockAdded(wxCommandEvent& event)
{
    AddClock(event.GetString());
}

void ptpPanel::OnClockUpdated(wxCommandEvent& event)
{
    wxString sClock(m_dbMac.GetVendor(event.GetString())+event.GetString());
    if(sClock == m_sSelectedClock)
    {
        ShowClockDetails();
    }
}

void ptpPanel::OnClockRemoved(wxCommandEvent& event)
{
    wxString sClock(m_dbMac.GetVendor(event.GetString())+event.GetString());
    if(m_sSelectedClock == sClock)
    {
        ClearClockDetails();
    }
    m_plstClocks->DeleteButton(m_plstClocks->FindButton(sClock));
}

void ptpPanel::OnClockTime(wxCommandEvent& event)
{
        ShowTime();

}

void ptpPanel::OnClockMaster(wxCommandEvent& event)
{
    wxString sClock(m_dbMac.GetVendor(event.GetString())+event.GetString());
    if(m_sSelectedClock == sClock)
    {
        m_pswp->ChangeSelection("Master");
        m_plblState->SetLabel("Master");
    }


    size_t nButton = m_plstClocks->FindButton(sClock);
    if(nButton != wmList::NOT_FOUND)
    {
        m_plstClocks->SetButtonColour(nButton, CLR_MASTER);
        m_plstClocks->SetSelectedButtonColour(nButton, CLR_MASTER_SELECTED);
    }
}

void ptpPanel::OnClockSlave(wxCommandEvent& event)
{
    wxString sClock(m_dbMac.GetVendor(event.GetString())+event.GetString());

    if(m_sSelectedClock == sClock)
    {
        m_pswp->ChangeSelection("Slave");
        m_plblState->SetLabel("Slave");
    }
    size_t nButton = m_plstClocks->FindButton(sClock);
    if(nButton != wmList::NOT_FOUND)
    {
        m_plstClocks->SetButtonColour(nButton, CLR_SLAVE);
        m_plstClocks->SetSelectedButtonColour(nButton, CLR_SLAVE_SELECTED);
    }
}


void ptpPanel::ShowClockDetails()
{

    if(!m_pClock) return;


    m_pblAddress->SetLabel(wxString(m_pClock->GetIpAddress()));
    m_plblIdentity->SetLabel(wxString(m_pClock->GetClockId()));

    m_ppnlAnnouncements->Show((m_pClock->GetCount(ptpV2Header::ANNOUNCE) != 0));

    m_pswp->ChangeSelection(m_pClock->IsSyncMaster() ? "Master" : "Slave");

    wxColour clrNormal(CLR_SLAVE);
    wxColour clrSelected(CLR_SLAVE_SELECTED);
    if(m_pClock->IsGrandMaster())
    {
        m_plblState->SetLabel("Grand Master");
        clrNormal = CLR_MASTER;
        clrSelected = CLR_MASTER_SELECTED;
    }
    else if(m_pClock->IsSyncMaster())
    {
        m_plblState->SetLabel("Sync Master");
        clrNormal = CLR_SYNC_MASTER;
        clrSelected = CLR_SYNC_MASTER_SELECTED;
    }
    else
    {
        m_plblState->SetLabel("Slave");
    }

    size_t nButton = m_plstClocks->FindButton(m_sSelectedClock);

    if(nButton != wmList::NOT_FOUND)
    {
        m_plstClocks->SetButtonColour(nButton, clrNormal);
        m_plstClocks->SetSelectedButtonColour(nButton, clrSelected);
    }


    if((m_pClock->GetCount(ptpV2Header::ANNOUNCE) != 0))
    {
        auto itAccuracy = m_mAccuracy.find(m_pClock->GetAccuracy());
        if(itAccuracy != m_mAccuracy.end())
        {
            m_plblAccuracy->SetLabel(itAccuracy->second);
        }
        else
        {
            m_plblAccuracy->SetLabel("Unknown");
        }
        m_plblAnnCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(ptpV2Header::ANNOUNCE)));
        m_plblAnnRate->SetLabel(ConvertRate(m_pClock->GetInterval(ptpV2Header::ANNOUNCE)));

        m_plblClass->SetLabel(wxString::Format("%u", static_cast<unsigned int>(m_pClock->GetClass())));

        auto itSource = m_mTimeSource.find(m_pClock->GetTimeSource());
        if(itSource != m_mTimeSource.end())
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
        m_plblSyncCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(ptpV2Header::SYNC)));
        m_plblSyncRate->SetLabel(ConvertRate(m_pClock->GetInterval(ptpV2Header::SYNC)));

        m_plblFollowCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(ptpV2Header::FOLLOW_UP)));
        m_plblFollowRate->SetLabel(ConvertRate(m_pClock->GetInterval(ptpV2Header::FOLLOW_UP)));

        m_ppnlAnnounceFlags->ShowFlags(m_pClock->GetFlags(ptpV2Header::ANNOUNCE));
        m_ppnlSyncFlags->ShowFlags(m_pClock->GetFlags(ptpV2Header::SYNC));
        m_ppnlFollowFlags->ShowFlags(m_pClock->GetFlags(ptpV2Header::FOLLOW_UP));

        m_plblMasterId->SetLabel(wxPtp::Get().GetMasterClockId(m_nDomain));
    }
    else
    {
        m_plblDelayMech->SetLabel("E-E");

        //m_plblDelayMessage->SetLabel
        m_plblDelayCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(ptpV2Header::DELAY_REQ)));
        m_plblDelayRate->SetLabel(ConvertRate(m_pClock->GetInterval(ptpV2Header::DELAY_REQ)));


        m_plblResponseCount->SetLabel(wxString::Format("%llu", m_pClock->GetCount(ptpV2Header::DELAY_RESP)));
        m_plblResponseRate->SetLabel(ConvertRate(m_pClock->GetInterval(ptpV2Header::DELAY_RESP)));

        m_ppnlRequestFlags->ShowFlags(m_pClock->GetFlags(ptpV2Header::DELAY_REQ));
        m_ppnlResponseFlags->ShowFlags(m_pClock->GetFlags(ptpV2Header::DELAY_RESP));
    }

}

void ptpPanel::ShowTime()
{
    if(m_pLocalClock)
    {
        m_plblTime->SetLabel(wxString(TimeToIsoString(m_pLocalClock->GetPtpTime())));
        m_plblTime->SetBackgroundColour(m_pLocalClock->IsSynced() ? wxColour(150,255,150) : wxColour(255,150,150));

        double offset = TimeToDouble(m_pLocalClock->GetOffset(ptpmonkey::PtpV2Clock::MEAN));


        double slip = (offset-m_offset);
        m_offset = offset;

        //m_plblOffsetAverage->SetLabel(wxString::Format("%lld.%09llu", offset.first.count(), offset.second.count()));
        m_plblOffsetAverage->SetLabel(wxString::Format("%f", m_offset));
        //m_plblDelayAverage->SetLabel(wxString::Format("%lld.%09llu", slip.first.count(), slip.second.count()));

        //time_s_ns rangeOffset = m_pLocalClock->GetOffset(PtpV2Clock::MAX) - m_pLocalClock->GetOffset(PtpV2Clock::MIN);
        //time_s_ns rangeDelay = m_pLocalClock->GetDelay(PtpV2Clock::MAX) - m_pLocalClock->GetDelay(PtpV2Clock::MIN);

        //m_plblOffsetRange->SetLabel(wxString::Format("%llu.%09llu", rangeOffset.first.count(), rangeOffset.second.count()));
        //m_plblDelayRange->SetLabel(wxString::Format("%llu.%09llu", rangeDelay.first.count(), rangeDelay.second.count()));
        auto pSyncMaster = wxPtp::Get().GetSyncMasterClock(m_nDomain);

        if(m_pLocalClock->IsSynced() && pSyncMaster)
        {
            auto dUTCOffset = static_cast<double>(pSyncMaster->GetUtcOffset());
            auto dPeak = (TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::CURRENT))+dUTCOffset)*1000.0;
            m_pHistoryGraph->AddPeak("Offset", dPeak);
            m_pHistogram->AddPeak("Offset", dPeak);

            double m = m_pLocalClock->GetOffsetSlope();
            double c = m_pLocalClock->GetOffsetIntersection();
            auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            double dEstimate = (dUTCOffset+c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()))*1000.0;

            m_pHistoryGraph->SetLine("OffsetLR", (c+dUTCOffset)*1000.0, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dEstimate, std::chrono::system_clock::now());

            dPeak = (TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::MEAN))+dUTCOffset)*1000.0;

            m_pHistoryGraph->SetLine("OffsetMean", dPeak, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dPeak, std::chrono::system_clock::now());
            dPeak -= TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SD))*1000.0;
            m_pHistoryGraph->SetLine("OffsetVarMin", dPeak, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dPeak, std::chrono::system_clock::now());
            dPeak += TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SD))*2000.0;
            m_pHistoryGraph->SetLine("OffsetVarMax", dPeak, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dPeak, std::chrono::system_clock::now());

            auto dDelay = TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::CURRENT))*1000.0;
            m_pHistoryGraph->AddPeak("Delay", dDelay);
            m_pHistogram->AddPeak("Delay", dDelay);

            m = m_pLocalClock->GetDelaySlope();
            c = m_pLocalClock->GetDelayIntersection();
            now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            dEstimate = (c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()))*1000.0;

            m_pHistoryGraph->SetLine("DelayLR", c*1000.0, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dEstimate, std::chrono::system_clock::now());

            dDelay = TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::MEAN))*1000.0;
            m_pHistoryGraph->SetLine("DelayMean", dDelay, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dDelay, std::chrono::system_clock::now());
            dDelay -= TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SD))*1000.0;
            m_pHistoryGraph->SetLine("DelayVarMin", dDelay, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dDelay, std::chrono::system_clock::now());
            dDelay += TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SD))*2000.0;
            m_pHistoryGraph->SetLine("DelayVarMax", dDelay, std::chrono::time_point<std::chrono::system_clock>(m_pLocalClock->GetFirstOffsetTime()), dDelay, std::chrono::system_clock::now());

            UpdateGraphLabels();

        }
    }
}

void ptpPanel::UpdateGraphLabels()
{
    if(m_sGraph == "Offset")
    {
        auto pSyncMaster = wxPtp::Get().GetSyncMasterClock(m_nDomain);
        auto dUTCOffset = static_cast<double>(pSyncMaster->GetUtcOffset());
        auto dCurrent = (TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::CURRENT))+dUTCOffset)*1000.0;
        auto dMean = (TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::MEAN))+dUTCOffset)*1000.0;
        auto dSD = TimeToDouble(m_pLocalClock->GetOffset(PtpV2Clock::SD))*1000.0;
        auto m = m_pLocalClock->GetOffsetSlope();
        auto c = m_pLocalClock->GetOffsetIntersection();
        auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
        auto dEstimate = (c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()))*1000.0;

        m_plblCurrent->SetLabel(wxString::Format("%fus", dCurrent));
        m_plblMean->SetLabel(wxString::Format("%fus", dMean));
        m_plblDeviation->SetLabel(wxString::Format("%fus", dSD));
        m_plblSlope->SetLabel(wxString::Format("%f us/s", m));
        m_plblPrediction->SetLabel(wxString::Format("%f us", dEstimate));
    }
    else
    {
        auto pSyncMaster = wxPtp::Get().GetSyncMasterClock(m_nDomain);
        auto dCurrent = (TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::CURRENT)))*1000.0;
        auto dMean = (TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::MEAN)))*1000.0;
        auto dSD = TimeToDouble(m_pLocalClock->GetDelay(PtpV2Clock::SD))*1000.0;
        auto m = m_pLocalClock->GetDelaySlope();
        auto c = m_pLocalClock->GetDelayIntersection();
        auto now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
        auto dEstimate = (c + m * TimeToDouble(now-m_pLocalClock->GetFirstOffsetTime()))*1000.0;

        m_plblCurrent->SetLabel(wxString::Format("%fus", dCurrent));
        m_plblMean->SetLabel(wxString::Format("%fus", dMean));
        m_plblDeviation->SetLabel(wxString::Format("%fus", dSD));
        m_plblSlope->SetLabel(wxString::Format("%f us/s", m));
        m_plblPrediction->SetLabel(wxString::Format("%f us", dEstimate));
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
    auto itRate = m_mRate.find(nRate);
    if(itRate != m_mRate.end())
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

        wxPtp::Get().RunDomain(Settings::Get().Read("AoIP_Settings", "Interface", "eth0"), m_nDomain);


        for(auto itClock = wxPtp::Get().GetClocksBegin(m_nDomain); itClock != wxPtp::Get().GetClocksEnd(m_nDomain); ++itClock)
        {
            AddClock(wxString(itClock->first));
        }
    }
}

void ptpPanel::AddClock(wxString sClock)
{
    sClock = m_dbMac.GetVendor(sClock)+sClock;

    if(m_plstClocks->FindButton(sClock) == wmList::NOT_FOUND)
    {
        wxColour clrNormal(CLR_SLAVE);
        wxColour clrSelected(CLR_SLAVE_SELECTED);
        if(wxPtp::Get().GetMasterClockId(m_nDomain) == sClock)
        {
            clrNormal = CLR_MASTER;
            clrSelected = CLR_MASTER_SELECTED;
        }
        else
        {
            std::shared_ptr<const ptpmonkey::PtpV2Clock> pSyncMaster = wxPtp::Get().GetSyncMasterClock(m_nDomain);
            if(pSyncMaster && pSyncMaster->GetId() == sClock)
            {
                clrNormal = CLR_SYNC_MASTER;
                clrSelected = CLR_SYNC_MASTER_SELECTED;
            }
        }
        size_t nButton = m_plstClocks->AddButton(sClock, wxNullBitmap, 0, wmList::wmENABLED, clrNormal);
        m_plstClocks->SetSelectedButtonColour(nButton, clrSelected);

    }
    if(m_pLocalClock == nullptr)
    {
        m_pLocalClock = wxPtp::Get().GetLocalClock(m_nDomain);
    }
}

void ptpPanel::OnClockMessage(wxCommandEvent& event)
{
    wxString sClock(m_dbMac.GetVendor(event.GetString())+event.GetString());
    if(sClock == m_sSelectedClock)
    {

        ShowClockDetails();
    }


}

void ptpPanel::ChangeView(const wxString& sWindow)
{
    m_pSwpMain->ChangeSelection(sWindow);
}


void ptpPanel::OnlstDataSelected(wxCommandEvent& event)
{
    m_sGraph = event.GetString();
    m_plstHistogramData->SelectButton(m_sGraph, false);
    m_plstGraphData->SelectButton(m_sGraph, false);

    m_plblGraphTitle->SetLabel("PTP "+m_sGraph);
    m_pHistoryGraph->HideAllGraphs();
    m_pHistoryGraph->ShowGraph(m_sGraph, true);
    m_pHistoryGraph->ShowGraph(m_sGraph+"LR", true);
    m_pHistoryGraph->ShowGraph(m_sGraph+"Mean", true);
    m_pHistoryGraph->ShowGraph(m_sGraph+"VarMin", true);
    m_pHistoryGraph->ShowGraph(m_sGraph+"VarMax", true);
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
    m_pBuilder->Maximize(false);
}

void ptpPanel::OnlstHistogramGranularitySelected(wxCommandEvent& event)
{
    m_pHistogram->ChangeGranularity(m_sGraph, event.GetInt());
}

void ptpPanel::OnlstHistogramResolutionSelected(wxCommandEvent& event)
{
    m_pHistogram->ChangeResolution(m_sGraph, event.GetInt());
}
