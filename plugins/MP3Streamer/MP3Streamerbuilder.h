#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
	class MP3StreamerPanel;

class WXEXPORT MP3StreamerBuilder : public MonitorPluginBuilder
{
    public:
        MP3StreamerBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("MP3Streamer");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
		
	friend class MP3StreamerPanel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();

	MP3StreamerPanel* m_pMeter;

};



