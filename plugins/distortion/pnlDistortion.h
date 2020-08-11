#ifndef PNLDISTORTION_H
#define PNLDISTORTION_H
#include <list>
#include <vector>

//(*Headers(pnlDistortion)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include <wx/timer.h>
//*)

class timedbuffer;
class pnlDistortionChannel;
struct session;
class DistortionBuilder;

class pnlDistortion: public wxPanel
{
	public:

		pnlDistortion(wxWindow* parent,DistortionBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlDistortion();

		void SetAudioData(const timedbuffer* pBuffer);
		void ResetTest();

		void InputSession(const session& aSession);

		//(*Declarations(pnlDistortion)
		wmButton* m_pbtnReset;
		wmLabel* m_pLbl12;
		wmLabel* m_pLbl15;
		wmLabel* m_pLbl17;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl4;
		wxPanel* pnlLeft;
		wxTimer m_timerTest;
		//*)

	protected:

		//(*Identifiers(pnlDistortion)
		static const long ID_M_PLBL37;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL38;
		static const long ID_M_PLBL40;
		static const long ID_M_PBTN1;
		static const long ID_PANEL1;
		static const long ID_TIMER1;
		//*)

	private:

		//(*Handlers(pnlDistortion)
		void OnbtnReset_LeftClick(wxCommandEvent& event);
		void OnbtnReset_RightClick(wxCommandEvent& event);
		void OntimerTestTrigger(wxTimerEvent& event);
		void OnbtnResetClick(wxCommandEvent& event);
		//*)

        void RunTest();

        DistortionBuilder* m_pBuilder;

		std::vector<pnlDistortionChannel*> m_vChannels;
        //double m_dMax[2];
        //double m_dLevelPeakMax[2];
        //double m_dLevelPeakMin[2];
		DECLARE_EVENT_TABLE()
};

#endif
