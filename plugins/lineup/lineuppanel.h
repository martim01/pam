#pragma once

//(*Headers(lineupPanel)
#include "wmlabel.h"
#include <wx/panel.h>
//*)
#include <list>
#include <vector>
#include "pmpanel.h"
#include "offsetcalculator.h"
#include "kiss_fft.h"

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
		wmLabel* m_plblDominantdBR;
		wmLabel* m_pLbl1;
		wmLabel* m_plblLevelL;
		wmLabel* m_plblPhaseSamples;
		wmLabel* m_plblPhaseDegrees;
		wmLabel* m_plblLevelR;
		wmLabel* m_plblDominantdBL;
		wmLabel* m_pLbl2;
		wmLabel* m_plblDominantHzL;
		wmLabel* m_plblDominantHzR;
		wmLabel* m_plblInput;
		//*)

		void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);
		void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers(lineupPanel)
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL60;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(lineupPanel)
		//*)

		void DoFFT(bool bRight);
		void OnOffsetDone(wxCommandEvent& event);
		lineupBuilder* m_pBuilder;

        std::vector<float> m_vBufferL;
		std::vector<float> m_vBufferR;
        OffsetCalculator* m_pCalc;

        std::list<float> m_lstBuffer;      ///< vector containing the a-leg samples
        std::vector<kiss_fft_cpx> m_vfft_out;

        unsigned int m_nChannels;
        unsigned int m_nChannel[2];
        unsigned int m_nSampleRate;
        unsigned int m_nSampleSize;

        double m_dOffsetSamples;
        double m_dDominantFrequency[2];
        double m_dDominantLevel[2];

        LevelCalculator* m_pLevelCalc;
        double m_dLevel[2];
		DECLARE_EVENT_TABLE()
};

