#include "pmcontrol.h"
#ifndef WXSPAM
#include "settingevent.h"
#include "settings.h"
#endif // WXSPAM
#include <wx/log.h>


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

