#ifndef PNLWINDOW_H
#define PNLWINDOW_H

//(*Headers(pnlWindow)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class SpectogramBuilder;

class pnlWindow: public wxPanel
{
	public:

		pnlWindow(wxWindow* parent,SpectogramBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlWindow();

		//(*Declarations(pnlWindow)
		wmList* m_plstFFT_Window;
		//*)

	protected:

		//(*Identifiers(pnlWindow)
		static const long ID_M_PLST5;
		//*)

	private:


        SpectogramBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
