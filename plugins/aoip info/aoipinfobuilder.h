#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class pnlAoIPInfo;
class SettingEvent;

class WXEXPORT AoIPInfoBuilder : public MonitorPluginBuilder
{
    public:
        AoIPInfoBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("Session Info");
        }

        void ChangeGranularity(int nWhich);
        void ChangeResolution(int nWhich);
        void ClearGraphs();
        void RecalculateRange();

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);
        virtual void SetQoSData(qosData* pData);

        virtual bool CanBeMaximized() const;

    protected:

        friend class pnlGraph;
        friend class pnlSettings;
        friend class pnlAoIPInfo;
        friend class pnlQos;

        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();

        Json::Value CreateWebsocketMessage(qosData* pData);
        Json::Value CreateWebsocketMessage(const timedbuffer* pBuffer);
        Json::Value CreateWebsocketMessage(const session& aSession);


    private:
        pnlAoIPInfo* m_pInfo;
};


