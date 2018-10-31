#pragma once
#include "testpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
	class LTCPanel;

class WXEXPORT LTCBuilder : public TestPluginBuilder
{
    public:
        LTCBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("LTC");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
		
	friend class LTCPanel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateTestPanel(wxWindow* pParent);

        void LoadSettings();

	LTCPanel* m_pMeter;

};



