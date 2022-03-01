#include "pnlSettingsGenerators.h"
#include "settings.h"
#include "settingevent.h"
#include "images/end_hz.xpm"
#include "images/end_hz_press.xpm"
#include "images/home_hz_press.xpm"
#include "images/home_hz.xpm"
#include "images/pagedown.xpm"
#include "images/pagedown_press.xpm"
#include "images/pageup.xpm"
#include "images/pageup_press.xpm"
#include "dlgSequence.h"
#include "iomanager.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include "generatorpluginfactory.h"
#include "generatorpluginbuilder.h"
#include "dlgWav.h"

//(*InternalHeaders(pnlSettingsGenerators)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

using namespace std;


//(*IdInit(pnlSettingsGenerators)
const long pnlSettingsGenerators::ID_M_PLST8 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL8 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST3 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST1 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST2 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST4 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST6 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST10 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST11 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST12 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL10 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL2 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL3 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL4 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL5 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL6 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL12 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL13 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL14 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL11 = wxNewId();
const long pnlSettingsGenerators::ID_PANEL11 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST7 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN6 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN7 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN8 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN9 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN20 = wxNewId();
const long pnlSettingsGenerators::ID_PANEL9 = wxNewId();
const long pnlSettingsGenerators::ID_STATICBOX2 = wxNewId();
const long pnlSettingsGenerators::ID_STATICBOX1 = wxNewId();
const long pnlSettingsGenerators::ID_SLIDER = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL7 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN12 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN11 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN10 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN13 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN14 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN15 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN16 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN17 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN18 = wxNewId();
const long pnlSettingsGenerators::ID_CUSTOM1 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL9 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN19 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST9 = wxNewId();
const long pnlSettingsGenerators::ID_PANEL10 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLST5 = wxNewId();
const long pnlSettingsGenerators::ID_STATICBOX3 = wxNewId();
const long pnlSettingsGenerators::ID_CUSTOM2 = wxNewId();
const long pnlSettingsGenerators::ID_M_PLBL1 = wxNewId();
const long pnlSettingsGenerators::ID_M_PBTN21 = wxNewId();
const long pnlSettingsGenerators::ID_PANEL12 = wxNewId();
const long pnlSettingsGenerators::ID_M_PSWP2 = wxNewId();
const long pnlSettingsGenerators::ID_PANEL8 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsGenerators,wxPanel)
	//(*EventTable(pnlSettingsGenerators)
	//*)
END_EVENT_TABLE()

