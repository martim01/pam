#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SpectogramMeter;
class pnlRoutiing;


class WXEXPORT SpectogramBuilder : public MonitorPluginBuilder
{
    public:
        SpectogramBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("Spectogram");
        }


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

        void LoadSettings(){}

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);



    private:
        SpectogramMeter* m_pMeter;
        //pnlRoutiing* m_ppnlRouting;
};
