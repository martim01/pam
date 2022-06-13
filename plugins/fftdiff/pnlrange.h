#ifndef PNLRANGE_H
#define PNLRANGE_H

//(*Headers(pnlRange)
#include "wmbutton.h"
#include <wx/panel.h>
//*)
#include <wmlist.h>

class fftdiffBuilder;
class SettingEvent;
class pnlRange: public wxPanel
{
	public:

		pnlRange(wxWindow* parent, fftdiffBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlRange();

		wmList* m_plstRange;

	protected:


	private:


		fftdiffBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
