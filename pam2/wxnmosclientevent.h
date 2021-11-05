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

        wxNmosClientNodeEvent(const typename std::list<std::shared_ptr<Self> >& lstAdded, const typename std::list<std::shared_ptr<Self> >& lstUpdated, const typename std::list<std::shared_ptr<Self> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_NODE), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientNodeEvent(const wxNmosClientNodeEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<Self> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<Self> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<Self> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientNodeEvent() : wxCommandEvent(){}
        ~wxNmosClientNodeEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientNodeEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientNodeEvent)

    private:
        std::list<std::shared_ptr<Self> > m_lstAdded;
        std::list<std::shared_ptr<Self> > m_lstUpdated;
        std::list<std::shared_ptr<Self> > m_lstRemoved;
};


class wxNmosClientDeviceEvent : public wxCommandEvent
{

    public:

        wxNmosClientDeviceEvent(const typename std::list<std::shared_ptr<Device> >& lstAdded, const typename std::list<std::shared_ptr<Device> >& lstUpdated, const typename std::list<std::shared_ptr<Device> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_DEVICE), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientDeviceEvent(const wxNmosClientDeviceEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<Device> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<Device> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<Device> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientDeviceEvent() : wxCommandEvent(){}
        ~wxNmosClientDeviceEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientDeviceEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientDeviceEvent)

    private:
        std::list<std::shared_ptr<Device> > m_lstAdded;
        std::list<std::shared_ptr<Device> > m_lstUpdated;
        std::list<std::shared_ptr<Device> > m_lstRemoved;
};


class wxNmosClientSourceEvent : public wxCommandEvent
{

    public:

        wxNmosClientSourceEvent(const typename std::list<std::shared_ptr<Source> >& lstAdded, const typename std::list<std::shared_ptr<Source> >& lstUpdated, const typename std::list<std::shared_ptr<Source> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_SOURCE), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientSourceEvent(const wxNmosClientSourceEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<Source> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<Source> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<Source> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientSourceEvent() : wxCommandEvent(){}
        ~wxNmosClientSourceEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientSourceEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientSourceEvent)

    private:
        std::list<std::shared_ptr<Source> > m_lstAdded;
        std::list<std::shared_ptr<Source> > m_lstUpdated;
        std::list<std::shared_ptr<Source> > m_lstRemoved;
};


class wxNmosClientFlowEvent : public wxCommandEvent
{

    public:

        wxNmosClientFlowEvent(const typename std::list<std::shared_ptr<Flow> >& lstAdded, const typename std::list<std::shared_ptr<Flow> >& lstUpdated, const typename std::list<std::shared_ptr<Flow> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_FLOW), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientFlowEvent(const wxNmosClientFlowEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<Flow> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<Flow> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<Flow> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientFlowEvent() : wxCommandEvent(){}
        ~wxNmosClientFlowEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientFlowEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientFlowEvent)

    private:
        std::list<std::shared_ptr<Flow> > m_lstAdded;
        std::list<std::shared_ptr<Flow> > m_lstUpdated;
        std::list<std::shared_ptr<Flow> > m_lstRemoved;
};

class wxNmosClientSenderEvent : public wxCommandEvent
{

    public:

        wxNmosClientSenderEvent(const typename std::list<std::shared_ptr<Sender> >& lstAdded, const typename std::list<std::shared_ptr<Sender> >& lstUpdated, const typename std::list<std::shared_ptr<Sender> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_SENDER), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientSenderEvent(const wxNmosClientSenderEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<Sender> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<Sender> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<Sender> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientSenderEvent() : wxCommandEvent(){}
        ~wxNmosClientSenderEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientSenderEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientSenderEvent)

    private:
        std::list<std::shared_ptr<Sender> > m_lstAdded;
        std::list<std::shared_ptr<Sender> > m_lstUpdated;
        std::list<std::shared_ptr<Sender> > m_lstRemoved;
};


class wxNmosClientReceiverEvent : public wxCommandEvent
{

    public:

        wxNmosClientReceiverEvent(const typename std::list<std::shared_ptr<Receiver> >& lstAdded, const typename std::list<std::shared_ptr<Receiver> >& lstUpdated, const typename std::list<std::shared_ptr<Receiver> >& lstRemoved) : wxCommandEvent(wxEVT_NMOS_CLIENT_RECEIVER), m_lstAdded(lstAdded), m_lstUpdated(lstUpdated), m_lstRemoved(lstRemoved){}

        wxNmosClientReceiverEvent(const wxNmosClientReceiverEvent& event) : wxCommandEvent(event), m_lstAdded(event.GetAdded()), m_lstUpdated(event.GetUpdated()), m_lstRemoved(event.GetRemoved()){}

        const std::list<std::shared_ptr<Receiver> >& GetAdded() const
        {   return m_lstAdded;  }
        const std::list<std::shared_ptr<Receiver> >& GetUpdated() const
        {   return m_lstUpdated;  }
        const std::list<std::shared_ptr<Receiver> >& GetRemoved() const
        {   return m_lstRemoved;  }

        wxNmosClientReceiverEvent() : wxCommandEvent(){}
        ~wxNmosClientReceiverEvent(){}

        virtual wxEvent *Clone() const { return new wxNmosClientReceiverEvent(*this); }


        DECLARE_DYNAMIC_CLASS(wxNmosClientReceiverEvent)

    private:
        std::list<std::shared_ptr<Receiver> > m_lstAdded;
        std::list<std::shared_ptr<Receiver> > m_lstUpdated;
        std::list<std::shared_ptr<Receiver> > m_lstRemoved;
};




#endif // __NMOS__
