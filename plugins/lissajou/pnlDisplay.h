#ifndef PNLDISPLAY_H
#define PNLDISPLAY_H

//(*Headers(pnlDisplay)
#include "wmbutton.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class LissajouBuilder;
class pnlDisplay: public wxPanel
{
	public:

		pnlDisplay(wxWindow* parent,LissajouBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlDisplay();

		//(*Declarations(pnlDisplay)
		wmButton* m_pbtnLevels;
		wmList* m_plstScaling;
		wmButton* m_pbtnDisplay;
		wmButton* m_pbtnRotate;
		//*)

	protected:

		//(*Identifiers(pnlDisplay)
		static const long ID_M_PLST24;
		static const long ID_M_PBTN36;
		static const long ID_M_PBTN37;
		static const long ID_M_PBTN1;
		//*)

	private:

		//(*Handlers(pnlDisplay)
		void OnlstScalingSelected(wxCommandEvent& event);
		void OnbtnRotateClick(wxCommandEvent& event);
		void OnbtnLevelsClick(wxCommandEvent& event);
		void OnbtnDisplayClick(wxCommandEvent& event);
		//*)
		LissajouBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
