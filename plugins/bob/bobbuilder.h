#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
class bobPanel;

class WXEXPORT bobBuilder : public MonitorPluginBuilder
{
    public:
        bobBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("bob");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
		
    	friend class bobPanel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
       virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();

        bobPanel* m_pMeter;

};


