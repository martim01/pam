#ifndef PNLABOUT_H
#define PNLABOUT_H

//(*Headers(pnlAbout)
#include <wx/panel.h>
//*)
#include "wmlistadv.h"

class pnlAbout: public wxPanel
{
	public:

		pnlAbout(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId="");
		virtual ~pnlAbout();

		//(*Declarations(pnlAbout)
		//*)

	protected:

		//(*Identifiers(pnlAbout)
		//*)

		wmListAdv* m_plstAbout;

	private:

		//(*Handlers(pnlAbout)
		//*)

		DECLARE_EVENT_TABLE()
};



#endif
