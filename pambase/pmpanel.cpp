#include "pmpanel.h"
#ifndef WXSPAM
#include "settingevent.h"
#include "settings.h"
wxIMPLEMENT_DYNAMIC_CLASS(pmPanel, wxPanel);
#else
IMPLEMENT_DYNAMIC_CLASS(pmPanel, wxPanel)
#endif // WXSPAM

BEGIN_EVENT_TABLE(pmPanel, wxPanel)
END_EVENT_TABLE()


pmPanel::pmPanel()
{
    }


