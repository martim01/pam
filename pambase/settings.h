#pragma once

#include "inimanager.h"
#include "inisection.h"
#include <map>
#include <wx/timer.h>
#include <wx/event.h>
#include <set>
#include "settingevent.h"

class PAMBASE_IMPEXPORT Settings : public wxEvtHandler
{
    public:
        static Settings& Get();

        void ReadSettings(const wxString& sFullPath);

        wxString Read(const wxString& sSection, const wxString& sKey, const wxString& sDefault);
        int Read(const wxString& sSection, const wxString& sKey, int nDefault);
        double Read(const wxString& sSection, const wxString& sKey, double dDefault);

        bool Write(const wxString& sSection, const wxString& sKey, const wxString& sValue, wxEvtHandler* pWriter = nullptr);
        bool Write(const wxString& sSection, const wxString& sKey, int nValue, wxEvtHandler* pWriter = nullptr);
        bool Write(const wxString& sSection, const wxString& sKey, unsigned int nValue, wxEvtHandler* pWriter = nullptr);
        bool Write(const wxString& sSection, const wxString& sKey, double dValue, wxEvtHandler* pWriter = nullptr);

        bool RemoveKey(const wxString& sSection, const wxString& sKey);
        bool RemoveSection(const wxString& sSection);


        void ResetFile();

        bool GetSectionDataBegin(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itBegin);
        bool GetSectionDataEnd(const wxString& sSection, std::map<wxString, wxString>::const_iterator& itEnd);

        void AddHandler( wxEvtHandler* pHandler, const wxString& sSection=wxString(""), const wxString& sKey=wxString(""));
        void RemoveHandler(wxEvtHandler* pHandler);

        wxString GetExecutableDirectory() const;
        wxString GetConfigDirectory() const;
        wxString GetDocumentDirectory() const;
        wxString GetMonitorPluginDirectory() const;
        wxString GetCoreLibDirectory() const;
        wxString GetTestPluginDirectory() const;
        wxString GetGeneratorPluginDirectory() const;
        wxString GetLogDirectory() const;
        wxString GetWavDirectory() const;
        wxString GetTempDirectory() const;

        wxString GetLibraryVersion() const;

        const std::multimap<wxString, wxString>& GetInterfaces();

        const mapSection& GetSections();
        std::shared_ptr<iniSection> GetSection(const wxString& sSection);

    protected:
        Settings();
        void CreatePaths();

        bool WriteInternal(const wxString& sSection, const wxString& sKey, const wxString& sValue, SettingEvent::enumType eType, wxEvtHandler* pWriter);

        void OnTimerSave(wxTimerEvent& event);

        void GetHandlers(const wxString& sHandlers, std::set<wxEvtHandler*>& setHandlers);

        void OnSettingChanged(SettingEvent& event);
        iniManager m_iniManager;
        wxString m_sFullPath;

        std::multimap<wxString, wxEvtHandler*> m_mmHandlers;

        std::multimap<wxString, wxString> m_mmInterfaces;

        wxTimer m_timerSave;
};


extern std::string GetChannelMapping();
