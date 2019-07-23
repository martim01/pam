#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class FftMeter;
class pnlRoutiing;


class WXEXPORT FFTBuilder : public MonitorPluginBuilder
{
    public:
        FFTBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("FFT");
        }


        void OnCursorMode(bool bOn);

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
        friend class pnlBins;
        friend class pnlDisplay;
        friend class pnlOptions;
        friend class pnlOverlap;
        friend class pnlRoutiing;
        friend class pnlType;
        friend class pnlWindow;
        friend class FftMeter;


        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();
        void OnBinsChanged(wxCommandEvent& event);
        void OnDisplayChanged(wxCommandEvent& event);
        void OnWindowChanged(wxCommandEvent& event);
        void OnOverlapChanged(wxCommandEvent& event);
        void OnRoutingChanged(wxCommandEvent& event);
        void OnTypeChanged(wxCommandEvent& event);
        void OnHoldClicked(wxCommandEvent& event);
        void ColourSelected(bool bSelected);

    private:
        FftMeter* m_pMeter;
        pnlRoutiing* m_ppnlRouting;
};
