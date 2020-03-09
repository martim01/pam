#pragma once
#include "monitorpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
	class ptpPanel;

class WXEXPORT ptpBuilder : public MonitorPluginBuilder
{
    public:
        ptpBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("PTP Monkey");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:

	friend class ptpPanel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateMonitorPanel(wxWindow* pParent);
        virtual std::list<pairOptionPanel_t>CreateOptionPanels(wxWindow* pParent);

        void LoadSettings();

	ptpPanel* m_pMeter;

};



