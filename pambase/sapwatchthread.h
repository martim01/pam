#pragma once

#include <wx/event.h>
#include "dlldefine.h"

class PAMBASE_IMPEXPORT SapWatchThread : public wxThread
{
    public:
        SapWatchThread(wxEvtHandler* pParent);
        void* Entry();

    private:
        wxEvtHandler* m_pParent;


        unsigned char m_packet[65536];
};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_SAP, -1)
