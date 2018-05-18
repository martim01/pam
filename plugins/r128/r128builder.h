#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class R128Meter;
class SettingEvent;

class WXEXPORT R128Builder : public MonitorPluginBuilder
{
    public:
        R128Builder();
        ~R128Builder();
        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("R128");
        }

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        bool CanBeMaximized() const
        {
            return false;
        }

    protected:

        friend class R128Meter;

        void ClearMeter();
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
    private:
        R128Meter* m_pRadar;

        unsigned int m_nInputChannels;
        unsigned int m_nDisplayChannel;
        unsigned int m_nMode;
};



