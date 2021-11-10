#pragma once
#ifdef __NMOS__


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

        wxNmosClientNodeEvent(const typename std::list<std::shared_ptr<pml::nmos::Self> >& lstAdded, const typename std::list<std::shared_ptr<pml::nmos::Self> >& lstUpdated, const typename std::list<std::shared_ptr<pml::nmos::Self> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_NODE), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientNodeEvent(const wxNmosClientNodeEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<pml::nmos::Self> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<pml::nmos::Self> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<pml::nmos::Self> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientNodeEvent() : wxCommandEvent(){}
        ~wxNmosClientNodeEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientNodeEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientNodeEvent)

    private:
        std::list<std::shared_ptr<pml::nmos::Self> > m_lstAdded;
        std::list<std::shared_ptr<pml::nmos::Self> > m_lstUpdated;
        std::list<std::shared_ptr<pml::nmos::Self> > m_lstRemoved;
};


class wxNmosClientDeviceEvent : public wxCommandEvent
{

    public:

        wxNmosClientDeviceEvent(const typename std::list<std::shared_ptr<pml::nmos::Device> >& lstAdded, const typename std::list<std::shared_ptr<pml::nmos::Device> >& lstUpdated, const typename std::list<std::shared_ptr<pml::nmos::Device> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_DEVICE), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientDeviceEvent(const wxNmosClientDeviceEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<pml::nmos::Device> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<pml::nmos::Device> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<pml::nmos::Device> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientDeviceEvent() : wxCommandEvent(){}
        ~wxNmosClientDeviceEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientDeviceEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientDeviceEvent)

    private:
        std::list<std::shared_ptr<pml::nmos::Device> > m_lstAdded;
        std::list<std::shared_ptr<pml::nmos::Device> > m_lstUpdated;
        std::list<std::shared_ptr<pml::nmos::Device> > m_lstRemoved;
};


class wxNmosClientSourceEvent : public wxCommandEvent
{

    public:

        wxNmosClientSourceEvent(const typename std::list<std::shared_ptr<pml::nmos::Source> >& lstAdded, const typename std::list<std::shared_ptr<pml::nmos::Source> >& lstUpdated, const typename std::list<std::shared_ptr<pml::nmos::Source> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_SOURCE), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientSourceEvent(const wxNmosClientSourceEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<pml::nmos::Source> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<pml::nmos::Source> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<pml::nmos::Source> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientSourceEvent() : wxCommandEvent(){}
        ~wxNmosClientSourceEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientSourceEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientSourceEvent)

    private:
        std::list<std::shared_ptr<pml::nmos::Source> > m_lstAdded;
        std::list<std::shared_ptr<pml::nmos::Source> > m_lstUpdated;
        std::list<std::shared_ptr<pml::nmos::Source> > m_lstRemoved;
};


class wxNmosClientFlowEvent : public wxCommandEvent
{

    public:

        wxNmosClientFlowEvent(const typename std::list<std::shared_ptr<pml::nmos::Flow> >& lstAdded, const typename std::list<std::shared_ptr<pml::nmos::Flow> >& lstUpdated, const typename std::list<std::shared_ptr<pml::nmos::Flow> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_FLOW), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientFlowEvent(const wxNmosClientFlowEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<pml::nmos::Flow> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<pml::nmos::Flow> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<pml::nmos::Flow> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientFlowEvent() : wxCommandEvent(){}
        ~wxNmosClientFlowEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientFlowEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientFlowEvent)

    private:
        std::list<std::shared_ptr<pml::nmos::Flow> > m_lstAdded;
        std::list<std::shared_ptr<pml::nmos::Flow> > m_lstUpdated;
        std::list<std::shared_ptr<pml::nmos::Flow> > m_lstRemoved;
};

class wxNmosClientSenderEvent : public wxCommandEvent
{

    public:

        wxNmosClientSenderEvent(const typename std::list<std::shared_ptr<pml::nmos::SenderBase> >& lstAdded, const typename std::list<std::shared_ptr<pml::nmos::SenderBase> >& lstUpdated, const typename std::list<std::shared_ptr<pml::nmos::SenderBase> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_SENDER), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientSenderEvent(const wxNmosClientSenderEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<pml::nmos::SenderBase> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<pml::nmos::SenderBase> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<pml::nmos::SenderBase> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientSenderEvent() : wxCommandEvent(){}
        ~wxNmosClientSenderEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientSenderEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientSenderEvent)

    private:
        std::list<std::shared_ptr<pml::nmos::SenderBase> > m_lstAdded;
        std::list<std::shared_ptr<pml::nmos::SenderBase> > m_lstUpdated;
        std::list<std::shared_ptr<pml::nmos::SenderBase> > m_lstRemoved;
};


class wxNmosClientReceiverEvent : public wxCommandEvent
{

    public:

        wxNmosClientReceiverEvent(const typename std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& lstAdded, const typename std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& lstUpdated, const typename std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_RECEIVER), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientReceiverEvent(const wxNmosClientReceiverEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<pml::nmos::ReceiverBase> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientReceiverEvent() : wxCommandEvent(){}
        ~wxNmosClientReceiverEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientReceiverEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientReceiverEvent)

    private:
        std::list<std::shared_ptr<pml::nmos::ReceiverBase> > m_lstAdded;
        std::list<std::shared_ptr<pml::nmos::ReceiverBase> > m_lstUpdated;
        std::list<std::shared_ptr<pml::nmos::ReceiverBase> > m_lstRemoved;
};




#endif // __NMOS__
