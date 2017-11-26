#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class pnlAngleMeters;
class SettingEvent;

class WXEXPORT AngleMetersBuilder : public MonitorPluginBuilder
{
    public:
        AngleMetersBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("Angle Meters");
        }

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:

        friend class pnlAngleMeters;
        friend class pnlMode;
        friend class pnlOptions;
        friend class pnlMeterSettings;

        void ClearMeter();
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
    private:
        pnlAngleMeters* m_pAngleMeters;
};



