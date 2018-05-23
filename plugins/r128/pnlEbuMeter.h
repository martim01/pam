#ifndef PNLMETERS_H
#define PNLMETERS_H

#include <vector>
#include <array>

class session;
class LevelMeter;
class timedbuffer;

#include "wmlabel.h"
#include <wx/panel.h>

class MaxMinGraph;
class R128Meter;
class wmButton;
class R128Builder;
class R128Calculator;
class TruePeakCalculator;

class pnlEbuMeter: public wxPanel
{
	public:

		pnlEbuMeter(wxWindow* parent,R128Builder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlEbuMeter();

        wmLabel* m_plblShortTitle;
		wmLabel* m_plblShort;
		wmLabel* m_plblMomentaryTitle;
		wmLabel* m_plblMomentary;

		wmLabel* m_plblShortMax;
		wmLabel* m_plblMomentaryMax;

		wmLabel* m_plblLufsTitle;
		wmLabel* m_plblLufs;

		wmLabel* m_plblRange;
		wmLabel* m_plblRangeTitle;

		wmLabel* m_plblPeakLeftTitle;
		wmLabel* m_plblPeakLeft;
		wmLabel* m_plblPeakLeftMax;
		wmLabel* m_plblPeakRightTitle;
		wmLabel* m_plblPeakRight;
		wmLabel* m_plblPeakRightMax;

		void SetSession(const session& aSession);

		void SetAudioData(const timedbuffer* pBuffer);

		void SetMode(unsigned int nMode);
        void Freeze(bool bFreeze);
        void ShowPeaks(unsigned int nMode);
        void ClearMeters();
        void SetShading(bool bShaded);
        void OutputChannels(const std::vector<char>& vChannels);
        void UpdateMeters();

        void InitLabel(wmLabel* pLabel, const wxColour clrBack, int nFontSize=12);


        void ChangeScale();

	protected:

		static const long ID_M_PLBL59;
		static const long ID_M_PLBL38;
		static const long ID_M_PLBL40;
		static const long ID_M_PLBL39;
		static const long ID_M_PLBL60;
		static const long ID_M_PLBL41;
		static const long ID_M_PLBL43;
		static const long ID_M_PLBL42;
		static const long ID_M_PLBL44;
		static const long ID_M_PLBL47;
		static const long ID_M_PLBL45;
		static const long ID_M_PLBL48;
		static const long ID_M_PLBL46;
		static const long ID_M_PLBL49;
        static const long ID_PANEL1;
        static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;

        wmButton* m_pbtnCalculate;
		wmButton* m_pbtnReset;


	private:

		void OnbtnCalculateClick(wxCommandEvent& event);
		void OnbtnResetClick(wxCommandEvent& event);

		void OnLeftUp(wxMouseEvent& event);
		void OnInfoLeftUp(wxMouseEvent& event);

        void SetPeakColour(wmLabel* pLabel, double dValue);
		void CreateMeters();

        R128Builder* m_pBuilder;
        R128Meter* m_pLevels;
		std::array<R128Meter*, 3> m_aMeters;

        R128Meter* m_pPeakLevels;
        R128Meter* m_pPeakLeft;
        R128Meter* m_pPeakRight;

        double m_dPeak[2];
        R128Calculator* m_pR128;
        TruePeakCalculator* m_pTrue;
        double m_dOffset;
        static const wxColour CLR_LUFS;
        static const wxColour CLR_SHORT;
        static const wxColour CLR_PEAK;
        static const wxColour CLR_PEAK_ALARM;
		DECLARE_EVENT_TABLE()
};

#endif

