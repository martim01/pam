#pragma once
#include "testpluginbuilder.h"

class pnlDistortion;

class DistortionBuilder : public TestPluginBuilder
{
    public:
        DistortionBuilder();
        virtual ~DistortionBuilder();

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        virtual void SetAudioData(const timedbuffer* pBuffer);
        virtual wxString GetName() const;

    protected:

        friend class pnlDistortion;
        friend class pnlDistortionChannel;

        virtual wxWindow* CreateTestPanel(wxWindow* pParent);
        virtual void LoadSettings();


        pnlDistortion* m_ppnlDistortion;

};




