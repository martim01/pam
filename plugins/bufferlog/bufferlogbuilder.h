#pragma once
#include "testpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
	class bufferlogPanel;

class WXEXPORT bufferlogBuilder : public TestPluginBuilder
{
    public:
        bufferlogBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("bufferlog");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
		
	friend class bufferlogPanel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateTestPanel(wxWindow* pParent);

        void LoadSettings();

	bufferlogPanel* m_pMeter;

};



