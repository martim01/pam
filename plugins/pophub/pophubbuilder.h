#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
class pophubPanel;

class WXEXPORT pophubBuilder : public MonitorPluginBuilder
{
    public:
        enum enumView { kRadio=0, kRadioLoudness, kNews, kNewsLoudness, kNewsGain, kWorkshop, kTV};

        pophubBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("pophub");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
		
    	friend class pophubPanel;
    	friend class pnlOptions;
    	friend class pnlMeters;
    	friend class pnlMode;
    	friend class pnlPhase;
    	friend class pnlView;
        friend class pnlR128;
        
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
       virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();

        pophubPanel* m_pMeter;

};


