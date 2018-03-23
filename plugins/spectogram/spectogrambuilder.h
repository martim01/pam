#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SpectogramMeter;
class pnlRoutiing;
class SettingEvent;

class WXEXPORT SpectogramBuilder : public MonitorPluginBuilder
{
    public:
        SpectogramBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("Specto");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
        friend class pnlBins;
        friend class pnlOverlap;
        friend class pnlRoutiing;
        friend class pnlWindow;
        friend class pnlColours;

        void LoadSettings(){}

        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);



    private:
        SpectogramMeter* m_pMeter;
        pnlRoutiing* m_ppnlRouting;
};
