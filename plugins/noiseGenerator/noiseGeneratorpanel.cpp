#include "noiseGeneratorpanel.h"
#include "noiseGeneratorbuilder.h"
#include "timedbuffer.h"
#include "session.h"
#include "settingevent.h"
#include <wx/log.h>
#include "log.h"

//(*InternalHeaders(noiseGeneratorPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(noiseGeneratorPanel)
const long noiseGeneratorPanel::ID_M_PLBL2 = wxNewId();
const long noiseGeneratorPanel::ID_M_PBTN1 = wxNewId();
const long noiseGeneratorPanel::ID_M_PLST1 = wxNewId();
const long noiseGeneratorPanel::ID_M_PLBL3 = wxNewId();
const long noiseGeneratorPanel::ID_M_PLST5 = wxNewId();
const long noiseGeneratorPanel::ID_STATICBOX3 = wxNewId();
const long noiseGeneratorPanel::ID_CUSTOM2 = wxNewId();
const long noiseGeneratorPanel::ID_M_PLBL1 = wxNewId();
const long noiseGeneratorPanel::ID_M_PBTN21 = wxNewId();
const long noiseGeneratorPanel::ID_PANEL12 = wxNewId();
//*)

wxIMPLEMENT_DYNAMIC_CLASS(noiseGeneratorPanel,pmPanel);

BEGIN_EVENT_TABLE(noiseGeneratorPanel,wxPanel)
	//(*EventTable(noiseGeneratorPanel)
	//*)
END_EVENT_TABLE()

noiseGeneratorPanel::noiseGeneratorPanel(wxWindow* parent,noiseGeneratorBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder),
    m_nSelected(-1)
{
	//(*Initialize(noiseGeneratorPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl1 = new wmLabel(this, ID_M_PLBL2, _("Generators:"), wxPoint(200,10), wxSize(100,40), 0, _T("ID_M_PLBL2"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->GetUiRect().SetGradient(0);
	m_pLbl1->SetForegroundColour(wxColour(255,255,255));
	m_pLbl1->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnGenerators = new wmButton(this, ID_M_PBTN1, _("1"), wxPoint(300,10), wxSize(80,40), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnGenerators->SetForegroundColour(wxColour(0,0,0));
	m_pbtnGenerators->SetBackgroundColour(wxColour(255,255,255));
	m_plstGenerators = new wmList(this, ID_M_PLST1, wxPoint(10,60), wxSize(580,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 8, wxSize(2,1));
	m_plstGenerators->SetBackgroundColour(wxColour(0,0,0));
	m_ppnlSettings = new wxPanel(this, ID_PANEL12, wxPoint(0,140), wxSize(600,480), wxTAB_TRAVERSAL, _T("ID_PANEL12"));
	m_ppnlSettings->SetBackgroundColour(wxColour(0,0,0));
	m_plblSelected = new wmLabel(m_ppnlSettings, ID_M_PLBL3, _("Generator #"), wxPoint(0,0), wxSize(600,30), 0, _T("ID_M_PLBL3"));
	m_plblSelected->SetBorderState(uiRect::BORDER_NONE);
	m_plblSelected->GetUiRect().SetGradient(0);
	m_plblSelected->SetForegroundColour(wxColour(255,255,255));
	m_plblSelected->SetBackgroundColour(wxColour(0,128,0));
	m_plstColour = new wmList(m_ppnlSettings, ID_M_PLST5, wxPoint(10,50), wxSize(580,44), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 5, wxSize(2,1));
	m_plstColour->SetBackgroundColour(wxColour(0,0,0));
	StaticBox3 = new wxStaticBox(m_ppnlSettings, ID_STATICBOX3, _("Amplitude"), wxPoint(5,110), wxSize(590,105), 0, _T("ID_STATICBOX3"));
	StaticBox3->SetForegroundColour(wxColour(255,255,255));
	StaticBox3->SetBackgroundColour(wxColour(0,0,0));
	m_pNoiseAmplitude = new wmSlider(m_ppnlSettings,ID_CUSTOM2,wxT("Frequency"),wxPoint(10,150),wxSize(400,40),5,_T("ID_CUSTOM2"));
	m_plblNoisedB = new wmLabel(m_ppnlSettings, ID_M_PLBL1, _("dB"), wxPoint(415,150), wxSize(60,40), 0, _T("ID_M_PLBL1"));
	m_plblNoisedB->SetBorderState(uiRect::BORDER_NONE);
	m_plblNoisedB->GetUiRect().SetGradient(0);
	m_plblNoisedB->SetForegroundColour(wxColour(0,0,0));
	m_plblNoisedB->SetBackgroundColour(wxColour(255,255,255));
	m_pbtnNoise0dBu = new wmButton(m_ppnlSettings, ID_M_PBTN21, _("-18 dBFS"), wxPoint(480,150), wxSize(80,40), 0, wxDefaultValidator, _T("ID_M_PBTN21"));

	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&noiseGeneratorPanel::OnpbtnGeneratorsClick);
	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&noiseGeneratorPanel::OnlstGeneratorsSelected);
	//*)

	m_ppnlSettings->Show(false);

	m_pNoiseAmplitude->Init(0,80, -18.0);
    m_plblNoisedB->SetLabel("-18.0 dBFS");
    Connect(m_pNoiseAmplitude->GetId(), wxEVT_SLIDER_MOVE, (wxObjectEventFunction)&noiseGeneratorPanel::OnNoiseAmplitudeMove);
    Connect(m_pbtnNoise0dBu->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&noiseGeneratorPanel::OnbtnNoise0dBuClicked);
    Connect(ID_M_PLST5,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&noiseGeneratorPanel::OnlstColourSelected);

	m_pbtnGenerators->ConnectToSetting(m_pBuilder->GetSection(), "count", "1");
    m_pbtnGenerators->SetPopup({"0","1","2","3","4","5","6","7","8"});


    for(size_t i = 0; i < 8; i++)
    {
        m_plstGenerators->AddButton("", wxNullBitmap, 0, wmList::wmHIDDEN);
    }

    m_plstColour->AddButton("White");
    m_plstColour->AddButton("Pink");
    m_plstColour->AddButton("Grey");
    m_plstColour->AddButton("A");
    m_plstColour->AddButton("K");

	SetSize(size);
	SetPosition(pos);

    LoadSettings();


}

