#pragma once
#include "testpluginbuilder.h"
#include <wx/string.h>

class SettingEvent;
class pnlMain;

class WXEXPORT fftdiffBuilder : public TestPluginBuilder
{
    public:
        fftdiffBuilder();

        void SetAudioData(const timedbuffer* pBuffer) override;

        wxString GetName() const override
        {
            return wxT("C.A.R.");
        }

        void ResetMax();
        void ResetMin();
        void ResetAverage();
        void CalculateDelay();
        void InputSession(const session& aSession) override;
        void OutputChannels(const std::vector<char>& vChannels) override;
        bool CanBeMaximized() const override { return true; }

    protected:

    	friend class fftdiffMeter;
    	friend class pnlMain;
        wxWindow* CreateTestPanel(wxWindow* pParent) override;

        void LoadSettings() override;

        pnlMain* m_ppnlMain;

};


