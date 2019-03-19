#ifndef PNLTESTOPTIONS_H
#define PNLTESTOPTIONS_H

//(*Headers(pnlTestOptions)
#include "wmbutton.h"
#include <wx/panel.h>
//*)

class pnlTestOptions: public wxPanel
{
	public:

		pnlTestOptions(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlTestOptions();

		//(*Declarations(pnlTestOptions)
		wmButton* m_pbtnLog;
		//*)

		wmButton* m_pbtnLogView;

	protected:

		//(*Identifiers(pnlTestOptions)
		static const long ID_M_PBTN4;
		//*)

	private:

		//(*Handlers(pnlTestOptions)
		void OnbtnLogClick(wxCommandEvent& event);
		//*)
		void OnbtnLogViewClick(wxCommandEvent& event);

		DECLARE_EVENT_TABLE()
};

#endif
