#include "pnlSettingsInputNmos.h"
#include "settings.h"
#include "settingevent.h"

//(*InternalHeaders(pnlSettingsInputNmos)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlSettingsInputNmos)
const long pnlSettingsInputNmos::ID_M_PLST1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlSettingsInputNmos,wxPanel)
	//(*EventTable(pnlSettingsInputNmos)
	//*)
END_EVENT_TABLE()

pnlSettingsInputNmos::pnlSettingsInputNmos(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, int nStyle, const wxString& sName)
{
	//(*Initialize(pnlSettingsInputNmos)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstSenders = new wmList(this, ID_M_PLST1, wxPoint(10,10), wxSize(580,290), 0, 0, wxSize(-1,40), 3, wxSize(1,1));
	m_plstSenders->SetBackgroundColour(wxColour(0,0,0));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlSettingsInputNmos::OnlstSendersSelected);
	//*)
	SetSize(size);
	SetPosition(pos);
	#ifdef __NMOS__
	m_plstSenders->SetButtonFactory(new wmSenderButtonFactory());
	#endif // __NMOS__
    m_plstSenders->SetBackgroundColour(wxColour(0,0,0));

    m_plstSenders->SetSelectedButtonColour(wxColour(wxT("#008000")));
    m_plstSenders->SetDisabledColour(wxColour(wxT("#808080")));
    m_plstSenders->AddButton(wxT("PARK"));

    m_plstSenders->Show(Settings::Get().Read("NMOS", "Client", NmosManager::CLIENT_OFF) != NmosManager::CLIENT_OFF &&
                        (Settings::Get().Read("NMOS", "Node", NmosManager::NODE_OFF) == NmosManager::NODE_RECEIVER ||
                        (Settings::Get().Read("NMOS", "Node", NmosManager::NODE_OFF) == NmosManager::NODE_BOTH)));

    Settings::Get().AddHandler("NMOS", "Node", this);
    Settings::Get().AddHandler("NMOS", "Client", this);

    Bind(wxEVT_SETTING_CHANGED, &pnlSettingsInputNmos::OnSettingChanged, this);
}

void pnlSettingsInputNmos::OnSettingChanged(SettingEvent& event)
{
    m_plstSenders->Show(Settings::Get().Read("NMOS", "Client", NmosManager::CLIENT_OFF) != NmosManager::CLIENT_OFF &&
                        (Settings::Get().Read("NMOS", "Node", NmosManager::NODE_OFF) == NmosManager::NODE_RECEIVER ||
                        (Settings::Get().Read("NMOS", "Node", NmosManager::NODE_OFF) == NmosManager::NODE_BOTH)));

}

pnlSettingsInputNmos::~pnlSettingsInputNmos()
{
	//(*Destroy(pnlSettingsInputNmos)
	//*)
}


void pnlSettingsInputNmos::OnlstSendersSelected(wxCommandEvent& event)
{
    #ifdef __NMOS__
    bool bAsked(false);
    switch(Settings::Get().Read(wxT("NMOS"), wxT("Client"), 0))
    {
        case NmosManager::CLIENT_IS04:
            bAsked = ConnectionIS04(event.GetInt());
            break;
        case NmosManager::CLIENT_IS05:
            bAsked = ConnectionIS05(event.GetInt());
            break;
    }

    uiSender* pSender = dynamic_cast<uiSender*>(m_plstSenders->GetButtonuiRect(event.GetInt()));
    if(pSender)
    {
        if(bAsked)
        {
            pSender->SetState(uiSender::STATE_ASKED);
        }
        else
        {
            pSender->SetState(uiSender::STATE_ERROR);
        }
        m_plstSenders->Refresh();
    }
    #endif // __NMOS__
}

bool pnlSettingsInputNmos::ConnectionIS04(size_t nSenderButton)
{
    #ifdef __NMOS__
    if(m_plstSenders->GetButtonAuxillaryText(nSenderButton).empty() == false)
    {
        return ClientApi::Get().Subscribe(std::string(m_plstSenders->GetButtonAuxillaryText(nSenderButton).mb_str()),std::string(m_sReceiverId.mb_str()));
    }

    return ClientApi::Get().Unsubscribe(std::string(m_sReceiverId.mb_str()));
    #else
    return false;
    #endif // __NMOS__
}

bool pnlSettingsInputNmos::ConnectionIS05(size_t nSenderButton)
{
    #ifdef __NMOS__
    if(m_plstSenders->GetButtonAuxillaryText(nSenderButton).empty() == false)
    {
        return ClientApi::Get().Connect(std::string(m_plstSenders->GetButtonAuxillaryText(nSenderButton).mb_str()),std::string(m_sReceiverId.mb_str()));
    }

    return ClientApi::Get().Disconnect(std::string(m_sReceiverId.mb_str()));
    #else
    return false;
    #endif // __NMOS__
}

#ifdef __NMOS__
void pnlSettingsInputNmos::AddSender(std::shared_ptr<Sender> pSender)
{
    size_t nIndex = m_plstSenders->AddButton(wxString::FromUTF8(pSender->GetLabel().c_str()));
    m_plstSenders->SetButtonAuxillaryText(nIndex, wxString::FromUTF8(pSender->GetId().c_str()));
    m_plstSenders->Refresh();

}

void pnlSettingsInputNmos::UpdateSender(std::shared_ptr<Sender> pSender)
{

}

void pnlSettingsInputNmos::RemoveSenders(const std::set<std::string>::const_iterator& itBegin, const std::set<std::string>::const_iterator& itEnd)
{
    for(std::set<std::string>::const_iterator itSender = itBegin; itSender != itEnd; ++itSender)
    {
        if((*itSender).empty() == false)
        {
            for(size_t i = 0; i < m_plstSenders->GetMaxIndex(); i++)
            {

                if(m_plstSenders->GetButtonAuxillaryText(i) == (*itSender))
                {
                    m_plstSenders->DeleteButton(i);
                    break;
                }
            }
        }
    }
}

void pnlSettingsInputNmos::SetSender(const wxString& sSenderId)
{
    for(size_t i = 0; i < m_plstSenders->GetMaxIndex(); i++)
    {
        uiSender* pSender = dynamic_cast<uiSender*>(m_plstSenders->GetButtonuiRect(i));
        if(pSender)
        {
            if(m_plstSenders->GetButtonAuxillaryText(i) == sSenderId)
            {
                pSender->SetState(uiSender::STATE_ACTIVE);
            }
            else
            {
                pSender->SetState(uiSender::STATE_IDLE);
            }
        }
    }
    m_plstSenders->Refresh();
}

void pnlSettingsInputNmos::SubscriptionRequest(const wxString& sReceiverId, const wxString& sResponse, unsigned long nResult)
{
    if(nResult == 202)  //success
    {
        for(size_t i = 0; i < m_plstSenders->GetMaxIndex(); i++)
        {
            uiSender* pSender = dynamic_cast<uiSender*>(m_plstSenders->GetButtonuiRect(i));
            if(pSender)
            {
                if(m_plstSenders->GetButtonAuxillaryText(i) == sResponse)
                {
                    pSender->SetState(uiSender::STATE_REPLIED);
                    m_plstSenders->Refresh();
                    break;
                }
            }
        }
    }
    else    //not worked
    {
        for(size_t i = 0; i < m_plstSenders->GetMaxIndex(); i++)
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
