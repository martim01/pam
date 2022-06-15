#ifndef PNLMAIN_H
#define PNLMAIN_H

//(*Headers(pnlMain)
#include "fftdiffmeter.h"
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
//*)

#include "delayline.h"
class fftdiffBuilder;
class SettingEvent;

class pnlMain: public wxPanel
{
	public:

		pnlMain(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlMain();

		//(*Declarations(pnlMain)
		fftdiffMeter* m_pMeter;
		wmButton* m_pbtnBins;
		wmButton* m_pbtnChannelA;
		wmButton* m_pbtnChannelB;
		wmButton* m_pbtnCursor;
		wmButton* m_pbtnDelay;
		wmButton* m_pbtnDelayCalculate;
		wmButton* m_pbtnDelayReset;
		wmButton* m_pbtnOverlap;
		wmButton* m_pbtnRange;
		wmButton* m_pbtnReset;
		wmButton* m_pbtnWindow;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl3;
		wmLabel* m_plblDelay;
		wmLabel* m_plblLevel;
		//*)

		void SetSampleRate(unsigned long nSampleRate);
        void SetChannels(const std::vector<subsession::channelGrouping>& vChannels);
        void ResetMax();
        void ResetMin();
        void ResetAverage();
        void CalculateDelay();
        void SetAudioData(const timedbuffer* pBuffer);


	protected:

		//(*Identifiers(pnlMain)
		static const long ID_METER;
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL3;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN6;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PLBL1;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN11;
		static const long ID_M_PBTN10;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL2;
		static const long ID_M_PBTN9;
		static const long ID_M_PBTN8;
		static const long ID_M_PBTN7;
		//*)

	private:

		//(*Handlers(pnlMain)
		void OnbtnResetClick(wxCommandEvent& event);
		//*)

		void OnSettingChange(SettingEvent& event);

		fftdiffBuilder* m_pBuilder;
		int m_nOffset;
        int m_nSelectedChannels[2];
        std::vector<subsession::channelGrouping> m_vChannels;

		DelayLine m_delayLine;
        long m_nDelayMode;
        unsigned long m_nSampleRate;

        double m_dAverageA = 0.0;
        double m_dAverageB = 0.0;
        double m_dTotalFrames = 0.0;

        enum {DELAY_OFF=0, DELAY_ONE, DELAY_AUTO};

		DECLARE_EVENT_TABLE()
};

#endif
