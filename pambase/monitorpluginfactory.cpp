#include "monitorpluginfactory.h"
#include "monitorpluginbuilder.h"
#include "wmswitcherpanel.h"
#include <wx/dynlib.h>
#include <wx/log.h>
#include <wx/xml/xml.h>


using namespace std;

MonitorPluginFactory* MonitorPluginFactory::Get()
{
    static MonitorPluginFactory wmf;
    return &wmf;
}

void MonitorPluginFactory::SetHandler(wxEvtHandler* pHandler)
{
    m_pHandler = pHandler;
}

void MonitorPluginFactory::Register(MonitorPluginBuilder* pBuilder)
{
    pBuilder->SetHandler(m_pHandler);
    pBuilder->CreatePanels(m_pswpMonitor, m_pswpOptions);

    m_mBuilders.insert(make_pair(pBuilder->GetName(), pBuilder));
}

MonitorPluginFactory::~MonitorPluginFactory()
{
    UnloadAll();
}

void MonitorPluginFactory::SetSwitcherPanels(wmSwitcherPanel* pswpMonitor, wmSwitcherPanel* pswpOptions)
{
    m_pswpMonitor = pswpMonitor;
    m_pswpOptions = pswpOptions;
}


bool MonitorPluginFactory::LoadLibrary(const wxString& sLibrary)
{
    wxLogNull ln;

    map<wxString, wxDynamicLibrary*>::iterator itLib = m_mLibraries.find(sLibrary);
    if(itLib == m_mLibraries.end())
    {
        wxString sCan = wxDynamicLibrary::CanonicalizeName(sLibrary);

        wxString sLib;
        #ifdef PAMBASE_DEBUG
            sLib = wxString::Format(wxT("lib/debug/%s"), sCan.c_str());
        #else
            sLib = wxT("lib/") + sCan;
        #endif
        wxDynamicLibrary* pLib = new wxDynamicLibrary(sLib);

        if(pLib && pLib->IsLoaded())
        {
            if(pLib->HasSymbol(wxT("CreateBuilders")))
            {
                typedef void (*RegPtr)();
                RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("CreateBuilders"));
                if(ptr)
                {
                    (*ptr)();
                    m_mLibraries.insert(make_pair(sLibrary, pLib));
                    return true;
                }
            }
        }
        delete pLib;
    }
    else
    {
        typedef void (*RegPtr)();

        RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("CreateBuilders"));
        if(ptr)
        {
            (*ptr)();
            return true;
        }
    }
    return false;
}


void MonitorPluginFactory::UnloadAll()
{
    for(map<wxString, MonitorPluginBuilder*>::iterator itBuilder = m_mBuilders.begin(); itBuilder != m_mBuilders.end(); ++itBuilder)
    {
        delete itBuilder->second;
    }
    m_mBuilders.clear();

    for(map<wxString, wxDynamicLibrary*>::iterator itBuilder = m_mLibraries.begin(); itBuilder != m_mLibraries.end(); ++itBuilder)
    {
        delete itBuilder->second;
    }
    m_mLibraries.clear();

}


map<wxString, MonitorPluginBuilder*>::iterator MonitorPluginFactory::MonitorPluginFactory::GetPluginBegin()
{
    return m_mBuilders.begin();
}
map<wxString, MonitorPluginBuilder*>::iterator MonitorPluginFactory::MonitorPluginFactory::GetPluginEnd()
{
    return m_mBuilders.end();
}

map<wxString, MonitorPluginBuilder*>::iterator MonitorPluginFactory::FindPlugin(const wxString& sName)
{
    return m_mBuilders.find(sName);
}


map<wxString, wxDynamicLibrary*>::const_iterator MonitorPluginFactory::GetLibraryBegin() const
{
    return m_mLibraries.begin();
}

map<wxString, wxDynamicLibrary*>::const_iterator MonitorPluginFactory::GetLibraryEnd() const
{
    return m_mLibraries.end();
}

map<wxString, wxDynamicLibrary*>::const_iterator MonitorPluginFactory::FindLibray(const wxString& sLibrary) const
{
    return m_mLibraries.find(sLibrary);
}


wxString MonitorPluginFactory::GetPluginName(const wxString& sDir, const wxString& sLibrary)
{
    wxLogNull ln;
    map<wxString, wxDynamicLibrary*>::const_iterator itLib = m_mLibraries.find(sLibrary);
    if(itLib != m_mLibraries.end())
    {
        if(itLib->second->HasSymbol(wxT("GetPluginName")))
        {
            typedef wxString (*RegPtr)();
            RegPtr ptr = (RegPtr)itLib->second->GetSymbol(wxT("GetPluginName"));
            return (*ptr)();
        }
    }
    else
    {
        wxString sLib = sDir + wxT("/") + wxDynamicLibrary::CanonicalizeName(sLibrary);

        wxDynamicLibrary* pLib = new wxDynamicLibrary(sLib);
        if(pLib)
        {
            if(pLib->IsLoaded())
            {
                if(pLib->HasSymbol(wxT("GetPluginName")))
                {
                    typedef wxString (*RegPtr)();
                    RegPtr ptr = (RegPtr)pLib->GetSymbol(wxT("GetPluginName"));
                    if(ptr)
                    {
                        return (*ptr)();
                    }
                }
            }
            delete pLib;
        }
    }
    return wxEmptyString;
}
