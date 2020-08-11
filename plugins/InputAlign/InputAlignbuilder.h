#pragma once
#include "testpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
	class InputAlignPanel;

class WXEXPORT InputAlignBuilder : public TestPluginBuilder
{
    public:
        InputAlignBuilder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("InputAlign");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:

	friend class InputAlignPanel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateTestPanel(wxWindow* pParent);

        void LoadSettings();

	InputAlignPanel* m_pMeter;

};

