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
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl9;
		wmList* m_plstClient;
		wmList* m_plstDiscovery;
		wmList* m_plstNode;
		wmList* m_plstQuery;
		wmList* m_plstRegistration;
		wxPanel* Panel1;
		//*)


	protected:

		//(*Identifiers(pnlSettingsNmos)
		static const long ID_M_PLBL8;
		static const long ID_M_PLST1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLST2;
		static const long ID_M_PLBL2;
		static const long ID_M_PLST3;
		static const long ID_M_PLBL3;
		static const long ID_M_PLST4;
		static const long ID_M_PLBL4;
		static const long ID_M_PLST5;
		static const long ID_PANEL1;
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
