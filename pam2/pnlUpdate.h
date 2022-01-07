#ifndef PNLUPDATE_H
#define PNLUPDATE_H

//(*Headers(pnlUpdate)
#include "wmlabel.h"
#include <wx/panel.h>
//*)
#include <wx/dir.h>
#include <map>
#include <wx/timer.h>
#include "pnlUSB.h"
class pnlUpdate: public wxPanel
{
	public:

		pnlUpdate(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, unsigned long n=0, const wxString& s=wxEmptyString);
		virtual ~pnlUpdate();

		//(*Declarations(pnlUpdate)
		pnlUSB* m_pnlUSB;
		wmLabel* m_pLbl3;
		//*)

	protected:

		//(*Identifiers(pnlUpdate)
		static const long ID_M_PLBL3;
		static const long ID_PANEL4;
		//*)

	private:

		//(*Handlers(pnlUpdate)
		void OnlstTypeSelected(wxCommandEvent& event);
		void OnlstFoldersSelected(wxCommandEvent& event);
		void OnbtnUpClick(wxCommandEvent& event);
		void OnbtnSelectClick(wxCommandEvent& event);
		void OnbtnCheckClick(wxCommandEvent& event);
		void OnbtnSetHttpClick(wxCommandEvent& event);
		void OnbtnShareSetClick(wxCommandEvent& event);
		void OnbtnDetectClick(wxCommandEvent& event);
		void OnlstUsbSelected(wxCommandEvent& event);
		//*)
        void OnShown(wxShowEvent& event);


		wxString m_sLocation;

		DECLARE_EVENT_TABLE()
};

#endif
