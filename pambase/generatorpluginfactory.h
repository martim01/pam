#pragma once
#include "dlldefine.h"
#include <wx/string.h>
#include <map>
#include <wx/event.h>
#include <list>
#include "plugin.h"

class wxDynamicLibrary;
class GeneratorPluginBuilder;
class wmSwitcherPanel;

class PAMBASE_IMPEXPORT GeneratorPluginFactory
{
public:
    static GeneratorPluginFactory* Get();
    bool LoadGeneratorLibrary(const wxString& sLibrary);

    void SetHandler(wxEvtHandler* pHandler);
    void SetSwitcherPanels(wmSwitcherPanel* pswpGenerators);

    void Register(GeneratorPluginBuilder* pBuilder);
    plugin GetPluginDetails(const wxString& sDir, const wxString& sLibrary);

    wxString GetPluginDirectory();
    void UnloadAll();

    std::map<wxString, GeneratorPluginBuilder*>::iterator GetPluginBegin();
    std::map<wxString, GeneratorPluginBuilder*>::iterator GetPluginEnd();
    std::map<wxString, GeneratorPluginBuilder*>::iterator FindPlugin(const wxString& sName);

private:
    /** Constructor.**/
    GeneratorPluginFactory() : m_pHandler(0), m_pswpGenerators(0){}

    /** Destructor.**/
    ~GeneratorPluginFactory();
    wxEvtHandler* m_pHandler;
    wmSwitcherPanel* m_pswpGenerators;
    std::map<wxString, GeneratorPluginBuilder*> m_mBuilders;
    std::map<wxString, wxDynamicLibrary*> m_mLibraries;
};


