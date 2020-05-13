#include "wxsaphandler.h"
#include "sap.h"

wxDEFINE_EVENT(wxEVT_SAP, wxCommandEvent);

wxSapHandler::wxSapHandler(wxEvtHandler* pHandler) :
    m_pHandler(pHandler)
    {

    }

void wxSapHandler::SapMessage(const pml::sap& aMessage)
{
    if(m_pHandler)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_SAP);
        pEvent->SetInt((aMessage.bDeletionMessage == false));
        pEvent->SetString((wxString(aMessage.sSenderIp)+"\n"+wxString(aMessage.sData)));
        wxQueueEvent(m_pHandler,pEvent);
    }
}