noiseGeneratorPanel::~noiseGeneratorPanel()
{
	//(*Destroy(noiseGeneratorPanel)
	//*)
}



void noiseGeneratorPanel::LoadSettings()
{
    m_asGenerators = wxStringTokenize(m_pBuilder->ReadSetting("generators",""), ",");
    ExtractAmplitudes(m_pBuilder->ReadSetting("amplitudes", ""));

    PopulateGeneratorList();
}

void noiseGeneratorPanel::OnSettingEvent(SettingEvent& event)
{
    if(event.GetKey() == "generators")
    {
        m_asGenerators = wxStringTokenize(event.GetValue(), ",");
        PopulateGeneratorList();
    }
    else if(event.GetKey() == "amplitudes")
    {
        ExtractAmplitudes(event.GetValue());
    }
    else if(event.GetKey() == "count")
    {

        while(event.GetValue(0l) > m_asGenerators.GetCount())
        {
            m_asGenerators.Add("White");
            m_adAmplitude.Add(-18.0);
        }

        while(m_asGenerators.GetCount() > event.GetValue(0l))
        {
            m_asGenerators.RemoveAt(m_asGenerators.GetCount()-1);
            m_adAmplitude.RemoveAt(m_adAmplitude.GetCount()-1);
        }


        WriteGenerators();

        PopulateGeneratorList();
    }
}

void noiseGeneratorPanel::ExtractAmplitudes(const wxString& sValue)
{
    m_adAmplitude.clear();
    wxArrayString as = wxStringTokenize(sValue, ",");
    for(size_t i = 0; i < as.GetCount(); i++)
    {
        double dValue(0.0);
        as[i].ToDouble(&dValue);
        m_adAmplitude.Add(dValue);
    }
}

