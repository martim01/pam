#pragma once
#include "generatorpluginbuilder.h"
#include <wx/string.h>
#include "ltc.h"
#include <queue>

class SettingEvent;
class LTCGeneratorPanel;

class WXEXPORT LTCGenerator
{
    public:
        LTCGenerator();

        void GetAudioData(timedbuffer* pBuffer);


        virtual void Init();
        virtual void Stop();

    protected:

	friend class LTCGeneratorPanel;

    void OnSettingChanged(SettingEvent& event);

    virtual wxWindow* CreateGeneratorPanel(wxWindow* pParent);

    void CreateFrame();
    void LoadSettings();

	LTCGeneratorPanel* m_pMeter;
	LTCEncoder * m_pEncoder;

	std::queue<float> m_queueSamples;
};




