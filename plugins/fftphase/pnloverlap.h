#ifndef PNLOVERLAP_H
#define PNLOVERLAP_H

//(*Headers(pnlOverlap)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class fftphaseBuilder;
class pnlOverlap: public wxPanel
{
	public:

		pnlOverlap(wxWindow* parent,fftphaseBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOverlap();

		//(*Declarations(pnlOverlap)
		wmList* m_plstFFT_Overlap;
		//*)

	protected:

		//(*Identifiers(pnlOverlap)
		static const long ID_M_PLST10;
		//*)

	private:

		//(*Handlers(pnlOverlap)
		void OnlstFFT_OverlapSelected(wxCommandEvent& event);
		//*)

        fftphaseBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
