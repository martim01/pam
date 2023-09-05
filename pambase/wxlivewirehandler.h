#pragma once
#include <wx/event.h>
#include "livewiremessage.h"
#include "livewirehandler.h"
#include "dlldefine.h"

class PAMBASE_IMPEXPORT wxLivewireHandler : public pml::livewire::Handler
{
    public:
        explicit wxLivewireHandler(wxEvtHandler* pHandler);
        void Message(const pml::livewire::message& aMessage);
    private:
        wxEvtHandler* m_pHandler;

};


class PAMBASE_IMPEXPORT LivewireEvent : public wxCommandEvent
{

public:
    /** @brief	Constructor
    *   @param commandType should be wxEVT_NI...
    *   @param id the ID of the button list control
    **/
    LivewireEvent();

    LivewireEvent(const pml::livewire::message& aMessage);
    /** @brief	Copy Constructor
    *   @param event a wxNIEvent
    **/
    LivewireEvent(const LivewireEvent& event);

    /** @brief	Destructor
    **/
    virtual ~LivewireEvent(){}

    /** @brief	Creates a copy of the wxNIEvent
    *   @return <i>wxNIEvent</i>
    **/
    virtual wxEvent *Clone() const { return new LivewireEvent(*this); }

    //wxDECLARE_DYNAMIC_CLASS(MonitorEvent)

    const pml::livewire::message& GetMessage() const { return m_message;}

protected:
    pml::livewire::message m_message;
};


wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_LIVEWIRE, LivewireEvent);
