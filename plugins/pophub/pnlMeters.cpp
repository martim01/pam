#include "pnlMeters.h"

#include <wx/intl.h>
#include <wx/string.h>

#include "dlgEditName.h"

#include "pophubbuilder.h"
#include "settingevent.h"
#include "settings.h"


BEGIN_EVENT_TABLE(pnlMeters,wxPanel)
END_EVENT_TABLE()

pnlMeters::pnlMeters(wxWindow* parent,pophubBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

	m_plblLeft = new wmLabel(this, wxID_ANY, "Left", wxPoint(2,0), wxSize(96,25), 0, _T("ID_M_PLBL1"));
	m_plblRight = new wmLabel(this, wxID_ANY, "Right", wxPoint(102,0), wxSize(96,25), 0, _T("ID_M_PLBL1"));
	
	m_plblLeft->SetBorderState(uiRect::BORDER_NONE);
	m_plblLeft->SetForegroundColour(wxColour(255,255,255));
	m_plblLeft->SetBackgroundColour(wxColour(63,115,192));

	m_plblRight->SetBorderState(uiRect::BORDER_NONE);
	m_plblRight->SetForegroundColour(wxColour(255,255,255));
	m_plblRight->SetBackgroundColour(wxColour(63,115,192));

	m_plstLeft = new wmList(this, wxID_ANY, wxPoint(0,30), wxSize(96,92), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 1, wxSize(5,5));
	m_plstRight = new wmList(this, wxID_ANY, wxPoint(102,30), wxSize(96,92), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 1, wxSize(5,5));
	m_plstLeft->SetBackgroundColour(*wxBLACK);
	m_plstRight->SetBackgroundColour(*wxBLACK);

	m_plstLeft->AddButton("Stereo");
	m_plstLeft->AddButton("M+S");
	
	m_plstRight->AddButton("Stereo");
	m_plstRight->AddButton("M+S");

	m_plstLeft->ConnectToSetting(m_pBuilder->GetSection(), "meter_left", "Stereo");
	m_plstRight->ConnectToSetting(m_pBuilder->GetSection(), "meter_right", "Stereo");
	HideButtons(m_pBuilder->ReadSetting("view", 0));

	m_pbtnLeft = new wmButton(this, wxID_ANY, "Edit Label", wxPoint(0, 180), wxSize(96, 40));
	m_pbtnRight = new wmButton(this, wxID_ANY, "Edit Label", wxPoint(102, 180), wxSize(96, 40));

	Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event)
	{
		dlgEditName aDlg(this,m_pBuilder->ReadSetting("label_left", "Channels 1+2"));
		if(aDlg.ShowModal() == wxID_OK)
		{
			m_pBuilder->WriteSetting("label_left", aDlg.m_pedtName->GetValue());
		}
	}, m_pbtnLeft->GetId());
	
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, [this](wxCommandEvent& event)
	{
		dlgEditName aDlg(this,m_pBuilder->ReadSetting("label_right", "Channels 3+4"));
		if(aDlg.ShowModal() == wxID_OK)
		{
			m_pBuilder->WriteSetting("label_right", aDlg.m_pedtName->GetValue());
		}
	}, m_pbtnRight->GetId());

	Bind(wxEVT_SETTING_CHANGED, [this](SettingEvent& e) { HideButtons(e.GetValue(0L)); });
}

pnlMeters::~pnlMeters()
{
	//(*Destroy(pnlMeters)
	//*)
}



void pnlMeters::HideButtons(unsigned long nView)
{
	switch(nView)
	{
		case pophubBuilder::enumView::kRadio:
		case pophubBuilder::enumView::kRadioLoudness:		
			m_plstLeft->EnableButton(0, wmList::wmENABLED);
			m_plstLeft->EnableButton(1, wmList::wmENABLED);
			m_plstRight->EnableButton(0, wmList::wmENABLED);
			m_plstRight->EnableButton(1, wmList::wmENABLED);
			m_plstLeft->SelectButton(0);
			m_plstRight->SelectButton(0);
			m_plblRight->Show();
			break;
		case pophubBuilder::enumView::kNews:
		case pophubBuilder::enumView::kNewsLoudness:
		case pophubBuilder::enumView::kNewsGain:
			m_plstLeft->EnableButton(0, wmList::wmENABLED);
			m_plstLeft->EnableButton(1, wmList::wmHIDDEN);
			m_plstRight->EnableButton(0, wmList::wmHIDDEN);
			m_plstRight->EnableButton(1, wmList::wmENABLED);
			m_plstLeft->SelectButton(0);
			m_plstRight->SelectButton(1);
			m_plblRight->Show();
			break;
		case pophubBuilder::enumView::kWorkshop:
		case pophubBuilder::enumView::kTV:
			m_plstLeft->EnableButton(0, wmList::wmENABLED);
			m_plstLeft->EnableButton(1, wmList::wmENABLED);
			m_plstRight->EnableButton(0, wmList::wmHIDDEN);
			m_plstRight->EnableButton(1, wmList::wmHIDDEN);
			m_plstLeft->SelectButton(0);
			m_plblRight->Show(false);
		break;
	}
}