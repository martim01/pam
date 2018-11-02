#pragma once
#include <wx/event.h>

extern "C" void WXEXPORT CreateGeneratorBuilder();
extern "C" wxString WXEXPORT GetGeneratorPluginName();
extern "C" wxString WXEXPORT GetPluginVersion();
extern "C" wxString WXEXPORT GetDetails();

