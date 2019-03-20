#ifndef PNLLOGCONTROL_H
#define PNLLOGCONTROL_H

//(*Headers(pnlLogControl)
#include "wmbutton.h"
#include <wx/panel.h>
//*)
#include "wmlist.h"

class pnlLog;

class pnlLogControl: public wxPanel
{
	public:

		pnlLogControl(wxWindow* parent, pnlLog* pLogWindow, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlLogControl();

		//(*Declarations(pnlLogControl)
		wmButton* m_pbtnScroll;
		wmButton* m_pbtnPageUp;
		wmButton* m_pbtnHome;
		wmButton* m_pbtnEnd;
		wmButton* m_ptbnClear;
		wmButton* m_ptbnPageDown;
		//*)
        wmList* m_plstFilter;
	protected:

	    friend class pnlLog;

	    void UpdateControl(size_t nCurrentPage, size_t nTotalPages);

		//(*Identifiers(pnlLogControl)
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN6;
		//*)

	private:

		//(*Handlers(pnlLogControl)
		void OnbtnHomeClick(wxCommandEvent& event);
		void OnbtnPageUpClick(wxCommandEvent& event);
		void OnbtnEndClick(wxCommandEvent& event);
		void OntbnPageDownClick(wxCommandEvent& event);
		void OnbtnScrollClick(wxCommandEvent& event);
		void OntbnClearClick(wxCommandEvent& event);
		//*)

		void OnlstFilterSelected(wxCommandEvent& event);

		pnlLog* m_ppnlLog;
		DECLARE_EVENT_TABLE()
};

#endif
