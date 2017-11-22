#pragma once
#include "testpluginbuilder.h"

class pnlIdentify;

class IdentifyBuilder : public TestPluginBuilder
{
    public:
        IdentifyBuilder();
        virtual ~IdentifyBuilder();

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        virtual void SetAudioData(const timedbuffer* pBuffer);
        virtual wxString GetName() const;

    protected:

        friend class pnlIdentify;
        virtual wxWindow* CreateTestPanel(wxWindow* pParent);
        virtual void LoadSettings();


        pnlIdentify* m_ppnlIdentify;

};






