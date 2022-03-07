#ifndef PNLOPTIONS_H
#define PNLOPTIONS_H

//(*Headers(pnlOptions)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class AngleMetersBuilder;

class pnlOptions: public wxPanel
{
	public:

		pnlOptions(wxWindow* parent,AngleMetersBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlOptions();

		//(*Declarations(pnlOptions)
		wmLabel* m_pLbl1;
		wmList* m_plstMeter_Peaks;
		wmButton* m_pbnFreeze;
		wmButton* m_pbtnSurround;
		wmList* m_plstText;
		wmButton* m_pbtnMeterClear;
		//*)

	protected:

		//(*Identifiers(pnlOptions)
		static const long ID_M_PBTN11;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN12;
		static const long ID_M_PLST25;
		static const long ID_M_PLST1;
		static const long ID_M_PLBL1;
		//*)

	private:

		//(*Handlers(pnlOptions)
		void OnlstTextSelected(wxCommandEvent& event);
		//*)

		AngleMetersBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
