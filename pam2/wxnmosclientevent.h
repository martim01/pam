#pragma once
#ifdef __NMOS__
#include "clientapiposter.h"

class wxNmosClientNodeEvent;
class wxNmosClientSourceEvent;
class wxNmosClientFlowEvent;
class wxNmosClientDeviceEvent;
class wxNmosClientSenderEvent;
class wxNmosClientReceiverEvent;

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_NODE, wxNmosClientNodeEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_SOURCE, wxNmosClientSourceEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_FLOW, wxNmosClientFlowEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_DEVICE, wxNmosClientDeviceEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_SENDER, wxNmosClientSenderEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_NMOS_CLIENT_RECEIVER, wxNmosClientReceiverEvent);


class wxNmosClientNodeEvent : public wxCommandEvent
{
    public:
        wxNmosClientNodeEvent(const pml::nmos::resourcechanges<pml::nmos::Self>& changed) : wxCommandEvent(wxEVT_NMOS_CLIENT_NODE), m_changed(changed){}
        wxNmosClientNodeEvent(const wxNmosClientNodeEvent& event) : wxCommandEvent(event), m_changed(event.GetChanges()){}
        wxNmosClientNodeEvent() : wxCommandEvent(){}
        ~wxNmosClientNodeEvent(){}
        virtual wxEvent *Clone() const { return new wxNmosClientNodeEvent(*this); }

        const pml::nmos::resourcechanges<pml::nmos::Self>& GetChanges() const { return m_changed;}

        DECLARE_DYNAMIC_CLASS(wxNmosClientNodeEvent)

    private:
        pml::nmos::resourcechanges<pml::nmos::Self> m_changed;
};

class wxNmosClientSourceEvent : public wxCommandEvent
{
    public:
        wxNmosClientSourceEvent(const pml::nmos::resourcechanges<pml::nmos::Source>& changed) : wxCommandEvent(wxEVT_NMOS_CLIENT_SOURCE), m_changed(changed){}
        wxNmosClientSourceEvent(const wxNmosClientSourceEvent& event) : wxCommandEvent(event), m_changed(event.GetChanges()){}
        wxNmosClientSourceEvent() : wxCommandEvent(){}
        ~wxNmosClientSourceEvent(){}
        virtual wxEvent *Clone() const { return new wxNmosClientSourceEvent(*this); }

        const pml::nmos::resourcechanges<pml::nmos::Source>& GetChanges() const { return m_changed;}

        DECLARE_DYNAMIC_CLASS(wxNmosClientSourceEvent)

    private:
        pml::nmos::resourcechanges<pml::nmos::Source> m_changed;
};

class wxNmosClientDeviceEvent : public wxCommandEvent
{
    public:
        wxNmosClientDeviceEvent(const pml::nmos::resourcechanges<pml::nmos::Device>& changed) : wxCommandEvent(wxEVT_NMOS_CLIENT_DEVICE), m_changed(changed){}
        wxNmosClientDeviceEvent(const wxNmosClientDeviceEvent& event) : wxCommandEvent(event), m_changed(event.GetChanges()){}
        wxNmosClientDeviceEvent() : wxCommandEvent(){}
        ~wxNmosClientDeviceEvent(){}
        virtual wxEvent *Clone() const { return new wxNmosClientDeviceEvent(*this); }

        const pml::nmos::resourcechanges<pml::nmos::Device>& GetChanges() const { return m_changed;}

        DECLARE_DYNAMIC_CLASS(wxNmosClientDeviceEvent)

    private:
        pml::nmos::resourcechanges<pml::nmos::Device> m_changed;
};

class wxNmosClientFlowEvent : public wxCommandEvent
{
    public:
        wxNmosClientFlowEvent(const pml::nmos::resourcechanges<pml::nmos::Flow>& changed) : wxCommandEvent(wxEVT_NMOS_CLIENT_FLOW), m_changed(changed){}
        wxNmosClientFlowEvent(const wxNmosClientFlowEvent& event) : wxCommandEvent(event), m_changed(event.GetChanges()){}
        wxNmosClientFlowEvent() : wxCommandEvent(){}
        ~wxNmosClientFlowEvent(){}
        virtual wxEvent *Clone() const { return new wxNmosClientFlowEvent(*this); }

        const pml::nmos::resourcechanges<pml::nmos::Flow>& GetChanges() const { return m_changed;}

        DECLARE_DYNAMIC_CLASS(wxNmosClientFlowEvent)

    private:
        pml::nmos::resourcechanges<pml::nmos::Flow> m_changed;
};

class wxNmosClientSenderEvent : public wxCommandEvent
{
    public:
        wxNmosClientSenderEvent(const pml::nmos::resourcechanges<pml::nmos::Sender>& changed) : wxCommandEvent(wxEVT_NMOS_CLIENT_SENDER), m_changed(changed){}
        wxNmosClientSenderEvent(const wxNmosClientSenderEvent& event) : wxCommandEvent(event), m_changed(event.GetChanges()){}
        wxNmosClientSenderEvent() : wxCommandEvent(){}
        ~wxNmosClientSenderEvent(){}
        virtual wxEvent *Clone() const { return new wxNmosClientSenderEvent(*this); }

        const pml::nmos::resourcechanges<pml::nmos::Sender>& GetChanges() const { return m_changed;}

        DECLARE_DYNAMIC_CLASS(wxNmosClientSenderEvent)

    private:
        pml::nmos::resourcechanges<pml::nmos::Sender> m_changed;
};

class wxNmosClientReceiverEvent : public wxCommandEvent
{
    public:
        wxNmosClientReceiverEvent(const pml::nmos::resourcechanges<pml::nmos::Receiver>& changed) : wxCommandEvent(wxEVT_NMOS_CLIENT_RECEIVER), m_changed(changed){}
        wxNmosClientReceiverEvent(const wxNmosClientReceiverEvent& event) : wxCommandEvent(event), m_changed(event.GetChanges()){}
        wxNmosClientReceiverEvent() : wxCommandEvent(){}
        ~wxNmosClientReceiverEvent(){}
        virtual wxEvent *Clone() const { return new wxNmosClientReceiverEvent(*this); }

        const pml::nmos::resourcechanges<pml::nmos::Receiver>& GetChanges() const { return m_changed;}

        DECLARE_DYNAMIC_CLASS(wxNmosClientReceiverEvent)

    private:
        pml::nmos::resourcechanges<pml::nmos::Receiver> m_changed;
};

#endif // __NMOS__
