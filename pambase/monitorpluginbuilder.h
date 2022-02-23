#pragma once
#include "dlldefine.h"
#include <wx/event.h>
#include <map>
#include <list>
#include <vector>
#include "session.h"
#include <set>
#include "RestGoose.h"


class wmSwitcherPanel;
class timedbuffer;

typedef std::pair<wxString, wxWindow*> pairOptionPanel_t;


class PAMBASE_IMPEXPORT MonitorPluginBuilder : public wxEvtHandler
{
    public:
        MonitorPluginBuilder();
        virtual ~MonitorPluginBuilder();

        void Maximize(bool bShow);

        virtual void InputSession(const session& aSession)=0;
        virtual void OutputChannels(const std::vector<char>& vChannels)=0;
        virtual void SetAudioData(const timedbuffer* pBuffer)=0;
        virtual wxString GetName() const=0;
        virtual void SetQoSData(qosData* pData){};
        virtual bool CanBeMaximized() const;

        void DeletePanels();

        bool WantsAudioAlways()
        {
            return m_bWantsAudioAlways;
        }

        bool WantsQoSAlways()
        {
            return m_bWantsQoSAlways;
        }

        wxWindow* GetMainWindow() const { return m_pWindow;}

        wxString GetSection() { return "Monitor::"+GetName();}


    protected:

        friend class MonitorPluginFactory;


        void SetHandler(wxEvtHandler* pHandler);
        void CreatePanels(wmSwitcherPanel* pswpMonitor, wmSwitcherPanel* pswpOptions);

        void WriteSetting(const wxString& sSetting, const wxString& sValue);
        void WriteSetting(const wxString& sSetting, int nValue);
        void WriteSetting(const wxString& sSetting, double dValue);

        wxString ReadSetting(const wxString& sSetting, const wxString& sDefault);
        int ReadSetting(const wxString& sSetting, int nDefault);
        double ReadSetting(const wxString& sSetting, double dDefault);

        void RegisterForSettingsUpdates(wxEvtHandler* pHandler, const wxString& sSetting="");

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent)=0;
        virtual std::list<pairOptionPanel_t> CreateOptionPanels(wxWindow* pParent)=0;
        virtual void LoadSettings()=0;

        void AskToMonitor(const std::vector<char>& vChannels);

        void InitRemoteApi();
        pml::restgoose::response GetStatus(const query& theQuery, const std::vector<pml::restgoose::partData>& vData, const endpoint& theEndpoint, const userName& theUser);

        void SendWebsocketMessage(const Json::Value& jsMessage);
        bool WebsocketsActive();

        void RegisterRemoteApiEnum(const wxString& sKey, const std::set<wxString>& setEnum);
        void RegisterRemoteApiEnum(const wxString& sKey, const std::map<int, wxString>& mEnum);
        void RegisterRemoteApiRangeDouble(const wxString& sKey, const std::pair<double, double>& dRange);
        void RegisterRemoteApiRangeInt(const wxString& sKey, const std::pair<int, int>& nRange);
        void RegisterRemoteApiCallback(const wxString& sKey, std::function<std::set<wxString>()> func);
        void RegisterRemoteApiCallback(const wxString& sKey, std::function<std::map<int, wxString>()> func);
        void RegisterRemoteApi(const wxString& sKey);
        void RegisterRemoteApiCSV(const wxString& sKey, const std::set<wxString>& setEnum);


        wxEvtHandler* m_pHandler;

        bool m_bWantsAudioAlways;
        bool m_bWantsQoSAlways;

    private:
        wmSwitcherPanel* m_pswpMonitor;
        wmSwitcherPanel* m_pswpOptions;
        wxWindow* m_pWindow;
        std::list<pairOptionPanel_t> m_lstOptions;

        std::set<wxEvtHandler*> m_setHandlers;
};

DECLARE_EXPORTED_EVENT_TYPE(PAMBASE_IMPEXPORT, wxEVT_MONITOR_MAX, -1)
