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
            return wxT("AoIP Info");
        }

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);
        virtual void SetQoSData(qosData* pData);

        virtual bool CanBeMaximized() const;

    protected:

        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
    private:
        pnlAoIPInfo* m_pInfo;
};


