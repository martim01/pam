#pragma once
#include "testpluginbuilder.h"

class pnlPeakLog;
class SettingEvent;

class PeakLogBuilder : public TestPluginBuilder
{
    public:
        PeakLogBuilder();
        virtual ~PeakLogBuilder();

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        virtual void SetAudioData(const timedbuffer* pBuffer);
        virtual wxString GetName() const;

    protected:

        void OnSettingChanged(SettingEvent& event);

        friend class pnlPeakLog;
        virtual wxWindow* CreateTestPanel(wxWindow* pParent);
        virtual void LoadSettings();


        pnlPeakLog* m_ppnlPeakLog;

};