pnlSettingsGenerators::pnlSettingsGenerators(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString sempty)
{
	//(*Initialize(pnlSettingsGenerators)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	pnlGenerator = new wxPanel(this, ID_PANEL8, wxDefaultPosition, wxSize(600,440), wxTAB_TRAVERSAL, _T("ID_PANEL8"));
	pnlGenerator->SetBackgroundColour(wxColour(0,0,0));
	m_plstAudioSources = new wmList(pnlGenerator, ID_M_PLST8, wxPoint(0,0), wxSize(600,64), wmList::STYLE_SELECT, 0, wxSize(-1,30), 8, wxSize(2,-1));
	m_plstAudioSources->SetBackgroundColour(wxColour(0,0,0));
	m_plstAudioSources->SetButtonColour(wxColour(wxT("#400080")));
	m_plstAudioSources->SetSelectedButtonColour(wxColour(wxT("#FF8000")));
	m_pswpAog = new wmSwitcherPanel(pnlGenerator, ID_M_PSWP2, wxPoint(0,70), wxSize(600,380), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP2"));
	m_pswpAog->SetPageNameStyle(0);
	Panel5 = new wxPanel(m_pswpAog, ID_PANEL11, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL11"));
	Panel5->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(Panel5, ID_M_PLBL8, _("Input-Output Channel Mapping"), wxPoint(0,0), wxSize(600,30), 0, _T("ID_M_PLBL8"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_plstOutput_1 = new wmList(Panel5, ID_M_PLST3, wxPoint(60,60), wxSize(540,35), wmList::STYLE_SELECT, 0, wxSize(-1,32), 8, wxSize(2,-1));
	m_plstOutput_1->SetBackgroundColour(wxColour(0,0,0));
	m_plstOutput_1->SetButtonColour(wxColour(wxT("#A0A0A0")));
	m_plstOutput_1->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstOutput_2 = new wmList(Panel5, ID_M_PLST1, wxPoint(60,95), wxSize(540,35), wmList::STYLE_SELECT, 0, wxSize(-1,32), 8, wxSize(2,-1));
	m_plstOutput_2->SetBackgroundColour(wxColour(0,0,0));
	m_plstOutput_2->SetButtonColour(wxColour(wxT("#A0A0A0")));
	m_plstOutput_2->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstOutput_3 = new wmList(Panel5, ID_M_PLST2, wxPoint(60,130), wxSize(540,35), wmList::STYLE_SELECT, 0, wxSize(-1,32), 8, wxSize(2,-1));
	m_plstOutput_3->SetBackgroundColour(wxColour(0,0,0));
	m_plstOutput_3->SetButtonColour(wxColour(wxT("#A0A0A0")));
	m_plstOutput_3->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstOutput_4 = new wmList(Panel5, ID_M_PLST4, wxPoint(60,165), wxSize(540,35), wmList::STYLE_SELECT, 0, wxSize(-1,32), 8, wxSize(2,-1));
	m_plstOutput_4->SetBackgroundColour(wxColour(0,0,0));
	m_plstOutput_4->SetButtonColour(wxColour(wxT("#A0A0A0")));
	m_plstOutput_4->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstOutput_5 = new wmList(Panel5, ID_M_PLST6, wxPoint(60,200), wxSize(540,35), wmList::STYLE_SELECT, 0, wxSize(-1,32), 8, wxSize(2,-1));
	m_plstOutput_5->SetBackgroundColour(wxColour(0,0,0));
	m_plstOutput_5->SetButtonColour(wxColour(wxT("#A0A0A0")));
	m_plstOutput_5->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstOutput_6 = new wmList(Panel5, ID_M_PLST10, wxPoint(60,235), wxSize(540,35), wmList::STYLE_SELECT, 0, wxSize(-1,32), 8, wxSize(2,-1));
	m_plstOutput_6->SetBackgroundColour(wxColour(0,0,0));
	m_plstOutput_6->SetButtonColour(wxColour(wxT("#A0A0A0")));
	m_plstOutput_6->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstOutput_7 = new wmList(Panel5, ID_M_PLST11, wxPoint(60,270), wxSize(540,35), wmList::STYLE_SELECT, 0, wxSize(-1,32), 8, wxSize(2,-1));
	m_plstOutput_7->SetBackgroundColour(wxColour(0,0,0));
	m_plstOutput_7->SetButtonColour(wxColour(wxT("#A0A0A0")));
	m_plstOutput_7->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plstOutput_8 = new wmList(Panel5, ID_M_PLST12, wxPoint(60,305), wxSize(540,35), wmList::STYLE_SELECT, 0, wxSize(-1,32), 8, wxSize(2,-1));
	m_plstOutput_8->SetBackgroundColour(wxColour(0,0,0));
	m_plstOutput_8->SetButtonColour(wxColour(wxT("#A0A0A0")));
	m_plstOutput_8->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_plblOutput = new wmLabel(Panel5, ID_M_PLBL10, _("Outputs:"), wxPoint(0,30), wxSize(70,30), 0, _T("ID_M_PLBL10"));
	m_plblOutput->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutput->GetUiRect().SetGradient(0);
	m_plblOutput->SetForegroundColour(wxColour(255,255,255));
	m_plblOutput->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblOutputFont(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblOutput->SetFont(m_plblOutputFont);
	m_plblOutput_1 = new wmLabel(Panel5, ID_M_PLBL2, _("1"), wxPoint(0,60), wxSize(60,35), 0, _T("ID_M_PLBL2"));
	m_plblOutput_1->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutput_1->GetUiRect().SetGradient(0);
	m_plblOutput_1->SetForegroundColour(wxColour(255,255,255));
	m_plblOutput_1->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblOutput_1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblOutput_1->SetFont(m_plblOutput_1Font);
	m_plblOutput_2 = new wmLabel(Panel5, ID_M_PLBL3, _("2"), wxPoint(0,95), wxSize(60,35), 0, _T("ID_M_PLBL3"));
	m_plblOutput_2->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutput_2->GetUiRect().SetGradient(0);
	m_plblOutput_2->SetForegroundColour(wxColour(255,255,255));
	m_plblOutput_2->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblOutput_2Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblOutput_2->SetFont(m_plblOutput_2Font);
	m_plblOutput_3 = new wmLabel(Panel5, ID_M_PLBL4, _("3"), wxPoint(0,130), wxSize(60,35), 0, _T("ID_M_PLBL4"));
	m_plblOutput_3->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutput_3->GetUiRect().SetGradient(0);
	m_plblOutput_3->SetForegroundColour(wxColour(255,255,255));
	m_plblOutput_3->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblOutput_3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblOutput_3->SetFont(m_plblOutput_3Font);
	m_plblOutput_4 = new wmLabel(Panel5, ID_M_PLBL5, _("4"), wxPoint(0,165), wxSize(60,35), 0, _T("ID_M_PLBL5"));
	m_plblOutput_4->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutput_4->GetUiRect().SetGradient(0);
	m_plblOutput_4->SetForegroundColour(wxColour(255,255,255));
	m_plblOutput_4->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblOutput_4Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblOutput_4->SetFont(m_plblOutput_4Font);
	m_plblOutput_5 = new wmLabel(Panel5, ID_M_PLBL6, _("5"), wxPoint(0,200), wxSize(60,35), 0, _T("ID_M_PLBL6"));
	m_plblOutput_5->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutput_5->GetUiRect().SetGradient(0);
	m_plblOutput_5->SetForegroundColour(wxColour(255,255,255));
	m_plblOutput_5->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblOutput_5Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblOutput_5->SetFont(m_plblOutput_5Font);
	m_plblOutput_6 = new wmLabel(Panel5, ID_M_PLBL12, _("6"), wxPoint(0,235), wxSize(60,35), 0, _T("ID_M_PLBL12"));
	m_plblOutput_6->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutput_6->GetUiRect().SetGradient(0);
	m_plblOutput_6->SetForegroundColour(wxColour(255,255,255));
	m_plblOutput_6->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblOutput_6Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblOutput_6->SetFont(m_plblOutput_6Font);
	m_plblOutput_7 = new wmLabel(Panel5, ID_M_PLBL13, _("7"), wxPoint(0,270), wxSize(60,35), 0, _T("ID_M_PLBL13"));
	m_plblOutput_7->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutput_7->GetUiRect().SetGradient(0);
	m_plblOutput_7->SetForegroundColour(wxColour(255,255,255));
	m_plblOutput_7->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblOutput_7Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblOutput_7->SetFont(m_plblOutput_7Font);
	m_plblOutput_8 = new wmLabel(Panel5, ID_M_PLBL14, _("8"), wxPoint(0,305), wxSize(60,35), 0, _T("ID_M_PLBL14"));
	m_plblOutput_8->SetBorderState(uiRect::BORDER_NONE);
	m_plblOutput_8->GetUiRect().SetGradient(0);
	m_plblOutput_8->SetForegroundColour(wxColour(255,255,255));
	m_plblOutput_8->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_plblOutput_8Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblOutput_8->SetFont(m_plblOutput_8Font);
	m_pLbl7 = new wmLabel(Panel5, ID_M_PLBL11, _("Inputs:"), wxPoint(200,30), wxSize(60,30), 0, _T("ID_M_PLBL11"));
	m_pLbl7->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl7->GetUiRect().SetGradient(0);
	m_pLbl7->SetForegroundColour(wxColour(255,255,255));
	m_pLbl7->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl7Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_pLbl7->SetFont(m_pLbl7Font);
	Panel3 = new wxPanel(m_pswpAog, ID_PANEL9, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL9"));
	Panel3->SetBackgroundColour(wxColour(0,0,0));
	m_plstAogFiles = new wmList(Panel3, ID_M_PLST7, wxPoint(0,0), wxSize(600,315), wmList::STYLE_SELECT, 2, wxSize(-1,40), 3, wxSize(5,5));
	m_plstAogFiles->SetBackgroundColour(wxColour(0,0,0));
	m_plstAogFiles->SetSelectedButtonColour(wxColour(wxT("#008000")));
	m_pbtnAogHome = new wmButton(Panel3, ID_M_PBTN6, wxEmptyString, wxPoint(10,325), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN6"));
	m_pbtnAogHome->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnAogPrev = new wmButton(Panel3, ID_M_PBTN7, wxEmptyString, wxPoint(130,325), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN7"));
	m_pbtnAogPrev->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnAogNext = new wmButton(Panel3, ID_M_PBTN8, wxEmptyString, wxPoint(250,325), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN8"));
	m_pbtnAogNext->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnAogEnd = new wmButton(Panel3, ID_M_PBTN9, wxEmptyString, wxPoint(370,325), wxSize(100,40), 0, wxDefaultValidator, _T("ID_M_PBTN9"));
	m_pbtnAogEnd->SetColourDisabled(wxColour(wxT("#808080")));
	m_pbtnSequences = new wmButton(Panel3, ID_M_PBTN20, _("Edit Sequences"), wxPoint(490,325), wxSize(105,40), 0, wxDefaultValidator, _T("ID_M_PBTN20"));
	m_pbtnSequences->Hide();
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
	m_plblFrequency->GetUiRect().SetGradient(0);
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
	m_plbldB->GetUiRect().SetGradient(0);
	m_plbldB->SetForegroundColour(wxColour(0,0,0));
	m_plbldB->SetBackgroundColour(wxColour(255,255,255));
	m_pbtn0dbu = new wmButton(Panel4, ID_M_PBTN19, _("-18 dBFS"), wxPoint(480,250), wxSize(80,40), 0, wxDefaultValidator, _T("ID_M_PBTN19"));
	m_plstShape = new wmList(Panel4, ID_M_PLST9, wxPoint(150,320), wxSize(300,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 4, wxSize(5,1));
	m_plstShape->SetBackgroundColour(wxColour(0,0,0));
	Panel2 = new wxPanel(m_pswpAog, ID_PANEL12, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL12"));
	Panel2->SetBackgroundColour(wxColour(0,0,0));
	m_plstColour = new wmList(Panel2, ID_M_PLST5, wxPoint(10,10), wxSize(580,200), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 4, wxSize(5,1));
	m_plstColour->SetBackgroundColour(wxColour(0,0,0));
	StaticBox3 = new wxStaticBox(Panel2, ID_STATICBOX3, _("Amplitude"), wxPoint(5,210), wxSize(590,105), 0, _T("ID_STATICBOX3"));
	StaticBox3->SetForegroundColour(wxColour(255,255,255));
	StaticBox3->SetBackgroundColour(wxColour(0,0,0));
	m_pNoiseAmplitude = new wmSlider(Panel2,ID_CUSTOM2,wxT("Frequency"),wxPoint(10,250),wxSize(400,40),5,_T("ID_CUSTOM2"));
	m_plblNoisedB = new wmLabel(Panel2, ID_M_PLBL1, _("dB"), wxPoint(415,250), wxSize(60,40), 0, _T("ID_M_PLBL1"));
	m_plblNoisedB->SetBorderState(uiRect::BORDER_NONE);
	m_plblNoisedB->GetUiRect().SetGradient(0);
	m_plblNoisedB->SetForegroundColour(wxColour(0,0,0));
	m_plblNoisedB->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnNoise0dBu = new wmButton(Panel2, ID_M_PBTN21, _("-18 dBFS"), wxPoint(480,250), wxSize(80,40), 0, wxDefaultValidator, _T("ID_M_PBTN21"));
	m_pswpAog->AddPage(Panel5, _("Input"), false);
	m_pswpAog->AddPage(Panel3, _("Files"), false);
	m_pswpAog->AddPage(Panel4, _("Generator"), false);
	m_pswpAog->AddPage(Panel2, _("Noise"), false);

	Connect(ID_M_PLST8,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsGenerators::OnlstAudioSourcesSelected);
	Connect(ID_M_PLST7,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsGenerators::OnlstAogFilesSelected);
	Connect(ID_M_PBTN6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnAogHomeClick);
	Connect(ID_M_PBTN7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnAogPrevClick);
	Connect(ID_M_PBTN8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnAogNextClick);
	Connect(ID_M_PBTN9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnAogEndClick);
	Connect(ID_M_PBTN20,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnSequencesClick);
	Connect(ID_M_PBTN12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnMinus10Click);
	Connect(ID_M_PBTN11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnMinus1Click);
	Connect(ID_M_PBTN10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnPlus1Click);
	Connect(ID_M_PBTN13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnPlus10Click);
	Connect(ID_M_PBTN14,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::Onbtn450Click);
	Connect(ID_M_PBTN15,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::Onbtn900Click);
	Connect(ID_M_PBTN16,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::Onbtn1000Click);
	Connect(ID_M_PBTN17,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::Onbtn5000Click);
	Connect(ID_M_PBTN18,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::Onbtn10000Click);
	Connect(ID_M_PBTN19,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::Onbtn0dbuClick);
	Connect(ID_M_PLST9,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsGenerators::OnlstShapeSelected);
	Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsGenerators::OnlstColourSelected);
	Connect(ID_M_PBTN21,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsGenerators::OnbtnNoise0dBuClick);
	//*)
	SetSize(size);
	SetPosition(pos);

	m_pbtnAogEnd->SetBitmapLabel(wxBitmap(end_hz_xpm));
    m_pbtnAogEnd->SetBitmapSelected(wxBitmap(end_hz_press_xpm));
    m_pbtnAogHome->SetBitmapLabel(wxBitmap(home_hz_xpm));
    m_pbtnAogHome->SetBitmapSelected(wxBitmap(home_hz_press_xpm));
    m_pbtnAogPrev->SetBitmapLabel(wxBitmap(pageup_xpm));
    m_pbtnAogPrev->SetBitmapSelected(wxBitmap(pageup_press_xpm));
    m_pbtnAogNext->SetBitmapLabel(wxBitmap(pagedown_xpm));
    m_pbtnAogNext->SetBitmapSelected(wxBitmap(pagedown_press_xpm));

    m_plstAudioSources->AddButton(wxT("Input"));
    m_plstAudioSources->AddButton(wxT("File"));
    m_plstAudioSources->AddButton(wxT("Sequence"));
    m_plstAudioSources->AddButton(wxT("Generator"));
    m_plstAudioSources->AddButton(wxT("Noise"));
    m_plstAudioSources->Enable(Settings::Get().Read(wxT("Output"), wxT("Destination"),wxT("Disabled"))!=wxT("Disabled"));
    m_plstAudioSources->ConnectToSetting("Output", "Source", "Input");

    LoadPlugins();

    if(Settings::Get().Read(wxT("Output"), wxT("Destination"),wxT("Disabled"))==wxT("Disabled"))
    {
        m_pswpAog->ChangeSelection(wxT("Input"));
    }
    else
    {
        m_pswpAog->ChangeSelection(Settings::Get().Read("Output", "Source", "Input"));
    }
    m_pSlider->Init(0,20*log10(22000), 20*log10(Settings::Get().Read(wxT("Generator"), wxT("Frequency"), 1000)));
    m_plblFrequency->SetLabel(wxString::Format(wxT("%.0f Hz"), pow(10,(m_pSlider->GetPosition()/20.0))));

    m_pAmplitude->Init(0,80, 80+Settings::Get().Read(wxT("Generator"), wxT("Amplitude"), -18.0));
    m_plbldB->SetLabel(wxString::Format(wxT("%.1f dBFS"),m_pAmplitude->GetPosition()-80.0));

    m_plstShape->AddButton(wxT("Sine"));
    m_plstShape->AddButton(wxT("Square"));
    m_plstShape->AddButton(wxT("Sawtooth"));
    m_plstShape->AddButton(wxT("Triangle"));

    m_plstShape->ConnectToSetting("Generator", "Shape", size_t(0));

    m_plstColour->AddButton(wxT("White"));
    m_plstColour->AddButton(wxT("Pink"));
    m_plstColour->AddButton(wxT("Grey"));
    m_plstColour->AddButton(wxT("A"));
    m_plstColour->AddButton(wxT("K"));
    m_plstColour->ConnectToSetting("Noise","Colour",size_t(0));

    m_pNoiseAmplitude->Init(0,80, 80+Settings::Get().Read(wxT("Noise"), wxT("Amplitude"), -18.0));
    m_plblNoisedB->SetLabel(wxString::Format(wxT("%.1f dBFS"),m_pNoiseAmplitude->GetPosition()-80.0));

    Connect(m_pSlider->GetId(), wxEVT_SLIDER_MOVE, (wxObjectEventFunction)&pnlSettingsGenerators::OnSliderMove);
    Connect(m_pAmplitude->GetId(), wxEVT_SLIDER_MOVE, (wxObjectEventFunction)&pnlSettingsGenerators::OnAmplitudeMove);
    Connect(m_pNoiseAmplitude->GetId(), wxEVT_SLIDER_MOVE, (wxObjectEventFunction)&pnlSettingsGenerators::OnNoiseAmplitudeMove);


    m_plstOutput_1->ConnectToSetting("Output", "Channel_1", size_t(0));
    m_plstOutput_2->ConnectToSetting("Output", "Channel_2", size_t(1));
    m_plstOutput_3->ConnectToSetting("Output", "Channel_3", size_t(2));
    m_plstOutput_4->ConnectToSetting("Output", "Channel_4", size_t(3));
    m_plstOutput_5->ConnectToSetting("Output", "Channel_5", size_t(4));
    m_plstOutput_6->ConnectToSetting("Output", "Channel_6", size_t(5));
    m_plstOutput_7->ConnectToSetting("Output", "Channel_7", size_t(6));
    m_plstOutput_8->ConnectToSetting("Output", "Channel_8", size_t(7));


    Settings::Get().AddHandler(this, "Output", "Destination");
    Settings::Get().AddHandler(this, "Output", "Source");
    Settings::Get().AddHandler(this, "Generator", "Frequency");
    Settings::Get().AddHandler(this, "Generator", "Amplitude");
    Settings::Get().AddHandler(this, "Noise", "Amplitude");
    Connect(wxID_ANY, wxEVT_SETTING_CHANGED, (wxObjectEventFunction)&pnlSettingsGenerators::OnSettingChanged);


}

pnlSettingsGenerators::~pnlSettingsGenerators()
{
	//(*Destroy(pnlSettingsGenerators)
	//*)
	Settings::Get().RemoveHandler(this);
}


void pnlSettingsGenerators::OnlstAudioSourcesSelected(wxCommandEvent& event)
{


}

void pnlSettingsGenerators::OnlstAogFilesSelected(wxCommandEvent& event)
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

void pnlSettingsGenerators::OnbtnAogHomeClick(wxCommandEvent& event)
{
    m_plstAogFiles->ShowFirstPage(false,false);
}

void pnlSettingsGenerators::OnbtnAogPrevClick(wxCommandEvent& event)
{
    m_plstAogFiles->ShowPreviousPage(false, false);
}

void pnlSettingsGenerators::OnbtnAogNextClick(wxCommandEvent& event)
{
    m_plstAogFiles->ShowNextPage(false, false);
}

void pnlSettingsGenerators::OnbtnAogEndClick(wxCommandEvent& event)
{
    m_plstAogFiles->ShowLastPage(false, false);
}


void pnlSettingsGenerators::OnSliderMove(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Generator"), wxT("Frequency"), wxString::Format(wxT("%.0f"), pow(10,(m_pSlider->GetPosition()/20.0))));
}

void pnlSettingsGenerators::OnAmplitudeMove(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Generator"), wxT("Amplitude"), wxString::Format(wxT("%.1f"),m_pAmplitude->GetPosition()-80.0));
}

void pnlSettingsGenerators::OnNoiseAmplitudeMove(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("Noise"), wxT("Amplitude"), wxString::Format(wxT("%.1f"),m_pNoiseAmplitude->GetPosition()-80.0));
}

void pnlSettingsGenerators::OnbtnMinus1Click(wxCommandEvent& event)
{
    double dPosition(pow(10,(m_pSlider->GetPosition()/20.0)));
    dPosition--;

    m_pSlider->SetSliderPosition(20*log10(dPosition), true);
}

void pnlSettingsGenerators::OnbtnPlus1Click(wxCommandEvent& event)
{
    double dPosition(pow(10,(m_pSlider->GetPosition()/20.0)));
    dPosition++;
    m_pSlider->SetSliderPosition(20*log10(dPosition), true);
}

void pnlSettingsGenerators::OnbtnPlus10Click(wxCommandEvent& event)
{
    double dPosition(pow(10,(m_pSlider->GetPosition()/20.0)));
    dPosition+=10;

    m_pSlider->SetSliderPosition(20*log10(dPosition), true);
}

void pnlSettingsGenerators::OnbtnMinus10Click(wxCommandEvent& event)
{
    double dPosition(pow(10,(m_pSlider->GetPosition()/20.0)));
    dPosition-=10;

    m_pSlider->SetSliderPosition(20*log10(dPosition), true);
}

void pnlSettingsGenerators::OnlstShapeSelected(wxCommandEvent& event)
{
//    Settings::Get().Write(wxT("Generator"), wxT("Shape"), event.GetInt());
}

void pnlSettingsGenerators::Onbtn450Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(450), true);
}

void pnlSettingsGenerators::Onbtn900Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(900), true);
}

void pnlSettingsGenerators::Onbtn1000Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(1000), true);
}

void pnlSettingsGenerators::Onbtn5000Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(5000), true);
}

void pnlSettingsGenerators::Onbtn10000Click(wxCommandEvent& event)
{
    m_pSlider->SetSliderPosition(20*log10(10000), true);
}

void pnlSettingsGenerators::Onbtn0dbuClick(wxCommandEvent& event)
{
    m_pAmplitude->SetSliderPosition(62, true);
}

void pnlSettingsGenerators::OnbtnSequencesClick(wxCommandEvent& event)
{
    if(Settings::Get().Read(wxT("Output"), wxT("Source"), wxEmptyString) == wxT("Sequence"))
    {
        dlgSequence aDlg(this);
        aDlg.ShowModal();
        ShowSequences();
    }
    else if(Settings::Get().Read(wxT("Output"), wxT("Source"), wxEmptyString) == wxT("File"))
    {
        dlgWav aDlg(this);
        aDlg.ShowModal();
        ShowFiles();
    }
}

void pnlSettingsGenerators::OnlstColourSelected(wxCommandEvent& event)
{
  //  Settings::Get().Write(wxT("Noise"), wxT("Colour"), event.GetInt());
}

void pnlSettingsGenerators::OnbtnNoise0dBuClick(wxCommandEvent& event)
{
    m_pNoiseAmplitude->SetSliderPosition(62, true);
}

void pnlSettingsGenerators::OnlstOutputLeftSelected(wxCommandEvent& event)
{
}

void pnlSettingsGenerators::OnlstOutputRightSelected(wxCommandEvent& event)
{
}

void pnlSettingsGenerators::PopulateChannelList(wmList* pList)
{
    pList->Clear();
    int nChannels(0);
    if(IOManager::Get().GetInputSession().GetCurrentSubsession() != IOManager::Get().GetInputSession().lstSubsession.end())
    {
        nChannels = IOManager::Get().GetInputSession().GetCurrentSubsession()->nChannels;
    }
    if(nChannels > 0)
    {
        if(nChannels==2)
        {
            pList->AddButton("Left");
            pList->AddButton("Right");
        }
        else
        {
            for(int i = 0; i < nChannels; ++i)
            {
                pList->AddButton(wxString::Format("Ch %d", i));
            }
        }
        pList->ReloadSetting();
    }
}

void pnlSettingsGenerators::ShowChannelMapping(wmList* pList, wmLabel* pLabel, int nChannels, int nOutput)
{
    if(nChannels >= nOutput)
    {
        pList->Show();
        pLabel->Show();
        PopulateChannelList(pList);
    }
    else
    {
        pList->Show(false);
        pLabel->Show(false);
    }
}

void pnlSettingsGenerators::SessionChanged()
{
    int nChannels = 0;
    if(IOManager::Get().GetOutputSession().GetCurrentSubsession() != IOManager::Get().GetOutputSession().lstSubsession.end())
    {
        nChannels = IOManager::Get().GetOutputSession().GetCurrentSubsession()->nChannels;
    }
    ShowChannelMapping(m_plstOutput_1, m_plblOutput_1, nChannels, 1);
    ShowChannelMapping(m_plstOutput_2, m_plblOutput_2, nChannels, 2);
    ShowChannelMapping(m_plstOutput_3, m_plblOutput_3, nChannels, 3);
    ShowChannelMapping(m_plstOutput_4, m_plblOutput_4, nChannels, 4);
    ShowChannelMapping(m_plstOutput_5, m_plblOutput_5, nChannels, 5);
    ShowChannelMapping(m_plstOutput_6, m_plblOutput_6, nChannels, 6);
    ShowChannelMapping(m_plstOutput_7, m_plblOutput_7, nChannels, 7);
    ShowChannelMapping(m_plstOutput_8, m_plblOutput_8, nChannels, 8);
}

void pnlSettingsGenerators::OnSettingChanged(SettingEvent& event)
{
    if(event.GetSection()==wxT("Output"))
    {
        if(event.GetKey() == wxT("Destination"))
        {
            DestinationChanged(event.GetValue());
        }
        else if(event.GetKey() == "Source")
        {
            SourceChanged(event.GetValue());
        }
    }
    else if(event.GetSection() == "Generator")
    {
        if(event.GetKey() == "Frequency")
        {
            m_plblFrequency->SetLabel(wxString::Format(wxT("%.0f Hz"), event.GetValue(0.0)));
        }
        else if(event.GetKey() == "Amplitude")
        {
            m_plbldB->SetLabel(wxString::Format("%.1f dBFS", event.GetValue(0.0)));
        }
    }
    else if(event.GetSection() == "Noise")
    {
        if(event.GetKey() == "Amplitude")
        {
            m_plblNoisedB->SetLabel(wxString::Format("%.1f dBFS", event.GetValue(0.0)));
        }
    }
}

void pnlSettingsGenerators::DestinationChanged(const wxString& sDestination)
{
    m_plstAudioSources->Enable(sDestination!=wxT("Disabled"));
    if(sDestination == wxT("Disabled"))
    {
        m_pswpAog->ChangeSelection(wxT("Input"));

    }
    else
    {
        m_plstAudioSources->SelectButton(Settings::Get().Read(wxT("Output"), wxT("Source"), wxT("Input")), true);
    }
}
void pnlSettingsGenerators::SourceChanged(const wxString& sSource)
{
    m_pbtnSequences->Show(false);
    if(sSource == wxT("Input"))
    {
        m_pswpAog->ChangeSelection(wxT("Input"));
        PopulateChannelList(m_plstOutput_1);
        PopulateChannelList(m_plstOutput_2);
        PopulateChannelList(m_plstOutput_3);
        PopulateChannelList(m_plstOutput_4);
        PopulateChannelList(m_plstOutput_5);
        PopulateChannelList(m_plstOutput_6);
        PopulateChannelList(m_plstOutput_7);
        PopulateChannelList(m_plstOutput_8);
    }
    else if(sSource == wxT("File"))
    {
        ShowFiles();
        m_pswpAog->ChangeSelection(wxT("Files"));
        m_pbtnSequences->Show(true);
        m_pbtnSequences->SetLabel(wxT("Manage Files"));
    }
    else if(sSource == wxT("Sequence"))
    {
        ShowSequences();
        m_pswpAog->ChangeSelection(wxT("Files"));
        m_pbtnSequences->Show(true);
        m_pbtnSequences->SetLabel(wxT("Edit Sequences"));
    }
    else if(sSource == wxT("Generator"))
    {
        m_pswpAog->ChangeSelection(wxT("Generator"));
    }
    else if(sSource == wxT("Noise"))
    {
        m_pswpAog->ChangeSelection(wxT("Noise"));
    }
    else
    {   //plugin
        m_pswpAog->ChangeSelection(sSource);
    }

}


void pnlSettingsGenerators::ShowFiles()
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

void pnlSettingsGenerators::ShowSequences()
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

void pnlSettingsGenerators::ShowAogPagingButtons()
{
    m_pbtnAogEnd->Show(m_plstAogFiles->GetPageCount() > 1);
    m_pbtnAogHome->Show(m_plstAogFiles->GetPageCount() > 1);
    m_pbtnAogPrev->Show(m_plstAogFiles->GetPageCount() > 1);
    m_pbtnAogNext->Show(m_plstAogFiles->GetPageCount() > 1);
}

void pnlSettingsGenerators::LoadPlugins()
{
    GeneratorPluginFactory::Get()->SetHandler(this);
    GeneratorPluginFactory::Get()->SetSwitcherPanels(m_pswpAog);


    map<wxString, wxString>::const_iterator itBegin, itEnd;
    if(Settings::Get().GetSectionDataBegin(wxT("Generator Plugins"), itBegin) && Settings::Get().GetSectionDataEnd(wxT("Generator Plugins"), itEnd))
    {
        for(; itBegin != itEnd; ++itBegin)
        {
            GeneratorPluginFactory::Get()->LoadGeneratorLibrary(itBegin->second);
        }
    }

    for(map<wxString, GeneratorPluginBuilder*>::iterator itPlugin = GeneratorPluginFactory::Get()->GetPluginBegin(); itPlugin != GeneratorPluginFactory::Get()->GetPluginEnd(); ++itPlugin)
    {
        m_plstAudioSources->AddButton(itPlugin->first);
    }

}
