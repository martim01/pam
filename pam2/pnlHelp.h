#ifndef PNLHELP_H
#define PNLHELP_H

//(*Headers(pnlHelp)
#include "wmbutton.h"
#include <wx/panel.h>
#include <wx/html/htmlwin.h>
#include "wmlist.h"
//*)
#include "wxtouchscreenhtml.h"

class pnlHelp: public wxPanel
{
	public:

		pnlHelp(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlHelp();

        void SelectHelp(const wxString& sHelp);
		//(*Declarations(pnlHelp)
		wxPanel* Panel1;
		wxTouchScreenHtml* m_pHtml;
		wmList* m_plstHelp;
		wmButton* m_pbtnClose;
		//*)

	protected:

		//(*Identifiers(pnlHelp)
		static const long ID_M_PLST1;
		static const long ID_M_PBTN1;
		static const long ID_PANEL1;
		static const long ID_HTMLWINDOW1;
		//*)

	private:

		//(*Handlers(pnlHelp)
		void OnlstHelpSelected(wxCommandEvent& event);
		void OnbtnCloseClick(wxCommandEvent& event);
		//*)
		wxString m_sHelp;

		DECLARE_EVENT_TABLE()
};

DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_HELP_CLOSE,-1)
#endif
