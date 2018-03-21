#ifndef PNLSETTINGS_H
#define PNLSETTINGS_H

//(*Headers(pnlSettings)
#include "wmbutton.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class AoIPInfoBuilder;

class pnlSettings: public wxPanel
{
	public:

		pnlSettings(wxWindow* parent, AoIPInfoBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlSettings();

		//(*Declarations(pnlSettings)
		wmList* m_plstInterval;
		wmButton* m_pbtnClear;
		//*)

	protected:

		//(*Identifiers(pnlSettings)
		static const long ID_M_PLST16;
		static const long ID_M_PBTN29;
		//*)

	private:

		//(*Handlers(pnlSettings)
		void OnlstIntervalSelected(wxCommandEvent& event);
		void OnbtnClearClick(wxCommandEvent& event);
		//*)

		AoIPInfoBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
