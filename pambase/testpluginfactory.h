#pragma once
#include "dlldefine.h"
#include <wx/string.h>
#include <map>
#include <wx/event.h>
#include <list>
#include "plugin.h"

class wxDynamicLibrary;
class TestPluginBuilder;
class wmSwitcherPanel;

class PAMBASE_IMPEXPORT TestPluginFactory
{
public:
    static TestPluginFactory* Get();
    bool LoadTestLibrary(const wxString& sLibrary);

    void SetHandler(wxEvtHandler* pHandler);
    void SetSwitcherPanels(wmSwitcherPanel* pswpTests);

    void Register(TestPluginBuilder* pBuilder);
    plugin GetPluginDetails(const wxString& sDir, const wxString& sLibrary);


    void UnloadAll();

    std::map<wxString, TestPluginBuilder*>::iterator GetPluginBegin();
    std::map<wxString, TestPluginBuilder*>::iterator GetPluginEnd();
    std::map<wxString, TestPluginBuilder*>::iterator FindPlugin(const wxString& sName);

private:
    /** Constructor.**/
    TestPluginFactory() : m_pHandler(0), m_pswpTests(0){}

    /** Destructor.**/
    ~TestPluginFactory();
    wxEvtHandler* m_pHandler;
    wmSwitcherPanel* m_pswpTests;
    std::map<wxString, TestPluginBuilder*> m_mBuilders;
    std::map<wxString, wxDynamicLibrary*> m_mLibraries;
};

