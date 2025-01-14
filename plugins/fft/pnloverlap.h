#ifndef PNLOVERLAP_H
#define PNLOVERLAP_H

//(*Headers(pnlOverlap)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class FFTBuilder;
class pnlOverlap: public wxPanel
{
	public:

		pnlOverlap(wxWindow* parent,FFTBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOverlap();

		wmList* m_plstFFT_Overlap;

	protected:

		static const long ID_M_PLST10;

	private:


        FFTBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
