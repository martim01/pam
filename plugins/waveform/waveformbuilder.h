#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
	class waveformMeter;

class WXEXPORT waveformBuilder : public MonitorPluginBuilder
{
    public:
        waveformBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("waveform");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:

	friend class waveformMeter;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();

	waveformMeter* m_pMeter;

};



