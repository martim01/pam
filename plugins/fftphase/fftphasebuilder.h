#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
class fftphaseMeter;
class pnlRoutiing;

class WXEXPORT fftphaseBuilder : public MonitorPluginBuilder
{
    public:
        fftphaseBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("fftphase");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:

        friend class fftphaseMeter;
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
        void OnTypeChanged(wxCommandEvent& event);

        void FallSelected(bool bSelected);

    private:
//        pnlRoutiing* m_ppnlRouting;

        void OnSettingChanged(SettingEvent& event);



        fftphaseMeter* m_pMeter;

};



