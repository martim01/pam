#pragma once

#include <wx/event.h>
#include "dlldefine.h"

class PAMBASE_IMPEXPORT SapWatchThread : public wxThread
{
    public:
        SapWatchThread(wxEvtHandler* pParent, const wxString& sMulticast);
        void* Entry();

    private:
        wxEvtHandler* m_pParent;
        wxString m_sMulticast;

        unsigned char m_packet[65536];
};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_SAP, -1)
