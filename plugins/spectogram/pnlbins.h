#ifndef PNLBINS_H
#define PNLBINS_H

//(*Headers(pnlBins)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class SpectogramBuilder;

class pnlBins: public wxPanel
{
	public:

		pnlBins(wxWindow* parent, SpectogramBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlBins();

		//(*Declarations(pnlBins)
		wmList* m_plstFFT_Bins;
		//*)

	protected:

		//(*Identifiers(pnlBins)
		static const long ID_M_PLST11;
		//*)

	private:

		//(*Handlers(pnlBins)
		void OnlstFFT_BinsSelected(wxCommandEvent& event);
		//*)

        SpectogramBuilder* m_pBuilder;
		DECLARE_EVENT_TABLE()
};

#endif
