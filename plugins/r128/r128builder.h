#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class pnlEbuMeter;
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

        friend class pnlEbuMeter;

        void ClearMeter();
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
    private:
        pnlEbuMeter* m_pMeters;

        unsigned int m_nInputChannels;
        unsigned int m_nDisplayChannel;
        unsigned int m_nMode;
};



