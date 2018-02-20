#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class pnlMeters;
class pnlScale;
class SettingEvent;

class WXEXPORT MetersBuilder : public MonitorPluginBuilder
{
    public:
        MetersBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("Meters");
        }

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:

        friend class pnlMeters;
        friend class pnlMode;
        friend class pnlOptions;
        friend class pnlMeterSettings;
        friend class pnlScale;

        void ClearMeter();
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
    private:
        pnlMeters* m_pMeters;
};


