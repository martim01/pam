#include "pnlOffset.h"
#include "scopebuilder.h"
#include <wx/tokenzr.h>
#include <wx/intl.h>
#include <wx/string.h>


BEGIN_EVENT_TABLE(pnlOffset,wxPanel)
END_EVENT_TABLE()

pnlOffset::pnlOffset(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	m_plblOffset = new wmLabel(this, wxNewId(), "Channel", wxPoint(0,0), wxSize(190,30));
	m_plblOffset->SetBackgroundColour(wxColour(80,100,128));
	m_plblOffset->SetForegroundColour(*wxWHITE);
	m_plstChannels = new wmList(this, wxNewId(), wxPoint(0,35), wxSize(190,72), wmList::STYLE_SELECT, 0, wxSize(-1,35), 4, wxSize(0,0));
	m_plstChannels->SetBackgroundColour(wxColour(0,0,0));

	m_plblMove = new wmLabel(this, wxNewId(), "Move", wxPoint(0,120), wxSize(190,30));
	m_plblMove->SetBackgroundColour(wxColour(80,100,128));
	m_plblMove->SetForegroundColour(*wxWHITE);

	m_pbtnDown5 = new wmButton(this, wxNewId(), "-5", wxPoint(0,165), wxSize(40,35), wmButton::STYLE_REPEAT);
	m_pbtnDown = new wmButton(this, wxNewId(), "-1", wxPoint(40,165), wxSize(40,35), wmButton::STYLE_REPEAT);
	m_pbtnReset = new wmButton(this, wxNewId(), "Zero", wxPoint(80,165), wxSize(40,35), wmButton::STYLE_NORMAL);
	m_pbtnUp = new wmButton(this, wxNewId(), "+1", wxPoint(120,165), wxSize(40,35), wmButton::STYLE_REPEAT);
	m_pbtnUp5 = new wmButton(this, wxNewId(), "+5", wxPoint(160,165), wxSize(40,35), wmButton::STYLE_REPEAT);



	Bind(wxEVT_LIST_SELECTED, &pnlOffset::OnlstChannelSelected, this, m_plstChannels->GetId());
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlOffset::OnbtnDownPressed, this, m_pbtnDown->GetId());
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlOffset::OnbtnDown5Pressed, this, m_pbtnDown5->GetId());
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlOffset::OnbtnResetPressed, this, m_pbtnReset->GetId());
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlOffset::OnbtnUpPressed, this, m_pbtnUp->GetId());
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &pnlOffset::OnbtnUp5Pressed, this, m_pbtnUp5->GetId());

	m_plstChannels->SelectButton(0, true);
}

pnlOffset::~pnlOffset()
{
	//(*Destroy(pnlOffset)
	//*)
}


void pnlOffset::OnlstChannelSelected(wxCommandEvent& event)
{
    m_nChannel = event.GetInt();
}



void pnlOffset::SetNumberOfChannels(unsigned int nChannels)
{
    m_plstChannels->Freeze();
    m_plstChannels->Clear();
    for(int i = 0; i < nChannels; i++)
    {
        m_plstChannels->AddButton(wxString::Format("Ch %d", i+1));
    }
    m_plstChannels->SelectButton(0);
    m_plstChannels->Thaw();
}

void pnlOffset::OnbtnDownPressed(wxCommandEvent& event)
{
    wxString sChannel =wxString::Format("Plot_Offset_%lu", m_nChannel);
    m_pBuilder->WriteSetting(sChannel, m_pBuilder->ReadSetting(sChannel, 0)-1);
}

void pnlOffset::OnbtnUpPressed(wxCommandEvent& event)
{
    wxString sChannel =wxString::Format("Plot_Offset_%lu", m_nChannel);
    m_pBuilder->WriteSetting(sChannel, m_pBuilder->ReadSetting(sChannel, 0)+1);
}

void pnlOffset::OnbtnDown5Pressed(wxCommandEvent& event)
{
    wxString sChannel =wxString::Format("Plot_Offset_%lu", m_nChannel);
    m_pBuilder->WriteSetting(sChannel, m_pBuilder->ReadSetting(sChannel, 0)-5);
}

void pnlOffset::OnbtnUp5Pressed(wxCommandEvent& event)
{
    wxString sChannel =wxString::Format("Plot_Offset_%lu", m_nChannel);
    m_pBuilder->WriteSetting(sChannel, m_pBuilder->ReadSetting(sChannel, 0)+5);
}

void pnlOffset::OnbtnResetPressed(wxCommandEvent& event)
{
    wxString sChannel =wxString::Format("Plot_Offset_%lu", m_nChannel);
    m_pBuilder->WriteSetting(sChannel, 0);
}
