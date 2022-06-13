#ifndef PNLWINDOW_H
#define PNLWINDOW_H

//(*Headers(pnlWindow)
#include "wmlist.h"
#include <wx/panel.h>
//*)

class fftdiffBuilder;

class pnlWindow: public wxPanel
{
	public:

		pnlWindow(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlWindow();

		wmList* m_plstFFT_Window;

	protected:

		static const long ID_M_PLST5;

	private:


        fftdiffBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
