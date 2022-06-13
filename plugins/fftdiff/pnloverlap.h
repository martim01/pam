#ifndef PNLOVERLAP_H
#define PNLOVERLAP_H

//(*Headers(pnlOverlap)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class fftdiffBuilder;
class pnlOverlap: public wxPanel
{
	public:

		pnlOverlap(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOverlap();

		wmList* m_plstFFT_Overlap;

	protected:

		static const long ID_M_PLST10;

	private:


        fftdiffBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