void noiseGeneratorPanel::OnlstGeneratorsSelected(wxCommandEvent& event)
{
    m_nSelected = event.GetInt();
    if(m_nSelected < m_asGenerators.GetCount())
    {
        m_ppnlSettings->Show();
        m_plstColour->SelectButton(m_asGenerators[m_nSelected]);
        double dPos = m_adAmplitude[m_nSelected]+80.0;
        m_pNoiseAmplitude->SetSliderPosition(dPos, true);
    }
}

void noiseGeneratorPanel::OnpbtnGeneratorsClick(wxCommandEvent& event)
{
    unsigned long nCount;
    event.GetString().ToULong(&nCount);
    while(nCount > m_asGenerators.GetCount())
    {
        m_asGenerators.Add("White");
    }

    while(nCount > m_adAmplitude.GetCount())
    {
        m_adAmplitude.Add(-18.0);
    }

    while(m_asGenerators.GetCount() > nCount)
    {
        m_asGenerators.RemoveAt(m_asGenerators.GetCount()-1);
    }

    while(m_adAmplitude.GetCount() > nCount)
    {
        m_adAmplitude.RemoveAt(m_adAmplitude.GetCount()-1);
    }


    WriteGenerators();
    PopulateGeneratorList();
}

void noiseGeneratorPanel::WriteGenerators()
{
    wxString sValue;
    for(size_t i = 0; i < m_asGenerators.GetCount(); i++)
    {
        if(sValue.empty() == false)
        {
            sValue += ",";
        }
        sValue += m_asGenerators[i];
    }
    m_pBuilder->WriteSetting("generators", sValue);

    WriteAmplitudes();
}

void noiseGeneratorPanel::WriteAmplitudes()
{
    wxString sValue;
    for(size_t i = 0; i < m_adAmplitude.GetCount(); i++)
    {
        if(sValue.empty() == false)
        {
            sValue += ",";
        }
        sValue += wxString::Format("%.2f", m_adAmplitude[i]);
    }
    m_pBuilder->WriteSetting("amplitudes", sValue);
}

void noiseGeneratorPanel::PopulateGeneratorList()
{
    m_plstGenerators->Freeze();

    for(size_t i = 0; i < m_plstGenerators->GetItemCount(); i++)
    {
        if(i < m_asGenerators.GetCount())
        {
            m_plstGenerators->SetButtonText(i, m_asGenerators[i]);
            m_plstGenerators->EnableButton(i, wmList::wmENABLED);
        }
        else
        {
            m_plstGenerators->EnableButton(i, wmList::wmHIDDEN);
            if(m_nSelected == i)
            {
                m_ppnlSettings->Show(false);
                m_nSelected = -1;
            }
        }
    }
    m_plstGenerators->Thaw();
}

void noiseGeneratorPanel::OnNoiseAmplitudeMove(wxCommandEvent& event)
{
    if(m_nSelected > -1)
    {
        m_adAmplitude[m_nSelected] = m_pNoiseAmplitude->GetPosition()-80.0;
        m_plblNoisedB->SetLabel(wxString::Format("%.1f dBFS", m_adAmplitude[m_nSelected]));
        WriteAmplitudes();
    }
}


void noiseGeneratorPanel::OnlstColourSelected(wxCommandEvent& event)
{
    if(m_nSelected > -1)
    {
        m_asGenerators[m_nSelected] = event.GetString();
        WriteGenerators();
    }
}

void noiseGeneratorPanel::OnbtnNoise0dBuClicked(wxCommandEvent& event)
{
    if(m_nSelected > -1)
    {
        m_adAmplitude[m_nSelected] = -18.0;
        m_plblNoisedB->SetLabel(wxString::Format("%.1f dBFS", m_adAmplitude[m_nSelected]));
        m_pNoiseAmplitude->SetSliderPosition(62.0,false);
        WriteAmplitudes();
    }
}
