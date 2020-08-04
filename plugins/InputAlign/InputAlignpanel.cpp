#include "InputAlignpanel.h"
#include "InputAlignbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "settings.h"

//(*InternalHeaders(InputAlignPanel)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(InputAlignPanel)
const long InputAlignPanel::ID_M_PLBL37 = wxNewId();
const long InputAlignPanel::ID_M_PLBL1 = wxNewId();
const long InputAlignPanel::ID_M_PEDT1 = wxNewId();
const long InputAlignPanel::ID_M_PBTN1 = wxNewId();
const long InputAlignPanel::ID_M_PBTN2 = wxNewId();
const long InputAlignPanel::ID_M_PKBD1 = wxNewId();
const long InputAlignPanel::ID_M_PLBL5 = wxNewId();
const long InputAlignPanel::ID_M_PLST2 = wxNewId();
const long InputAlignPanel::ID_M_PLBL2 = wxNewId();
const long InputAlignPanel::ID_M_PLBL3 = wxNewId();
const long InputAlignPanel::ID_M_PLBL4 = wxNewId();
const long InputAlignPanel::ID_M_PLBL6 = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(InputAlignPanel,pmPanel);

BEGIN_EVENT_TABLE(InputAlignPanel,wxPanel)
	//(*EventTable(InputAlignPanel)
	//*)
END_EVENT_TABLE()

