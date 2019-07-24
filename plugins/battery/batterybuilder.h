#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
	class batteryPanel;

class WXEXPORT batteryBuilder : public MonitorPluginBuilder
{
    public:
        batteryBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("battery");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
		
	friend class batteryPanel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();

	batteryPanel* m_pMeter;

};



