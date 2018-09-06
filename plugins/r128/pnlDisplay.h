#ifndef PNLDISPLAY_H
#define PNLDISPLAY_H

//(*Headers(pnlDisplay)
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class R128Builder;
class pnlDisplay: public wxPanel
{
	public:

		pnlDisplay(wxWindow* parent,R128Builder* pBuilder,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlDisplay();

		//(*Declarations(pnlDisplay)
		wmList* m_plstShow;
		wmLabel* m_pLbl1;
		//*)

	protected:

		//(*Identifiers(pnlDisplay)
		static const long ID_M_PLBL1;
		static const long ID_M_PLST1;
		//*)

	private:

		//(*Handlers(pnlDisplay)
		void OnlstShowSelected(wxCommandEvent& event);
		//*)

		R128Builder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
