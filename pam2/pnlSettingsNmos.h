#ifndef PNLSETTINGSNMOS_H
#define PNLSETTINGSNMOS_H


//(*Headers(pnlSettingsNmos)
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)
#include "wmlist.h"
#include <memory>

namespace pml
{
    namespace nmos
    {
        class Sender;
    };
};

class pnlSettingsNmos: public wxPanel
{
	public:

		pnlSettingsNmos(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId=wxEmptyString);
		virtual ~pnlSettingsNmos();

		//(*Declarations(pnlSettingsNmos)
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl9;
		wmList* m_plstClient;
		wmList* m_plstNode;
		//*)


	protected:

		//(*Identifiers(pnlSettingsNmos)
		static const long ID_M_PLBL8;
		static const long ID_M_PLST1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLST2;
		//*)

	private:

		//(*Handlers(pnlSettingsNmos)
		void OnbtnNmosClick(wxCommandEvent& event);
		void OnbtnClientClick(wxCommandEvent& event);
		void OnlstNodeSelected(wxCommandEvent& event);
		void OnlstClientSelected(wxCommandEvent& event);
		//*)

        DECLARE_EVENT_TABLE()
};

#endif
