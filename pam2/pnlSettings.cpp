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
#include "dlgSequence.h"
#include "iomanager.h"

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
#include "soundcardmanager.h"


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
const long pnlSettings::ID_M_PBTN3 = wxNewId();
const long pnlSettings::ID_M_PLST6 = wxNewId();
const long pnlSettings::ID_M_PLST4 = wxNewId();
const long pnlSettings::ID_M_PLBL5 = wxNewId();
const long pnlSettings::ID_PANEL2 = wxNewId();
const long pnlSettings::ID_M_PLBL2 = wxNewId();
const long pnlSettings::ID_M_PLST8 = wxNewId();
const long pnlSettings::ID_M_PLBL6 = wxNewId();
const long pnlSettings::ID_M_PLBL8 = wxNewId();
const long pnlSettings::ID_M_PLST3 = wxNewId();
const long pnlSettings::ID_M_PLST10 = wxNewId();
const long pnlSettings::ID_M_PLBL10 = wxNewId();
const long pnlSettings::ID_M_PLBL11 = wxNewId();
const long pnlSettings::ID_PANEL11 = wxNewId();
const long pnlSettings::ID_M_PLST7 = wxNewId();
const long pnlSettings::ID_M_PBTN6 = wxNewId();
const long pnlSettings::ID_M_PBTN7 = wxNewId();
const long pnlSettings::ID_M_PBTN8 = wxNewId();
const long pnlSettings::ID_M_PBTN9 = wxNewId();
const long pnlSettings::ID_PANEL9 = wxNewId();
const long pnlSettings::ID_STATICBOX2 = wxNewId();
const long pnlSettings::ID_STATICBOX1 = wxNewId();
const long pnlSettings::ID_SLIDER = wxNewId();
const long pnlSettings::ID_M_PLBL7 = wxNewId();
const long pnlSettings::ID_M_PBTN12 = wxNewId();
const long pnlSettings::ID_M_PBTN11 = wxNewId();
const long pnlSettings::ID_M_PBTN10 = wxNewId();
const long pnlSettings::ID_M_PBTN13 = wxNewId();
const long pnlSettings::ID_M_PBTN14 = wxNewId();
const long pnlSettings::ID_M_PBTN15 = wxNewId();
const long pnlSettings::ID_M_PBTN16 = wxNewId();
const long pnlSettings::ID_M_PBTN17 = wxNewId();
const long pnlSettings::ID_M_PBTN18 = wxNewId();
const long pnlSettings::ID_CUSTOM1 = wxNewId();
const long pnlSettings::ID_M_PLBL9 = wxNewId();
const long pnlSettings::ID_M_PBTN19 = wxNewId();
const long pnlSettings::ID_M_PLST9 = wxNewId();
const long pnlSettings::ID_PANEL10 = wxNewId();
const long pnlSettings::ID_M_PLST5 = wxNewId();
const long pnlSettings::ID_STATICBOX3 = wxNewId();
const long pnlSettings::ID_CUSTOM2 = wxNewId();
const long pnlSettings::ID_M_PLBL1 = wxNewId();
const long pnlSettings::ID_M_PBTN21 = wxNewId();
const long pnlSettings::ID_PANEL12 = wxNewId();
const long pnlSettings::ID_M_PSWP2 = wxNewId();
const long pnlSettings::ID_M_PBTN20 = wxNewId();
const long pnlSettings::ID_PANEL8 = wxNewId();
const long pnlSettings::ID_PANEL4 = wxNewId();
const long pnlSettings::ID_PANEL5 = wxNewId();
const long pnlSettings::ID_PANEL3 = wxNewId();
const long pnlSettings::ID_PANEL7 = wxNewId();
const long pnlSettings::ID_M_PBTN22 = wxNewId();
const long pnlSettings::ID_M_PBTN23 = wxNewId();
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
    Create(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("wxID_ANY"));
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
    m_pLbl3 = new wmLabel(pnlInput, ID_M_PLBL3, _("Audio Monitoring Source"), wxPoint(0,5), wxSize(600,30), 0, _T("ID_M_PLBL3"));
    m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl3->SetForegroundColour(wxColour(255,255,255));
    m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
    wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl3->SetFont(m_pLbl3Font);
    m_plstDevices = new wmList(pnlInput, ID_M_PLST1, wxPoint(0,70), wxSize(600,315), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
    m_plstDevices->SetBackgroundColour(wxColour(0,0,0));
    m_plstDevices->SetSelectedButtonColour(wxColour(wxT("#008000")));
    m_plstDevices->SetDisabledColour(wxColour(wxT("#808080")));
    m_plstInput = new wmList(pnlInput, ID_M_PLST2, wxPoint(0,36), wxSize(600,34), wmList::STYLE_SELECT, 0, wxSize(100,30), 3, wxSize(-1,-1));
    m_plstInput->SetButtonColour(wxColour(wxT("#400080")));
    m_plstInput->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_pbtnHome = new wmButton(pnlInput, ID_M_PBTN1, wxEmptyString, wxPoint(70,395), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
    m_pbtnHome->SetColourDisabled(wxColour(wxT("#808080")));
    m_pbtnPrevious = new wmButton(pnlInput, ID_M_PBTN2, wxEmptyString, wxPoint(190,395), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
    m_pbtnPrevious->SetColourDisabled(wxColour(wxT("#808080")));
    m_pbtnNext = new wmButton(pnlInput, ID_M_PBTN4, wxEmptyString, wxPoint(310,395), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
    m_pbtnNext->SetColourDisabled(wxColour(wxT("#808080")));
    m_pbtnEnd = new wmButton(pnlInput, ID_M_PBTN5, wxEmptyString, wxPoint(430,395), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN5"));
    m_pbtnEnd->SetColourDisabled(wxColour(wxT("#808080")));
    pnlOutput = new wxPanel(m_pswpSettings, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    pnlOutput->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl4 = new wmLabel(pnlOutput, ID_M_PLBL4, _("Audio Output Device"), wxPoint(0,5), wxSize(600,30), 0, _T("ID_M_PLBL4"));
    m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl4->SetForegroundColour(wxColour(255,255,255));
    m_pLbl4->SetBackgroundColour(wxColour(0,64,0));
    wxFont m_pLbl4Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl4->SetFont(m_pLbl4Font);
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
    pnlGenerator = new wxPanel(m_pswpSettings, ID_PANEL8, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL8"));
    pnlGenerator->SetBackgroundColour(wxColour(0,0,0));
    m_pLbl2 = new wmLabel(pnlGenerator, ID_M_PLBL2, _("Audio Output Generation"), wxPoint(0,5), wxSize(600,30), 0, _T("ID_M_PLBL2"));
    m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl2->SetForegroundColour(wxColour(255,255,255));
    m_pLbl2->SetBackgroundColour(wxColour(0,64,0));
    wxFont m_pLbl2Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl2->SetFont(m_pLbl2Font);
    m_plstAudioSources = new wmList(pnlGenerator, ID_M_PLST8, wxPoint(0,36), wxSize(480,34), wmList::STYLE_SELECT, 0, wxSize(-1,30), 5, wxSize(2,-1));
    m_plstAudioSources->SetBackgroundColour(wxColour(0,0,0));
    m_plstAudioSources->SetButtonColour(wxColour(wxT("#400080")));
    m_plstAudioSources->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
    m_pswpAog = new wmSwitcherPanel(pnlGenerator, ID_M_PSWP2, wxPoint(0,70), wxSize(600,380), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
    m_pswpAog->SetPageNameStyle(0);
    Panel5 = new wxPanel(m_pswpAog, ID_PANEL11, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL11"));
    Panel5->SetBackgroundColour(wxColour(0,0,0));
    m_plblInput = new wmLabel(Panel5, ID_M_PLBL6, _("Audio output taken from incoming audio siganl.\nSee Audio Input page"), wxPoint(0,0), wxSize(600,100), 0, _T("ID_M_PLBL6"));
    m_plblInput->SetBorderState(uiRect::BORDER_NONE);
    m_plblInput->SetForegroundColour(wxColour(128,255,0));
    m_plblInput->SetBackgroundColour(wxColour(0,0,0));
    wxFont m_plblInputFont(14,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_plblInput->SetFont(m_plblInputFont);
    m_pLbl1 = new wmLabel(Panel5, ID_M_PLBL8, _("Input-Output Channel Mapping"), wxPoint(0,100), wxSize(600,30), 0, _T("ID_M_PLBL8"));
    m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl1->SetForegroundColour(wxColour(255,255,255));
    m_pLbl1->SetBackgroundColour(wxColour(0,64,0));
    wxFont m_pLbl1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_pLbl1->SetFont(m_pLbl1Font);
    m_plstOutputLeft = new wmList(Panel5, ID_M_PLST3, wxPoint(60,140), wxSize(540,44), wmList::STYLE_SELECT, 0, wxSize(-1,30), 8, wxSize(2,-1));
    m_plstOutputLeft->SetBackgroundColour(wxColour(0,0,0));
    m_plstOutputLeft->SetButtonColour(wxColour(wxT("#A0A0A0")));
    m_plstOutputLeft->SetSelectedButtonColour(wxColour(wxT("#008000")));
    m_plstOutputRight = new wmList(Panel5, ID_M_PLST10, wxPoint(60,200), wxSize(540,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 8, wxSize(2,-1));
    m_plstOutputRight->SetBackgroundColour(wxColour(0,0,0));
    m_plstOutputRight->SetButtonColour(wxColour(wxT("#A0A0A0")));
    m_plstOutputRight->SetSelectedButtonColour(wxColour(wxT("#008000")));
    m_pLbl6 = new wmLabel(Panel5, ID_M_PLBL10, _("LEFT:"), wxPoint(0,140), wxSize(60,44), 0, _T("ID_M_PLBL10"));
    m_pLbl6->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl6->SetForegroundColour(wxColour(255,255,255));
    m_pLbl6->SetBackgroundColour(wxColour(0,0,0));
    wxFont m_pLbl6Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
    m_pLbl6->SetFont(m_pLbl6Font);
    m_pLbl7 = new wmLabel(Panel5, ID_M_PLBL11, _("RIGHT:"), wxPoint(0,200), wxSize(60,44), 0, _T("ID_M_PLBL11"));
    m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
    m_pLbl7->SetForegroundColour(wxColour(255,255,255));
    m_pLbl7->SetBackgroundColour(wxColour(0,0,0));
    wxFont m_pLbl7Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
    m_pLbl7->SetFont(m_pLbl7Font);
    Panel3 = new wxPanel(m_pswpAog, ID_PANEL9, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL9"));
    Panel3->SetBackgroundColour(wxColour(0,0,0));
    m_plstAogFiles = new wmList(Panel3, ID_M_PLST7, wxPoint(0,0), wxSize(600,315), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
    m_plstAogFiles->SetBackgroundColour(wxColour(0,0,0));
    m_plstAogFiles->SetSelectedButtonColour(wxColour(wxT("#008000")));
    m_pbtnAogHome = new wmButton(Panel3, ID_M_PBTN6, wxEmptyString, wxPoint(70,325), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
    m_pbtnAogHome->SetColourDisabled(wxColour(wxT("#808080")));
    m_pbtnAogPrev = new wmButton(Panel3, ID_M_PBTN7, wxEmptyString, wxPoint(190,325), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
    m_pbtnAogPrev->SetColourDisabled(wxColour(wxT("#808080")));
    m_pbtnAogNext = new wmButton(Panel3, ID_M_PBTN8, wxEmptyString, wxPoint(310,325), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
    m_pbtnAogNext->SetColourDisabled(wxColour(wxT("#808080")));
    m_pbtnAogEnd = new wmButton(Panel3, ID_M_PBTN9, wxEmptyString, wxPoint(430,325), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN9"));
    m_pbtnAogEnd->SetColourDisabled(wxColour(wxT("#808080")));
    Panel4 = new wxPanel(m_pswpAog, ID_PANEL10, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL10"));
    Panel4->SetBackgroundColour(wxColour(0,0,0));
    wxFont Panel4Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    Panel4->SetFont(Panel4Font);
    StaticBox2 = new wxStaticBox(Panel4, ID_STATICBOX2, _("Amplitude"), wxPoint(5,210), wxSize(590,105), 0, _T("ID_STATICBOX2"));
    StaticBox2->SetForegroundColour(wxColour(255,255,255));
    StaticBox2->SetBackgroundColour(wxColour(0,0,0));
    StaticBox1 = new wxStaticBox(Panel4, ID_STATICBOX1, _("Frequency"), wxPoint(5,5), wxSize(590,200), 0, _T("ID_STATICBOX1"));
    StaticBox1->SetForegroundColour(wxColour(255,255,255));
    m_pSlider = new wmSlider(Panel4,ID_SLIDER,wxT("Frequency"),wxPoint(10,30),wxSize(580,50),5,_T("ID_SLIDER"));
    m_plblFrequency = new wmLabel(Panel4, ID_M_PLBL7, _("Hz"), wxPoint(200,90), wxSize(200,50), 0, _T("ID_M_PLBL7"));
    m_plblFrequency->SetBorderState(uiRect::BORDER_FLAT);
    m_plblFrequency->SetForegroundColour(wxColour(0,0,0));
    m_plblFrequency->SetBackgroundColour(wxColour(255,255,255));
    wxFont m_plblFrequencyFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    m_plblFrequency->SetFont(m_plblFrequencyFont);
    m_pbtnMinus10 = new wmButton(Panel4, ID_M_PBTN12, _("-10 Hz"), wxPoint(30,90), wxSize(80,50), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN12"));
    m_pbtnMinus1 = new wmButton(Panel4, ID_M_PBTN11, _("-1 Hz"), wxPoint(115,90), wxSize(80,50), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN11"));
    m_pbtnPlus1 = new wmButton(Panel4, ID_M_PBTN10, _("+1 Hz"), wxPoint(405,90), wxSize(80,50), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN10"));
    m_pbtnPlus10 = new wmButton(Panel4, ID_M_PBTN13, _("+10 Hz"), wxPoint(490,90), wxSize(80,50), wmButton::STYLE_REPEAT, wxDefaultValidator, _T("ID_M_PBTN13"));
    m_pbtn450 = new wmButton(Panel4, ID_M_PBTN14, _("450 Hz"), wxPoint(115,145), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN14"));
    m_pbtn900 = new wmButton(Panel4, ID_M_PBTN15, _("900 Hz"), wxPoint(200,145), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN15"));
    m_pbtn1000 = new wmButton(Panel4, ID_M_PBTN16, _("1 kHz"), wxPoint(285,145), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN16"));
    m_pbtn5000 = new wmButton(Panel4, ID_M_PBTN17, _("5 kHz"), wxPoint(370,145), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN17"));
    m_pbtn10000 = new wmButton(Panel4, ID_M_PBTN18, _("10 kHz"), wxPoint(455,145), wxSize(80,50), 0, wxDefaultValidator, _T("ID_M_PBTN18"));
    m_pAmplitude = new wmSlider(Panel4,ID_CUSTOM1,wxT("Frequency"),wxPoint(10,250),wxSize(400,40),5,_T("ID_CUSTOM1"));
    m_plbldB = new wmLabel(Panel4, ID_M_PLBL9, _("dB"), wxPoint(415,250), wxSize(60,40), 0, _T("ID_M_PLBL9"));
    m_plbldB->SetBorderState(uiRect::BORDER_NONE);
    m_plbldB->SetForegroundColour(wxColour(0,0,0));
    m_plbldB->SetBackgroundColour(wxColour(255,255,255));
    m_pbtn0dbu = new wmButton(Panel4, ID_M_PBTN19, _("-18 dBFS"), wxPoint(480,250), wxSize(80,40), 0, wxDefaultValidator, _T("ID_M_PBTN19"));
    m_plstShape = new wmList(Panel4, ID_M_PLST9, wxPoint(150,320), wxSize(300,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 4, wxSize(5,1));
    m_plstShape->SetBackgroundColour(wxColour(0,0,0));
    Panel2 = new wxPanel(m_pswpAog, ID_PANEL12, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL12"));
    Panel2->SetBackgroundColour(wxColour(0,0,0));
    m_plstColour = new wmList(Panel2, ID_M_PLST5, wxPoint(10,10), wxSize(580,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 4, wxSize(5,1));
    m_plstColour->SetBackgroundColour(wxColour(0,0,0));
    StaticBox3 = new wxStaticBox(Panel2, ID_STATICBOX3, _("Amplitude"), wxPoint(5,210), wxSize(590,105), 0, _T("ID_STATICBOX3"));
    StaticBox3->SetForegroundColour(wxColour(255,255,255));
    StaticBox3->SetBackgroundColour(wxColour(0,0,0));
    m_pNoiseAmplitude = new wmSlider(Panel2,ID_CUSTOM2,wxT("Frequency"),wxPoint(10,250),wxSize(400,40),5,_T("ID_CUSTOM2"));
    m_plblNoisedB = new wmLabel(Panel2, ID_M_PLBL1, _("dB"), wxPoint(415,250), wxSize(60,40), 0, _T("ID_M_PLBL1"));
    m_plblNoisedB->SetBorderState(uiRect::BORDER_NONE);
    m_plblNoisedB->SetForegroundColour(wxColour(0,0,0));
    m_plblNoisedB->SetBackgroundColour(wxColour(255,255,255));
    m_pbtnNoise0dBu = new wmButton(Panel2, ID_M_PBTN21, _("-18 dBFS"), wxPoint(480,250), wxSize(80,40), 0, wxDefaultValidator, _T("ID_M_PBTN21"));
    m_pswpAog->AddPage(Panel5, _("Input"), false);
    m_pswpAog->AddPage(Panel3, _("Files"), false);
    m_pswpAog->AddPage(Panel4, _("Generator"), false);
    m_pswpAog->AddPage(Panel2, _("Noise"), false);
    m_pbtnSequences = new wmButton(pnlGenerator, ID_M_PBTN20, _("Edit Sequences"), wxPoint(485,39), wxSize(105,30), 0, wxDefaultValidator, _T("ID_M_PBTN20"));
    m_pbtnSequences->Hide();
    pnlSettingsRTP = new pnlRTP(m_pswpSettings, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    pnlSettingsNetwork = new pnlNetworkSetup(m_pswpSettings, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    m_ppnlPlugins = new pnlSettingsPlugins(m_pswpSettings, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    Panel1 = new pnlUpdate(m_pswpSettings, ID_PANEL7, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL7"));
    pnlGeneral = new wxPanel(m_pswpSettings, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    pnlGeneral->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BACKGROUND));
    m_pbtnCursor = new wmButton(pnlGeneral, ID_M_PBTN22, _("Cursor"), wxPoint(10,10), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN22"));
    m_ptbnOptions = new wmButton(pnlGeneral, ID_M_PBTN23, _("View"), wxPoint(10,60), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN23"));
    m_pswpSettings->AddPage(pnlInput, _("Monitor"), false);
    m_pswpSettings->AddPage(pnlOutput, _("Output Device"), false);
    m_pswpSettings->AddPage(pnlGenerator, _("Output Source"), false);
    m_pswpSettings->AddPage(pnlSettingsRTP, _("AoIP"), false);
    m_pswpSettings->AddPage(pnlSettingsNetwork, _("Network"), false);
    m_pswpSettings->AddPage(m_ppnlPlugins, _("Plugins"), false);
    m_pswpSettings->AddPage(Panel1, _("Update"), false);
    m_pswpSettings->AddPage(pnlGeneral, _("General"), false);

    Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstDevicesSelected);
    Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstInputSelected);
    Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnHomeClick);
    Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnPreviousClick);
    Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnNextClick);
    Connect(ID_M_PBTN5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnEndClick);
    Connect(ID_M_PBTN3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnOutputClick);
    Connect(ID_M_PLST6,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlblLatencySelected);
    Connect(ID_M_PLST4,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstPlaybackSelected);
    Connect(ID_M_PLST8,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstAudioSourcesSelected);
    Connect(ID_M_PLST3,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstOutputLeftSelected);
    Connect(ID_M_PLST10,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstOutputRightSelected);
    Connect(ID_M_PLST7,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstAogFilesSelected);
    Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnAogHomeClick);
    Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnAogPrevClick);
    Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnAogNextClick);
    Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnAogEndClick);
    Connect(ID_M_PBTN12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnMinus10Click);
    Connect(ID_M_PBTN11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnMinus1Click);
    Connect(ID_M_PBTN10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnPlus1Click);
    Connect(ID_M_PBTN13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnPlus10Click);
    Connect(ID_M_PBTN14,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::Onbtn450Click);
    Connect(ID_M_PBTN15,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::Onbtn900Click);
    Connect(ID_M_PBTN16,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::Onbtn1000Click);
    Connect(ID_M_PBTN17,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::Onbtn5000Click);
    Connect(ID_M_PBTN18,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::Onbtn10000Click);
    Connect(ID_M_PBTN19,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::Onbtn0dbuClick);
    Connect(ID_M_PLST9,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstShapeSelected);
    Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettings::OnlstColourSelected);
    Connect(ID_M_PBTN21,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnNoise0dBuClick);
    Connect(ID_M_PBTN20,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnSequencesClick);
    Connect(ID_M_PBTN22,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnCursorClick);
    Connect(ID_M_PBTN23,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettings::OnbtnOptionsClick);
    //*)

    m_pbtnCursor->SetToggleLook(true, wxT("Hide"), wxT("Show"), 40);
    m_ptbnOptions->SetToggleLook(true, wxT("Screens"), wxT("Options"), 40);

    m_pbtnCursor->ToggleSelection((Settings::Get().Read(wxT("General"), wxT("Cursor"), 1) == 1), true);
    m_ptbnOptions->ToggleSelection((Settings::Get().Read(wxT("General"), wxT("ShowOptions"), 1) == 1), true);

    m_pbtnEnd->SetBitmapLabel(wxBitmap(end_hz_xpm));
    m_pbtnEnd->SetBitmapSelected(wxBitmap(end_hz_press_xpm));
    m_pbtnHome->SetBitmapLabel(wxBitmap(home_hz_xpm));
    m_pbtnHome->SetBitmapSelected(wxBitmap(home_hz_press_xpm));
    m_pbtnPrevious->SetBitmapLabel(wxBitmap(pageup_xpm));
    m_pbtnPrevious->SetBitmapSelected(wxBitmap(pageup_press_xpm));
    m_pbtnNext->SetBitmapLabel(wxBitmap(pagedown_xpm));
    m_pbtnNext->SetBitmapSelected(wxBitmap(pagedown_press_xpm));

    m_pbtnAogEnd->SetBitmapLabel(wxBitmap(end_hz_xpm));
    m_pbtnAogEnd->SetBitmapSelected(wxBitmap(end_hz_press_xpm));
    m_pbtnAogHome->SetBitmapLabel(wxBitmap(home_hz_xpm));
    m_pbtnAogHome->SetBitmapSelected(wxBitmap(home_hz_press_xpm));
    m_pbtnAogPrev->SetBitmapLabel(wxBitmap(pageup_xpm));
    m_pbtnAogPrev->SetBitmapSelected(wxBitmap(pageup_press_xpm));
    m_pbtnAogNext->SetBitmapLabel(wxBitmap(pagedown_xpm));
    m_pbtnAogNext->SetBitmapSelected(wxBitmap(pagedown_press_xpm));


    m_pswpSettings->Connect(wxID_ANY, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, (wxObjectEventFunction)&pnlSettings::OnswpSettingsPageChanged, NULL, this);
    SetSize(size);
    SetPosition(pos);


    for(unsigned int i = 0; i < 10; i++)
    {
        m_plstLatency->AddButton(wxString::Format(wxT("%u ms"), i*40));
    }

    m_plstInput->Freeze();
    m_plstInput->AddButton(wxT("Soundcard"));
    m_plstInput->AddButton(wxT("AoIP"));
    m_plstInput->AddButton(wxT("Output"));

    m_plstInput->Thaw();

    m_pbtnOutput->SetToggleLook(true, wxT("OFF"), wxT("ON"),50.0);

    ShowSoundcardOutputs();

    m_plstPlayback->Disable();



    m_plstInput->SelectButton(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")), true);
    m_pbtnOutput->ToggleSelection(Settings::Get().Read(wxT("Output"), wxT("Enabled"), 1), true);

    ShowRTPDefined();


    m_plstLatency->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Latency"), 0)/40, false);

    m_plblSettings->SetLabel(wxString::Format(wxT("Settings: %ld.%ld.%ld.%ld"), AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::BUILD, AutoVersion::REVISION));

    m_plstAudioSources->AddButton(wxT("Input"));
    m_plstAudioSources->AddButton(wxT("File"));
    m_plstAudioSources->AddButton(wxT("Sequence"));
    m_plstAudioSources->AddButton(wxT("Generator"));
    m_plstAudioSources->AddButton(wxT("Noise"));
    m_plstAudioSources->Enable((Settings::Get().Read(wxT("Output"), wxT("Enabled"), 1) == 1));
    if((Settings::Get().Read(wxT("Output"), wxT("Enabled"), 1) != 1))
    {
        m_plblInput->SetLabel(wxT("Audio output disabled"));
        m_pswpAog->ChangeSelection(wxT("Input"));
    }
    else
    {
        m_plblInput->SetLabel(wxT("Audio output taken from incoming audio siganl.\nSee Audio Input page"));
        m_plstAudioSources->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")), true);
    }

    m_pSlider->Init(0,20*log10(22000), 20*log10(Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000)));
    m_pSlider->SetSliderColour(wxColour(128,128,128));
    m_pSlider->SetButtonColour(wxColour(100,100,255));
    m_plblFrequency->SetLabel(wxString::Format(wxT("%.0f Hz"), pow(10,(m_pSlider->GetPosition()/20.0))));

    m_pAmplitude->Init(0,80, 80+Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0));
    m_pAmplitude->SetSliderColour(wxColour(128,128,128));
    m_pAmplitude->SetButtonColour(wxColour(100,255,100));
    m_plbldB->SetLabel(wxString::Format(wxT("%.1f dBFS"),m_pAmplitude->GetPosition()-80.0));

    m_plstShape->AddButton(wxT("Sine"));
    m_plstShape->AddButton(wxT("Square"));
    m_plstShape->AddButton(wxT("Sawtooth"));
    m_plstShape->AddButton(wxT("Triangle"));

    m_plstShape->SelectButton(Settings::Get().Read(wxT("Generator"), wxT("Shape"), 0), true);

    m_plstColour->AddButton(wxT("White"));
    m_plstColour->AddButton(wxT("Pink"));
    m_plstColour->SelectButton(Settings::Get().Read(wxT("Noise"), wxT("Colour"), 0));

    m_pNoiseAmplitude->Init(0,80, 80+Settings::Get().Read(wxT("Noise"), wxT("Amplitude"), -18.0));
    m_pNoiseAmplitude->SetSliderColour(wxColour(128,128,128));
    m_pNoiseAmplitude->SetButtonColour(wxColour(100,255,100));
    m_plblNoisedB->SetLabel(wxString::Format(wxT("%.1f dBFS"),m_pNoiseAmplitude->GetPosition()-80.0));

    Connect(m_pSlider->GetId(), wxEVT_SLIDER_MOVE, (wxObjectEventFunction)&pnlSettings::OnSliderMove);
    Connect(m_pAmplitude->GetId(), wxEVT_SLIDER_MOVE, (wxObjectEventFunction)&pnlSettings::OnAmplitudeMove);
    Connect(m_pNoiseAmplitude->GetId(), wxEVT_SLIDER_MOVE, (wxObjectEventFunction)&pnlSettings::OnNoiseAmplitudeMove);



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

        ShowSoundcardOutputs();

    }
    else if(sDevice == wxT("RTP"))
    {
        Settings::Get().Write(wxT("Input"), wxT("RTP"), event.GetString());
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
    m_plstAudioSources->Enable(event.IsChecked());
    if(event.IsChecked() == false)
    {
        m_plblInput->SetLabel(wxT("Audio output disabled"));
        m_pswpAog->ChangeSelection(wxT("Input"));
        m_plstOutputLeft->Clear();
        m_plstOutputRight->Clear();
    }
    else
    {
        m_plblInput->SetLabel(wxT("Audio output taken from incoming audio siganl.\nSee Audio Input page"));
        m_plstAudioSources->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")), true);
    }

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

    Pa_Initialize();
    int nDevices =  Pa_GetDeviceCount();
    if(nDevices < 0)
    {
        wxLogDebug(wxString::FromAscii(Pa_GetErrorText(nDevices)));
    }

    for(int i = 0; i < nDevices; i++)
    {
        const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(i);
        if(pInfo && pInfo->maxInputChannels > 0)
        {
            short nEnabled = wmList::wmENABLED;
            #ifdef __WXGTK__
            if((Settings::Get().Read(wxT("Output"), wxT("Enabled"),1) == 1) && Settings::Get().Read(wxT("Output"), wxT("Device"), 0) == i)
            {
                nEnabled = wmList::wmDISABLED;
            }
            #endif // __WXGTK__

            m_plstDevices->AddButton(wxString::Format(wxT("[%d] %s [%d]"),i, wxString::FromAscii(pInfo->name).c_str(), pInfo->maxInputChannels), wxNullBitmap, (void*)i, nEnabled);
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

    Pa_Terminate();
}


void pnlSettings::ShowSoundcardOutputs()
{
    Pa_Initialize();
    m_plstPlayback->Clear();

    int nDevices =  Pa_GetDeviceCount();
    if(nDevices < 0)
    {
        wxLogDebug(wxString::FromAscii(Pa_GetErrorText(nDevices)));
    }
    for(int i = 0; i < nDevices; i++)
    {
        const PaDeviceInfo* pInfo = Pa_GetDeviceInfo(i);
        if(pInfo && pInfo->maxOutputChannels > 0)
        {
            short nEnabled = wmList::wmENABLED;
            #ifdef __WXGTK__
            if(Settings::Get().Read(wxT("Input"), wxT("Type"), wxT("Soundcard")) == wxT("Soundcard") && Settings::Get().Read(wxT("Input"), wxT("Device"),0) == i)
            {
                nEnabled = wmList::wmDISABLED;
            }
            #endif // __WXGTK__
            m_plstPlayback->AddButton(wxString::Format(wxT("[%d]%s [%d]"), i, wxString::FromAscii(pInfo->name).c_str(), pInfo->maxOutputChannels), wxNullBitmap, (void*)i, nEnabled);
        }
    }

    unsigned int nOutput(Settings::Get().Read(wxT("Output"), wxT("Device"), (int)Pa_GetDefaultOutputDevice()));
    int nDevice = m_plstPlayback->FindButton(reinterpret_cast<void*>(nOutput));
    if(nDevice == wmList::NOT_FOUND)
    {
        nDevice = 0;
    }
    m_plstPlayback->SelectButton(nDevice);


    Pa_Terminate();
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
    m_plstAogFiles->Freeze();
    m_plstAogFiles->Clear();


    wxArrayString asFiles;
    wxDir::GetAllFiles(Settings::Get().GetWavDirectory(), &asFiles, wxT("*.wav"), wxDIR_FILES);

    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fn(asFiles[i]);
        m_plstAogFiles->AddButton(fn.GetName(), wxNullBitmap, (void*)i);
    }
    m_plstAogFiles->Thaw();

    ShowAogPagingButtons();

    m_plstAogFiles->SelectButton(Settings::Get().Read(wxT("Output"), wxT("File"), wxEmptyString));

}

void pnlSettings::ShowSequences()
{
    m_plstAogFiles->Freeze();
    m_plstAogFiles->Clear();


    wxArrayString asFiles;
    wxDir::GetAllFiles(wxString::Format(wxT("%s/generator"), Settings::Get().GetDocumentDirectory().c_str()), &asFiles, wxT("*.xml"), wxDIR_FILES);

    for(size_t i = 0; i < asFiles.GetCount(); i++)
    {
        wxFileName fn(asFiles[i]);
        m_plstAogFiles->AddButton(fn.GetName(), wxNullBitmap, (void*)i);
    }
    m_plstAogFiles->Thaw();

    ShowAogPagingButtons();

    m_plstAogFiles->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Sequence"), wxEmptyString));

}

void pnlSettings::ShowPagingButtons()
{
    m_pbtnEnd->Show(m_plstDevices->GetPageCount() > 1);
    m_pbtnHome->Show(m_plstDevices->GetPageCount() > 1);
    m_pbtnPrevious->Show(m_plstDevices->GetPageCount() > 1);
    m_pbtnNext->Show(m_plstDevices->GetPageCount() > 1);
}

void pnlSettings::ShowAogPagingButtons()
{
    m_pbtnAogEnd->Show(m_plstAogFiles->GetPageCount() > 1);
    m_pbtnAogHome->Show(m_plstAogFiles->GetPageCount() > 1);
    m_pbtnAogPrev->Show(m_plstAogFiles->GetPageCount() > 1);
    m_pbtnAogNext->Show(m_plstAogFiles->GetPageCount() > 1);
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


void pnlSettings::OnbtnMeterShadingClick(wxCommandEvent& event)
{
    //Settings::Get()->Write(wxT("Meters/shading"), event.IsChecked());
}

void pnlSettings::OnlblLatencySelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Latency"), event.GetInt()*40);
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
    else if(sType == wxT("Output"))
    {
        m_plstDevices->Freeze();
        m_plstDevices->Clear();
        m_plstDevices->Thaw();
        ShowPagingButtons();
    }

}

void pnlSettings::OnswpSettingsPageChanged(wxNotebookEvent& event)
{
    if(m_pswpSettings->GetPageText(m_pswpSettings->GetSelection()) == wxT("Audio Input"))
    {
       RefreshInputs();
    }
    else if(m_pswpSettings->GetPageText(m_pswpSettings->GetSelection()) == wxT("Audio Output"))
    {
        ShowSoundcardOutputs();
    }
}

void pnlSettings::OnbtnHomeClick(wxCommandEvent& event)
{
    m_plstDevices->ShowFirstPage(false,false);
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
    m_plstDevices->ShowLastPage(false, false);
}

void pnlSettings::OnlstAudioSourcesSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Source"), event.GetString());
    if(event.GetString() == wxT("Input"))
    {
        m_pswpAog->ChangeSelection(wxT("Input"));
        PopulateChannelList(m_plstOutputLeft, Settings::Get().Read(wxT("Output"), wxT("Left"), 0));
        PopulateChannelList(m_plstOutputRight, Settings::Get().Read(wxT("Output"), wxT("Right"), 0));
    }
    else if(event.GetString() == wxT("File"))
    {
        ShowFiles();
        m_pswpAog->ChangeSelection(wxT("Files"));
    }
    else if(event.GetString() == wxT("Sequence"))
    {
        ShowSequences();
        m_pswpAog->ChangeSelection(wxT("Files"));
    }
    else if(event.GetString() == wxT("Generator"))
    {
        m_pswpAog->ChangeSelection(wxT("Generator"));
    }
    else if(event.GetString() == wxT("Noise"))
    {
        m_pswpAog->ChangeSelection(wxT("Noise"));
    }
    m_pbtnSequences->Show(event.GetString() == wxT("Sequence"));
}

void pnlSettings::OnlstAogFilesSelected(wxCommandEvent& event)
{
    if(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("File"))
    {
        Settings::Get().Write(wxT("Output"), wxT("File"), event.GetString());
    }
    else if(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")) == wxT("Sequence"))
    {
        Settings::Get().Write(wxT("Output"), wxT("Sequence"), event.GetString());
    }
}

void pnlSettings::OnbtnAogHomeClick(wxCommandEvent& event)
{
    m_plstAogFiles->ShowFirstPage(false,false);
}

void pnlSettings::OnbtnAogPrevClick(wxCommandEvent& event)
{
    m_plstAogFiles->ShowPreviousPage(false, false);
}

void pnlSettings::OnbtnAogNextClick(wxCommandEvent& event)
{
    m_plstAogFiles->ShowNextPage(false, false);
}

void pnlSettings::OnbtnAogEndClick(wxCommandEvent& event)
{
    m_plstAogFiles->ShowLastPage(false, false);
}


void pnlSettings::OnSliderMove(wxCommandEvent& event)
{
    m_plblFrequency->SetLabel(wxString::Format(wxT("%.0f Hz"), pow(10,(m_pSlider->GetPosition()/20.0))));
    Settings::Get().Write(wxT("Generator"), wxT("Frequency"), wxString::Format(wxT("%.0f"), pow(10,(m_pSlider->GetPosition()/20.0))));
}

void pnlSettings::OnAmplitudeMove(wxCommandEvent& event)
{
    m_plbldB->SetLabel(wxString::Format(wxT("%.1f dBFS"),m_pAmplitude->GetPosition()-80.0));
    Settings::Get().Write(wxT("Generator"), wxT("Amplitude"), wxString::Format(wxT("%.1f"),m_pAmplitude->GetPosition()-80.0));
}

void pnlSettings::OnNoiseAmplitudeMove(wxCommandEvent& event)
{
    m_plblNoisedB->SetLabel(wxString::Format(wxT("%.1f dBFS"),m_pNoiseAmplitude->GetPosition()-80.0));
    Settings::Get().Write(wxT("Noise"), wxT("Amplitude"), wxString::Format(wxT("%.1f"),m_pNoiseAmplitude->GetPosition()-80.0));
}

void pnlSettings::OnbtnMinus1Click(wxCommandEvent& event)
{
    double dPosition(pow(10,(m_pSlider->GetPosition()/20.0)));
    dPosition--;

    m_pSlider->SetSliderPosition(20*log10(dPosition), true);
}

void pnlSettings::OnbtnPlus1Click(wxCommandEvent& event)
{
    double dPosition(pow(10,(m_pSlider->GetPosition()/20.0)));
    dPosition++;
    m_pSlider->SetSliderPosition(20*log10(dPosition), true);
}

void pnlSettings::OnbtnPlus10Click(wxCommandEvent& event)
{
    double dPosition(pow(10,(m_pSlider->GetPosition()/20.0)));
    dPosition+=10;

    m_pSlider->SetSliderPosition(20*log10(dPosition), true);
}

void pnlSettings::OnbtnMinus10Click(wxCommandEvent& event)
{
    double dPosition(pow(10,(m_pSlider->GetPosition()/20.0)));
    dPosition-=10;

    m_pSlider->SetSliderPosition(20*log10(dPosition), true);
}

void pnlSettings::OnlstShapeSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Generator"), wxT("Shape"), event.GetInt());
}

void pnlSettings::Onbtn450Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(450), true);
}

void pnlSettings::Onbtn900Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(900), true);
}

void pnlSettings::Onbtn1000Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(1000), true);
}

void pnlSettings::Onbtn5000Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(5000), true);
}

void pnlSettings::Onbtn10000Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(10000), true);
}

void pnlSettings::Onbtn0dbuClick(wxCommandEvent& event)
{
    m_pAmplitude->SetSliderPosition(62, true);
}

void pnlSettings::OnbtnSequencesClick(wxCommandEvent& event)
{
    dlgSequence aDlg(this);
    aDlg.ShowModal();
    ShowSequences();
}

void pnlSettings::OnlstColourSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Noise"), wxT("Colour"), event.GetInt());
}

void pnlSettings::OnbtnNoise0dBuClick(wxCommandEvent& event)
{
    m_pNoiseAmplitude->SetSliderPosition(62, true);
}

void pnlSettings::OnbtnCursorClick(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("General"), wxT("Cursor"), event.IsChecked());
}

void pnlSettings::OnbtnOptionsClick(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("General"), wxT("ShowOptions"), event.IsChecked());
}

void pnlSettings::OnlstOutputLeftSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Left"), event.GetInt());
}

void pnlSettings::OnlstOutputRightSelected(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Output"), wxT("Right"), event.GetInt());
}


void pnlSettings::PopulateChannelList(wmList* pList, int nSelected)
{
    pList->Clear();
    int nChannels(0);
    if(IOManager::Get().GetSession().itCurrentSubsession != IOManager::Get().GetSession().lstSubsession.end())
    {
        nChannels = IOManager::Get().GetSession().itCurrentSubsession->nChannels;
    }
    if(nChannels > 0)
    {
        if(SoundcardManager::Get().GetInputNumberOfChannels() == 2)
        {
            pList->AddButton(wxT("Left"));
            pList->AddButton(wxT("Right"));
        }
        else
        {
            for(int i = 0; i < SoundcardManager::Get().GetInputNumberOfChannels(); ++i)
            {
            pList->AddButton(wxString::Format(wxT("Ch %d"), i));
            }
        }
        pList->SelectButton(nSelected, true);
    }
}


void pnlSettings::InputSessionChanged()
{
    PopulateChannelList(m_plstOutputLeft, Settings::Get().Read(wxT("Output"), wxT("Left"), 0));
    PopulateChannelList(m_plstOutputRight, Settings::Get().Read(wxT("Output"), wxT("Right"), 0));
}
