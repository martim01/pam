#pragma once
#include "testpluginbuilder.h"
class pnlPeakCount;


class PeakCountBuilder : public TestPluginBuilder
{
    public:
        PeakCountBuilder();
        virtual ~PeakCountBuilder();

        virtual void InputSession(const session& aSession);
        virtual void OutputChannels(const std::vector<char>& vChannels);

        virtual void SetAudioData(const timedbuffer* pBuffer);
        virtual wxString GetName() const;

    protected:

        friend class pnlPeakCount;
        virtual wxWindow* CreateTestPanel(wxWindow* pParent);
        virtual void LoadSettings();


        pnlPeakCount* m_ppnlPeakCount;

};





