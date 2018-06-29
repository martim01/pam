#pragma once
#include "testpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
@meter	class [CLASS_PREFIX]Meter;
@panel	class [CLASS_PREFIX]Panel;

class WXEXPORT [CLASS_PREFIX]Builder : public TestPluginBuilder
{
    public:
        [CLASS_PREFIX]Builder();

        virtual void SetAudioData(const timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("[CLASS_PREFIX]");
        }


        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

    protected:
		
@meter	friend class [CLASS_PREFIX]Meter;
@panel	friend class [CLASS_PREFIX]Panel;
        void OnSettingChanged(SettingEvent& event);

        virtual wxWindow* CreateTestPanel(wxWindow* pParent);

        void LoadSettings();

@meter	[CLASS_PREFIX]Meter* m_pMeter;
@panel	[CLASS_PREFIX]Panel* m_pMeter;

};


