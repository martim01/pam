#pragma once
#include "testpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
	class lineupPanel;

class WXEXPORT lineupBuilder : public TestPluginBuilder
{
    public:
        lineupBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("lineup");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
		
	friend class lineupPanel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateTestPanel(wxWindow* pParent);

        void LoadSettings();

	lineupPanel* m_pMeter;

};