InputAlignPanel::InputAlignPanel(wxWindow* parent,InputAlignBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(InputAlignPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_pLbl12 = new wmLabel(this, ID_M_PLBL37, _("Audio Input Alignment"), wxPoint(0,0), wxSize(600,40), 0, _T("ID_M_PLBL37"));
	m_pLbl12->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl12->GetUiRect().SetGradient(0);
	m_pLbl12->SetForegroundColour(wxColour(255,255,255));
	m_pLbl12->SetBackgroundColour(wxColour(91,149,57));
	wxFont m_pLbl12Font(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl12->SetFont(m_pLbl12Font);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Set expected input level in -dBFS"), wxPoint(10,220), wxSize(238,25), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(128,64,0));
	wxFont m_pLbl1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl1->SetFont(m_pLbl1Font);
	m_pedtInputLevel = new wmEdit(this, ID_M_PEDT1, wxEmptyString, wxPoint(255,220), wxSize(120,25), 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtInputLevel->SetValidation(5);
	m_pedtInputLevel->SetFocus();
	m_pedtInputLevel->SetForegroundColour(wxColour(0,0,0));
	m_pedtInputLevel->SetBackgroundColour(wxColour(255,255,255));
	m_pedtInputLevel->SetFocusedBackground(wxColour(wxT("#FFFFFF")));
	m_pedtInputLevel->SetFocusedForeground(wxColour(wxT("#000000")));
	m_pedtInputLevel->SetBorderStyle(2,1);
	m_pbtnAlignSelected = new wmButton(this, ID_M_PBTN1, _("Align Selected Channel"), wxPoint(255,250), wxSize(120,50), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnAlignAll = new wmButton(this, ID_M_PBTN2, _("Align All Channels"), wxPoint(255,310), wxSize(120,50), 0, wxDefaultValidator, _T("ID_M_PBTN2"));
	m_pKbd1 = new wmKeyboard(this, ID_M_PKBD1, wxPoint(10,250), wxSize(240,200), 5, 0);
	m_pKbd1->SetForegroundColour(wxColour(255,255,255));
	wxFont m_pKbd1Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pKbd1->SetFont(m_pKbd1Font);
	m_pLbl4 = new wmLabel(this, ID_M_PLBL5, _("Choose Channel"), wxPoint(10,50), wxSize(340,25), 0, _T("ID_M_PLBL5"));
	m_pLbl4->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl4->GetUiRect().SetGradient(0);
	m_pLbl4->SetForegroundColour(wxColour(255,255,255));
	m_pLbl4->SetBackgroundColour(wxColour(128,64,0));
	wxFont m_pLbl4Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl4->SetFont(m_pLbl4Font);
	m_plstChannel1 = new wmList(this, ID_M_PLST2, wxPoint(10,77), wxSize(340,32), wmList::STYLE_SELECT, 0, wxSize(-1,30), 8, wxSize(-1,-1));
	m_plstChannel1->SetBackgroundColour(wxColour(0,0,0));
	m_pLbl2 = new wmLabel(this, ID_M_PLBL2, _("Current Level"), wxPoint(10,115), wxSize(100,0), 0, _T("ID_M_PLBL2"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(64,0,128));
	m_plblCurrent = new wmLabel(this, ID_M_PLBL3, _("Label"), wxPoint(110,115), wxSize(100,25), 0, _T("ID_M_PLBL3"));
	m_plblCurrent->SetBorderState(uiRect::BORDER_DOWN);
	m_plblCurrent->GetUiRect().SetGradient(0);
	m_plblCurrent->SetBackgroundColour(wxColour(255,255,255));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL4, _("Current Gain"), wxPoint(10,145), wxSize(100,0), 0, _T("ID_M_PLBL4"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(64,0,128));
	m_plblGain = new wmLabel(this, ID_M_PLBL6, _("Label"), wxPoint(110,145), wxSize(100,25), 0, _T("ID_M_PLBL6"));
	m_plblGain->SetBorderState(uiRect::BORDER_DOWN);
	m_plblGain->GetUiRect().SetGradient(0);
	m_plblGain->SetBackgroundColour(wxColour(255,255,255));

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&InputAlignPanel::OnbtnAlignSelectedClick);
	Connect(ID_M_PBTN2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&InputAlignPanel::OnbtnAlignAllClick);
	Connect(ID_M_PLST2,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&InputAlignPanel::OnlstChannelSelected);
	//*)


	m_pedtInputLevel->SetValue("18");
	SetSize(size);
	SetPosition(pos);
}

InputAlignPanel::~InputAlignPanel()
{
	//(*Destroy(InputAlignPanel)
	//*)
}

void InputAlignPanel::SetAudioData(const timedbuffer* pBuffer)
{
    m_dPeak = 0;
    for(unsigned int i = 0; i < pBuffer->GetBufferSize(); i+=m_nTotalChannels)
    {
        m_dPeak = std::max((double)fabs(pBuffer->GetBuffer()[i+m_nSelectedChannel]), m_dPeak);
    }
    if(m_dPeak == 0.0)
    {
        m_dPeak = -120.0;
    }
    else
    {
        m_dPeak =20*log10(m_dPeak);
    }
    m_plblCurrent->SetLabel(wxString::Format("%.3f dBFS", m_dPeak));
}

void InputAlignPanel::InputSession(const session& aSession)
{
    if(aSession.GetCurrentSubsession() != aSession.lstSubsession.end())
    {
        m_nTotalChannels = std::min((unsigned int)256 ,aSession.GetCurrentSubsession()->nChannels);
    }
    else
    {
        m_nTotalChannels = 0;
    }

    m_plstChannel1->Freeze();
    m_plstChannel1->Clear();

    for(size_t i = 1; i <= m_nTotalChannels; i++)
    {
        #ifdef __WXGNU__
        m_plstChannel1->AddButton(wxString::Format(wxT("Ch %zu"), i));
        #else
        m_plstChannel1->AddButton(wxString::Format(wxT("Ch %u"), i));
        #endif
    }

    m_plstChannel1->Thaw();

    m_plblCurrent->SetLabel(wxEmptyString);
    m_plblGain->SetLabel(wxEmptyString);


    m_plstChannel1->SelectButton(m_pBuilder->ReadSetting(wxT("Channel"), 0), true);


}

void InputAlignPanel::OutputChannels(const std::vector<char>& vChannels)
{

}


void InputAlignPanel::OnlstChannelSelected(wxCommandEvent& event)
{
    m_pBuilder->WriteSetting(wxT("Channel"), event.GetInt());
    m_nSelectedChannel = event.GetInt();
    m_plblGain->SetLabel(wxString::Format("%.4f dB",
                        ConvertRatioToGain(Settings::Get().Read("Input", wxString::Format("Ratio_%02d", m_nSelectedChannel), 1.0))));

}

double InputAlignPanel::GetGain()
{
    double dInput;
    if(m_pedtInputLevel->GetValue().ToDouble(&dInput))
    {
        return -dInput - m_dPeak;
    }
    return 0.0;
}

void InputAlignPanel::OnbtnAlignSelectedClick(wxCommandEvent& event)
{
    double dGain = GetGain();

    m_plblGain->SetLabel(wxString::Format("%.4f dB", dGain));
    Settings::Get().Write("Input", wxString::Format("Ratio_%02d", m_nSelectedChannel), ConvertGainToRatio(dGain));
}

void InputAlignPanel::OnbtnAlignAllClick(wxCommandEvent& event)
{
    double dGain = GetGain();
    m_plblGain->SetLabel(wxString::Format("%.4f dB", dGain));
    double dRatio = ConvertGainToRatio(dGain);
    for(unsigned int i = 0; i < m_nTotalChannels; i++)
    {
        Settings::Get().Write("Input", wxString::Format("Ratio_%02d", i), dRatio);
    }
}

double InputAlignPanel::ConvertGainToRatio(double dGain)
{
    return pow(10.0, dGain/20.0);
}

double InputAlignPanel::ConvertRatioToGain(double dRatio)
{
    return 20*log10(dRatio);
}
