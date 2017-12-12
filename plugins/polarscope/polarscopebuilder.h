#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class PolarScope;
class SettingEvent;

class pnlRouting;

class WXEXPORT PolarScopeBuilder : public MonitorPluginBuilder
{
    public:
        PolarScopeBuilder();
        ~PolarScopeBuilder();
        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("Polar Scope");
        }

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        bool CanBeMaximized() const
        {
            return false;
        }

    protected:

        friend class PolarScope;
        //friend class pnlRouting;
        //friend class pnlDisplay;
        //friend class pnlMeters;

        void ClearMeter();
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
    private:

        PolarScope* m_pPolarScope;
        //pnlRouting* m_ppnlRouting;
        unsigned int m_nInputChannels;
        unsigned int m_nDisplayChannel;
        unsigned int m_nMode;
};




