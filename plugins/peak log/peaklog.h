#pragma once
#include <wx/event.h>

extern "C" void WXEXPORT CreateTestBuilder();
extern "C" wxString WXEXPORT GetTestPluginName();


extern "C" wxString WXEXPORT GetPluginVersion();
extern "C" wxString WXEXPORT GetDetails();
