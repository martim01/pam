#pragma once
#include "generatorpluginbuilder.h"
#include <wx/string.h>
#include "ltc.h"
#include <queue>

class SettingEvent;
	class LTCGeneratorPanel;

class WXEXPORT LTCGeneratorBuilder : public GeneratorPluginBuilder
{
    public:
        LTCGeneratorBuilder();

        virtual void GetAudioData(timedbuffer* pBuffer);

        wxString GetName() const
        {
            return wxT("LTC");
        }

        virtual void Init();
        virtual void Stop();

    protected:

	friend class LTCGeneratorPanel;

    void OnSettingChanged(SettingEvent& event);

    virtual wxWindow* CreateGeneratorPanel(wxWindow* pParent);

    void CreateFrame();
    void LoadSettings();
    void CreateBBCTime();
    void CreateTVETime();
    void CreateMTDTime();

	LTCGeneratorPanel* m_pMeter;
	LTCEncoder * m_pEncoder;


	void SetFrameTime();

	wxDateTime m_dtFrame;
	double m_dFPS;
	unsigned int m_nFrame;
    int m_nDateType;
    enum {SMPTE=0, BBC=1, TVE=2, MTD=3};
	enum {LIVE=0, OFFSET=1, ABS=2};
	std::queue<float> m_queueSamples;
};



