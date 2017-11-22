#pragma once
#include "testpluginbuilder.h"

class pnlChannelDelay;

class ChannelDelayBuilder : public TestPluginBuilder
{
    public:
        ChannelDelayBuilder();
        virtual ~ChannelDelayBuilder();

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        virtual void SetAudioData(const timedbuffer* pBuffer);
        virtual wxString GetName() const;

    protected:

        friend class pnlChannelDelay;
        virtual wxWindow* CreateTestPanel(wxWindow* pParent);
        virtual void LoadSettings();


        pnlChannelDelay* m_ppnlDelay;

};



