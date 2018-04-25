#pragma once
#include "dlldefine.h"
#include <wx/string.h>
#include <map>
#include <wx/event.h>
#include <list>
#include "plugin.h"


class wxDynamicLibrary;
class MonitorPluginBuilder;
class wmSwitcherPanel;

class PAMBASE_IMPEXPORT MonitorPluginFactory
{
public:
    static MonitorPluginFactory* Get();


    bool LoadLibrary(const wxString& sLibrary);

    void SetHandler(wxEvtHandler* pHandler);
    void SetSwitcherPanels(wmSwitcherPanel* pswpMonitor, wmSwitcherPanel* pswpOptions);

    void Register(MonitorPluginBuilder* pBuilder);

    wxString GetPluginDirectory();

    void UnloadAll(bool bDeletePanel = false);

    std::map<wxString, MonitorPluginBuilder*>::iterator GetPluginBegin();
    std::map<wxString, MonitorPluginBuilder*>::iterator GetPluginEnd();
    std::map<wxString, MonitorPluginBuilder*>::iterator FindPlugin(const wxString& sName);

    std::map<wxString, wxDynamicLibrary*>::const_iterator GetLibraryBegin() const;
    std::map<wxString, wxDynamicLibrary*>::const_iterator GetLibraryEnd() const;
    std::map<wxString, wxDynamicLibrary*>::const_iterator FindLibray(const wxString& sLibrary) const;

    plugin GetPluginDetails(const wxString& sDir, const wxString& sLibrary);

private:
    /** Constructor.**/
    MonitorPluginFactory() : m_pHandler(0), m_pswpMonitor(0), m_pswpOptions(0){}

    /** Destructor.**/
    ~MonitorPluginFactory();
    wxEvtHandler* m_pHandler;
    wmSwitcherPanel* m_pswpMonitor;
    wmSwitcherPanel* m_pswpOptions;
    std::map<wxString, MonitorPluginBuilder*> m_mBuilders;
    std::map<wxString, wxDynamicLibrary*> m_mLibraries;
};

