#ifndef PNLUSB_H
#define PNLUSB_H
#include <wx/timer.h>

//(*Headers(pnlUSB)
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)

class pnlUSB: public wxPanel
{
	public:

		pnlUSB(wxWindow* parent, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, int nStyle=0,  const wxString& sN=wxEmptyString);
		virtual ~pnlUSB();

		void SetSection(const wxString& sSection);

		//(*Declarations(pnlUSB)
		wmButton* m_pbtnDetect;
		wmLabel* m_pLbl7;
		wmLabel* m_plblUSB;
		wmList* m_plstUsb;
		//*)

		void StartCheck();
		void StopCheck();

	protected:

		//(*Identifiers(pnlUSB)
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL12;
		static const long ID_M_PBTN6;
		static const long ID_M_PLST3;
		//*)

	private:

		//(*Handlers(pnlUSB)
		void OnbtnDetectClick(wxCommandEvent& event);
		void OnlstUsbSelected(wxCommandEvent& event);
		//*)



		void OnTimerUSB(wxTimerEvent& event);
		void CheckUSB();

		wxTimer m_timerUSB;

		std::map<wxString, wxString> m_mUsb;

        wxString m_sSection;

		DECLARE_EVENT_TABLE()
};

#endif
