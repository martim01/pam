#ifndef PNLSCALE_H
#define PNLSCALE_H

//(*Headers(pnlScale)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class MetersBuilder;

class pnlScale: public wxPanel
{
	public:

		pnlScale(wxWindow* parent,MetersBuilder* pBuilder,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlScale();

		//(*Declarations(pnlScale)
		wmList* m_plstScale;
		//*)

	protected:

		//(*Identifiers(pnlScale)
		static const long ID_M_PLST1;
		//*)

	private:

		//(*Handlers(pnlScale)
		void OnlstScaleSelected(wxCommandEvent& event);
		//*)

		MetersBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
