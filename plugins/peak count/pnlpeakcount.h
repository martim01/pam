#ifndef PNLPEAKCOUNT_H
#define PNLPEAKCOUNT_H

//(*Headers(pnlPeakCount)
#include "wmbutton.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmedit.h"
//*)

#include "timedbuffer.h"
#include "session.h"

class pnlPeakCountChannel;

class pnlPeakCount: public wxPanel
{
	public:

		pnlPeakCount(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlPeakCount();

				void SetAudioData(const timedbuffer* pBuffer);
		void ResetTest();

		void InputSession(const session& aSession);


		//(*Declarations(pnlPeakCount)
		wmLabel* m_pLbl4;
		wxPanel* pnlLeft;
		wmLabel* m_pLbl3;
		wmKeyboard* m_pKbd1;
		wmLabel* m_pLbl1;
		wmLabel* m_plblLimit;
		wmLabel* m_pLbl12;
		wmLabel* m_pLbl5;
		wmLabel* m_pLbl6;
		wmEdit* m_pedtLimit;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnReset;
		//*)

	protected:

		//(*Identifiers(pnlPeakCount)
		static const long ID_M_PLBL37;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL4;
		static const long ID_M_PKBD1;
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL7;
		static const long ID_M_PEDT1;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(pnlPeakCount)
		void OnbtnResetClick(wxCommandEvent& event);
		void OnedtLimitTextEnter(wxCommandEvent& event);
		//*)

		double m_dLimit;
		std::vector<pnlPeakCountChannel*> m_vChannels;

		DECLARE_EVENT_TABLE()
};

#endif
