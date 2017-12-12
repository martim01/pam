#ifndef PNLDISPLAY_H
#define PNLDISPLAY_H

//(*Headers(pnlDisplay)
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class RadarBuilder;
class pnlDisplay: public wxPanel
{
	public:

		pnlDisplay(wxWindow* parent,RadarBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlDisplay();

		//(*Declarations(pnlDisplay)
		wmLabel* m_pLbl1;
		wmList* m_plstPoints;
		wmLabel* m_pLbl2;
		wmList* m_plstTimeframe;
		//*)

	protected:

		//(*Identifiers(pnlDisplay)
		static const long ID_M_PLST24;
		static const long ID_M_PLBL2;
		static const long ID_M_PLST1;
		static const long ID_M_PLBL1;
		//*)

	private:

		//(*Handlers(pnlDisplay)
		void OnlstTimeframeSelected(wxCommandEvent& event);
		void OnlstPointsSelected(wxCommandEvent& event);
		//*)
		RadarBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
