#ifndef PNLMETERSETTINGS_H
#define PNLMETERSETTINGS_H

//(*Headers(pnlMeterSettings)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class MetersBuilder;

class pnlMeterSettings: public wxPanel
{
	public:

		pnlMeterSettings(wxWindow* parent,MetersBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlMeterSettings();

		//(*Declarations(pnlMeterSettings)
		wmLabel* m_pLbl1;
		wmList* m_plstSpeed;
		wmButton* m_pbtnM3M6;
		wmButton* m_pbtnShading;
		//*)

	protected:

		//(*Identifiers(pnlMeterSettings)
		static const long ID_M_PBTN1;
		static const long ID_M_PLST2;
		static const long ID_M_PBTN2;
		static const long ID_M_PLBL1;
		//*)

	private:

		//(*Handlers(pnlMeterSettings)
		void OnbtnM3M6Click(wxCommandEvent& event);
		void OnlstSpeedSelected(wxCommandEvent& event);
		void OnbtnShadingClick(wxCommandEvent& event);
		//*)

		MetersBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
