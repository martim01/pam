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

class pnlEbuMeter: public wxPanel
{
	public:

		pnlEbuMeter(wxWindow* parent,R128Builder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlEbuMeter();

		wxPanel* pnlInfo;
		wmLabel* m_pLbl23;
		wmLabel* m_pLbl25;
		wmLabel* m_pLbl24;
		wmLabel* m_plblSessionChannels;
		wmLabel* m_plblSessionBits;
		wmLabel* m_plblSessionFrequency;
		wmLabel* m_pLbl20;
		wmLabel* m_pLbl27;
		wmLabel* m_pLbl26;
		wmLabel* m_plblSessionType;
		wmLabel* m_plblSessionSource;
		wmLabel* m_pLbl33;
		wmLabel* m_plblSessionName;
		wmLabel* m_plblInput;

        wmLabel* m_plblShortTitle;
		wmLabel* m_plblShort;
		wmLabel* m_plblMomentaryTitle;
		wmLabel* m_plblMomentary;

		wmLabel* m_plblLufsTitle;
		wmLabel* m_plblLufs;

		wmLabel* m_plblRange;
		wmLabel* m_plblRangeTitle;

		void SetSession(const session& aSession);

		void SetAudioData(const timedbuffer* pBuffer);

		void SetMode(unsigned int nMode);
        void Freeze(bool bFreeze);
        void ShowPeaks(unsigned int nMode);
        void ClearMeters();
        void SetShading(bool bShaded);
        void OutputChannels(const std::vector<char>& vChannels);
        void UpdateMeters();


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

	private:

		void OnLeftUp(wxMouseEvent& event);
		void OnInfoLeftUp(wxMouseEvent& event);


		void CreateMeters();

        R128Builder* m_pBuilder;
        R128Meter* m_pLevels;
		std::array<R128Meter*, 3> m_aMeters;

        R128Calculator* m_pR128;
        double m_dOffset;
        static const wxColour CLR_LUFS;
		DECLARE_EVENT_TABLE()
};

#endif

