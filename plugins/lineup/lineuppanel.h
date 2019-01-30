#pragma once

//(*Headers(lineupPanel)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
//*)
#include <list>
#include <vector>
#include "pmpanel.h"
#include "offsetcalculator.h"
#include "kiss_fft.h"
#include "glitsdetector.h"

class lineupBuilder;
class timedbuffer;
class session;
class LevelCalculator;


class lineupPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(lineupPanel);

		lineupPanel(){}
		lineupPanel(wxWindow* parent,lineupBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~lineupPanel();

		//(*Declarations(lineupPanel)
		wmLabel* m_pLbl4;
		wmLabel* m_plblDominantdBR;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl7;
		wmLabel* m_plblDistortionR;
		wmLabel* m_plblRangeL;
		wmLabel* m_pLbl1;
		wmLabel* m_plblLevelL;
		wmLabel* m_plblDistortionMaxL;
		wmLabel* m_plblRangeR;
		wmLabel* m_pLbl10;
		wmLabel* m_plblPhaseSamples;
		wmLabel* m_plblPhaseDegrees;
		wmLabel* m_pLbl5;
		wmLabel* m_plblLevelR;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl8;
		wmLabel* m_plblDominantdBL;
		wmLabel* m_pLbl2;
		wmLabel* m_plblDistortionMaxR;
		wmLabel* m_pLbl9;
		wmLabel* m_plblDominantHzL;
		wmLabel* m_plblDistortionL;
		wmButton* m_pbtnReset;
		wmLabel* m_plblDominantHzR;
		wmLabel* m_plblInput;
		//*)

		void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);
		void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers(lineupPanel)
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL16;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL6;
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL17;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL60;
		static const long ID_M_PLBL21;
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL14;
		static const long ID_M_PLBL18;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL11;
		static const long ID_M_PLBL19;
		static const long ID_M_PLBL12;
		static const long ID_M_PLBL23;
		static const long ID_M_PLBL22;
		static const long ID_M_PLBL15;
		static const long ID_M_PLBL24;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL20;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(lineupPanel)
		void OnbtnResetClick(wxCommandEvent& event);
		//*)

		void DoFFT();
		void OnOffsetDone(wxCommandEvent& event);
		lineupBuilder* m_pBuilder;

        std::vector<float> m_vBufferL;
		std::vector<float> m_vBufferR;
        OffsetCalculator* m_pCalc;

        std::list<float> m_lstBufferL;      ///< vector containing the a-leg samples
        std::list<float> m_lstBufferR;      ///< vector containing the a-leg samples
        std::vector<kiss_fft_cpx> m_vfft_out;

        unsigned int m_nChannels;
        unsigned int m_nChannel[2];
        unsigned int m_nSampleRate;
        unsigned int m_nSampleSize;

        double m_dOffsetSamples;
        double m_dDominantFrequency[2];
        double m_dDominantLevel[2];

        LevelCalculator* m_pLevelCalc;
        GlitsDetector m_glitsDetector;

        double m_dLevelMin[2];
        double m_dLevelMax[2];
        double m_dLevel[2];
        double m_dDistortionMax[2];
        bool m_bFirstLevel;
        bool m_bFirstDistortion;
		DECLARE_EVENT_TABLE()
};

