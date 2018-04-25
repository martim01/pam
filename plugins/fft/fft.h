#pragma once
#include <wx/event.h>

extern "C" void WXEXPORT CreateMonitorBuilder();
extern "C" wxString WXEXPORT GetPluginName();
extern "C" wxString WXEXPORT GetPluginVersion();
extern "C" wxString WXEXPORT GetDetails();

