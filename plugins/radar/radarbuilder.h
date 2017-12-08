#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class RadarMeter;
class SettingEvent;
class LevelCalculator;
class pnlRouting;

class WXEXPORT RadarBuilder : public MonitorPluginBuilder
{
    public:
        RadarBuilder();
        ~RadarBuilder();
        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("Radar");
        }

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        bool CanBeMaximized() const
        {
            return false;
        }

    protected:

        friend class RadarMeter;
        friend class pnlRouting;
        friend class pnlDisplay;
        friend class pnlMeters;

        void ClearMeter();
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
    private:
        LevelCalculator* m_pCalculator;
        RadarMeter* m_pRadar;
        pnlRouting* m_ppnlRouting;
        unsigned int m_nInputChannels;
        unsigned int m_nDisplayChannel;
        unsigned int m_nMode;
};



