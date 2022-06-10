#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
class fftdiffMeter;

class WXEXPORT fftdiffBuilder : public MonitorPluginBuilder
{
    public:
        fftdiffBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("fftdiff");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:

    	friend class fftdiffMeter;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();

        fftdiffMeter* m_pMeter;

};


