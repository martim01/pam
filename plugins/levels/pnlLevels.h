#ifndef PNLLEVELS_H
#define PNLLEVELS_H

#include <vector>
//(*Headers(pnlLevels)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmslider.h"
#include "wmlist.h"
//*)
#include "json/json.h"

class MaxMinGraph;

class timedbuffer;
struct session;
class LevelsBuilder;
class SettingEvent;


class pnlLevels: public wxPanel
{
	public:

		pnlLevels(wxWindow* parent,LevelsBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlLevels();

		wmList* m_plstMontor;
		wmButton* m_pbtndB;
		wmLabel* m_pLbl12;
		wmSlider* m_pAmplitude;
		wmButton* m_pbtnReset;

		wmButton* m_pbtnRangedB;
        wmSlider* m_pRange;
        void SetAudioData(const timedbuffer* pBuffer);
		void ResetTest();

		void InputSession(const session& aSession);

	protected:

		static const long ID_M_PLBL37;
		static const long ID_M_PBTN1;
		static const long ID_M_PLST1;
		static const long ID_CUSTOM1;
		static const long ID_M_PLBL9;

	private:

		void OnbtnResetClick(wxCommandEvent& event);
		//void OnlstMontorSelected(wxCommandEvent& event);
        void OnbtndBClick(wxCommandEvent& event);
        void OnbtnRangedBClick(wxCommandEvent& event);

		void CreateGraphs(unsigned int nChannels);

		void OnSliderMove(wxCommandEvent& event);
		void OnRangeSliderMove(wxCommandEvent& event);

		void OnSettingChanged(SettingEvent& event);

		std::vector<MaxMinGraph*> m_vGraph;
		std::vector<wmLabel*> m_vLabel;
		std::vector<double> m_vLevelPeakMax;
		std::vector<double> m_vLevelPeakMin;

        Json::Value m_jsSetting;
        LevelsBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
