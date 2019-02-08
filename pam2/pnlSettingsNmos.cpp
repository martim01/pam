#ifdef __NMOS__
#include "pnlSettingsNmos.h"
#include "settings.h"
#include "settingevent.h"
#include "sender.h"
#include <wx/log.h>
#include "clientapi.h"
#include <wx/msgdlg.h>
#include "senderbuttonfactory.h"

//(*InternalHeaders(pnlSettingsNmos)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsNmos)
const long pnlSettingsNmos::ID_M_PBTN22 = wxNewId();
const long pnlSettingsNmos::ID_M_PBTN1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsNmos,wxPanel)
	//(*EventTable(pnlSettingsNmos)
	//*)
END_EVENT_TABLE()

pnlSettingsNmos::pnlSettingsNmos(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, long nStyle, const wxString& sId)
{
	//(*Initialize(pnlSettingsNmos)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pbtnNmos = new wmButton(this, ID_M_PBTN22, _("NMOS"), wxPoint(10,10), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN22"));
	m_pbtnNmos->SetForegroundColour(wxColour(255,255,255));
	m_pbtnNmos->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnNmos->SetToggleLook(true, wxT("OFF"), wxT("ON"), 50);
	m_pbtnClient = new wmButton(this, ID_M_PBTN1, _("Client"), wxPoint(10,60), wxSize(200,40), wmButton::STYLE_SELECT, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnClient->SetForegroundColour(wxColour(255,255,255));
	m_pbtnClient->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnClient->SetColourDisabled(wxColour(120,120,120));
	m_pbtnClient->SetToggleLook(true, wxT("OFF"), wxT("ON"), 50);

	Connect(ID_M_PBTN22,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsNmos::OnbtnNmosClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pnlSettingsNmos::OnbtnClientClick);
	//*)

	m_plstSenders = new wmList(this, wxNewId(), wxPoint(0,110), wxSize(600,260), wmList::STYLE_NORMAL, 2, wxSize(-1,40), 3, wxSize(2,2));
	m_plstSenders->SetButtonFactory(new wmSenderButtonFactory());
    m_plstSenders->SetBackgroundColour(wxColour(0,0,0));

    m_plstSenders->SetSelectedButtonColour(wxColour(wxT("#008000")));
    m_plstSenders->SetDisabledColour(wxColour(wxT("#808080")));

    Connect(m_plstSenders->GetId(), wxEVT_LIST_SELECTED, (wxObjectEventFunction)&pnlSettingsNmos::OnSenderSelected);

    m_plstSenders->Show(Settings::Get().Read(wxT("NMOS"), wxT("Activate"), false));
    m_pbtnClient->Enable(Settings::Get().Read(wxT("NMOS"), wxT("Activate"), false));
	m_pbtnNmos->ToggleSelection(Settings::Get().Read(wxT("NMOS"), wxT("Activate"), false), true);
	m_pbtnClient->ToggleSelection(Settings::Get().Read(wxT("NMOS"), wxT("Client"), false));
}

pnlSettingsNmos::~pnlSettingsNmos()
{
	//(*Destroy(pnlSettingsNmos)
	//*)
}


void pnlSettingsNmos::OnbtnNmosClick(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("NMOS"), wxT("Activate"), event.IsChecked());
    m_pbtnClient->Enable(event.IsChecked());
    m_plstSenders->Show(Settings::Get().Read(wxT("NMOS"), wxT("Activate"), false));
}

void pnlSettingsNmos::OnbtnClientClick(wxCommandEvent& event)
{
    Settings::Get().Write(wxT("NMOS"), wxT("Client"), event.IsChecked());
}

void pnlSettingsNmos::OnSenderSelected(wxCommandEvent& event)
{
    if(ClientApi::Get().Subscribe(std::string(m_plstSenders->GetButtonAuxillaryText(event.GetInt()).mb_str()),std::string(m_sReceiverId.mb_str())))
    {
        uiSender* pSender = dynamic_cast<uiSender*>(m_plstSenders->GetButtonuiRect(event.GetInt()));
        if(pSender)
        {
            pSender->SetState(uiSender::STATE_ASKED);
            m_plstSenders->Refresh();
        }
    }
    else
    {
        uiSender* pSender = dynamic_cast<uiSender*>(m_plstSenders->GetButtonuiRect(event.GetInt()));
        if(pSender)
        {
            pSender->SetState(uiSender::STATE_ERROR);
            m_plstSenders->Refresh();
        }
    }
}


void pnlSettingsNmos::AddSender(std::shared_ptr<Sender> pSender)
{
    wxLogDebug(wxT("pnlSettingsNmos::AddSender"));
    size_t nIndex = m_plstSenders->AddButton(wxString::FromAscii(pSender->GetLabel().c_str()));
    m_plstSenders->SetButtonAuxillaryText(nIndex, wxString::FromAscii(pSender->GetId().c_str()));
}

void pnlSettingsNmos::UpdateSender(std::shared_ptr<Sender> pSender)
{

}

void pnlSettingsNmos::SetSender(const wxString& sSenderId)
{
    for(size_t i = 0; i < m_plstSenders->GetItemCount(); i++)
    {
        if(m_plstSenders->GetButtonAuxillaryText(i) == sSenderId)
        {
            uiSender* pSender = dynamic_cast<uiSender*>(m_plstSenders->GetButtonuiRect(i));
            if(pSender)
            {
                pSender->SetState(uiSender::STATE_ACTIVE);
            }
        }
        else
        {
            uiSender* pSender = dynamic_cast<uiSender*>(m_plstSenders->GetButtonuiRect(i));
            if(pSender)
            {
                pSender->SetState(uiSender::STATE_IDLE);
            }
        }
    }
    m_plstSenders->Refresh();
}

void pnlSettingsNmos::SubscriptionRequest(const wxString& sReceiverId, const wxString& sResponse, unsigned long nResult)
{
    wxLogDebug(sResponse);
    if(nResult == 202)  //success
    {
        for(size_t i = 0; i < m_plstSenders->GetItemCount(); i++)
        {
            if(m_plstSenders->GetButtonAuxillaryText(i) == sResponse)
            {
                uiSender* pSender = dynamic_cast<uiSender*>(m_plstSenders->GetButtonuiRect(i));
                if(pSender)
                {
                    pSender->SetState(uiSender::STATE_REPLIED);
                }
                m_plstSenders->Refresh();
                break;
            }
        }
    }
    else    //not worked
    {
        for(size_t i = 0; i < m_plstSenders->GetItemCount(); i++)
        {
            uiSender* pSender = dynamic_cast<uiSender*>(m_plstSenders->GetButtonuiRect(i));
            if(pSender)
            {
                pSender->SetState(uiSender::STATE_ERROR);
            }
            m_plstSenders->Refresh();
        }
        wxMessageBox(sResponse);
    }
}
#endif
