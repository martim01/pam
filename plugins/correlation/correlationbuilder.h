#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
class correlationPanel;

class WXEXPORT correlationBuilder : public MonitorPluginBuilder
{
    public:
        correlationBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("correlation");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
		
    	friend class correlationPanel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();

        correlationPanel* m_pMeter;

};


