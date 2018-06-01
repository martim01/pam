#pragma once

#include "inimanager.h"
#include "inisection.h"
#include <map>
#include <wx/event.h>

class PAMBASE_IMPEXPORT Settings
{
    public:
        static Settings& Get();

        wxString Read(const wxString& sSection, const wxString& sKey, const wxString& sDefault);
        int Read(const wxString& sSection, const wxString& sKey, int nDefault);
        double Read(const wxString& sSection, const wxString& sKey, double dDefault);

        bool Write(const wxString& sSection, const wxString& sKey, const wxString& sValue);
        bool Write(const wxString& sSection, const wxString& sKey, int nValue);
        bool Write(const wxString& sSection, const wxString& sKey, double dValue);

        bool RemoveKey(const wxString& sSection, const wxString& sKey);
        bool RemoveSection(const wxString& sSection);


        bool GetSectionDataBegin(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itBegin) const;
        bool GetSectionDataEnd(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itEnd) const;

        void AddHandler(const wxString& sSection, const wxString& sKey, wxEvtHandler* pHandler);
        void RemoveHandler(wxEvtHandler* pHandler);

        wxString GetExecutableDirectory() const;
        wxString GetConfigDirectory() const;
        wxString GetDocumentDirectory() const;
        wxString GetMonitorPluginDirectory() const;
        wxString GetCoreLibDirectory() const;
        wxString GetTestPluginDirectory() const;
        wxString GetLogDirectory() const;
        wxString GetTempDirectory() const;

        wxString GetLibraryVersion() const;

        bool HideCursor();

    protected:
        Settings();
        void CreatePaths();
        iniManager m_iniManager;

        std::multimap<wxString, wxEvtHandler*> m_mmHandlers;
};


