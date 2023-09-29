#pragma once
#include <wx/event.h>

#include "saphandler.h"


class wxSapHandler : public pml::sap::Handler
{
    public:
        wxSapHandler(wxEvtHandler* pHandler);
        void SapMessage(const pml::sap::sap& aMessage);
    private:
        wxEvtHandler* m_pHandler;

};

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_SAP, wxCommandEvent);
