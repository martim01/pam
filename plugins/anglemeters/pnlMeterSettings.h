#ifndef PNLMETERSETTINGS_H
#define PNLMETERSETTINGS_H

//(*Headers(pnlMeterSettings)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class AngleMetersBuilder;

class pnlMeterSettings: public wxPanel
{
	public:

		pnlMeterSettings(wxWindow* parent,AngleMetersBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlMeterSettings();

		wmLabel* m_pLbl1;
		wmButton* m_pbtnStereo;
		wmList* m_plstSpeed;
		wmButton* m_pbtnM3M6;

	protected:

		static const long ID_M_PBTN1;
		static const long ID_M_PLST2;
		static const long ID_M_PBTN2;
		static const long ID_M_PLBL1;

	private:

		AngleMetersBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
