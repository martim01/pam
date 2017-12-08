#ifndef PNLDISPLAY_H
#define PNLDISPLAY_H

//(*Headers(pnlDisplay)
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
		wmList* m_plstPoints;
		wmList* m_plstTimeframe;
		//*)

	protected:

		//(*Identifiers(pnlDisplay)
		static const long ID_M_PLST24;
		static const long ID_M_PLST1;
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
