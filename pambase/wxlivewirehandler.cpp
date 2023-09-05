#include "wxlivewirehandler.h"
#include "livewiremessage.h"

wxDEFINE_EVENT(wxEVT_LIVEWIRE, LivewireEvent);

wxLivewireHandler::wxLivewireHandler(wxEvtHandler* pHandler) :
    m_pHandler(pHandler)
    {

    }

void wxLivewireHandler::Message(const pml::livewire::message& aMessage)
{
    if(m_pHandler)
    {
        auto pEvent = new LivewireEvent(aMessage);
        wxQueueEvent(m_pHandler,pEvent);
    }
}


LivewireEvent::LivewireEvent() : wxCommandEvent(wxEVT_LIVEWIRE)
{

}

LivewireEvent::LivewireEvent(const pml::livewire::message& aMessage)  : wxCommandEvent(wxEVT_LIVEWIRE),
m_message(aMessage)
{

}
    
LivewireEvent::LivewireEvent(const LivewireEvent& event) :
m_message(event.GetMessage())
{

}