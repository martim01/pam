#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class pnlLissajou;
class SettingEvent;
class pnlRouting;

class WXEXPORT LissajouBuilder : public MonitorPluginBuilder
{
    public:
        LissajouBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("Lissajou");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:

        friend class pnlLissajou;
        friend class pnlDisplay;
        friend class pnlMeters;
        friend class pnlRouting;


        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
    private:
        pnlLissajou* m_pLissajou;
        pnlRouting* m_ppnlRouting;
};



