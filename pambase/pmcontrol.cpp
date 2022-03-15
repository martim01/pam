#include "pmcontrol.h"
#ifndef WXSPAM
#include "settingevent.h"
#include "settings.h"
#endif // WXSPAM
#include <wx/log.h>


wxString CH_GROUPING[8] = {L"\u2081", L"\u2082", L"\u2083", L"\u2084", L"\u2085", L"\u2086", L"\u2087", L"\u2088"};

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(pmControl, wxWindow)
#else
wxIMPLEMENT_DYNAMIC_CLASS(pmControl, wxWindow);
#endif // WXSPAM



BEGIN_EVENT_TABLE(pmControl, wxWindow)
END_EVENT_TABLE()


pmControl::pmControl()
{
}

