#ifndef PNLSETTINGSNMOS_H
#define PNLSETTINGSNMOS_H


//(*Headers(pnlSettingsNmos)
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)
#include "wmlist.h"
#include <memory>

class SettingEvent;
namespace pml
{
    namespace nmos
    {
        class Sender;
    };
};

class wxNmosNodeRegistrationEvent;
class wxNmosClientQueryEvent;


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
		wmLabel* m_pLbl5;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl9;
		wmLabel* m_plblDiscoveryNode;
		wmLabel* m_plblQueryNode;
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
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL4;
		static const long ID_M_PLST5;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL9;
		static const long ID_PANEL1;
		//*)

	private:

		//(*Handlers(pnlSettingsNmos)
		void OnbtnNmosClick(wxCommandEvent& event);
		void OnbtnClientClick(wxCommandEvent& event);
		void OnlstNodeSelected(wxCommandEvent& event);
		void OnlstClientSelected(wxCommandEvent& event);
		//*)

		void OnNmosRegistrationNodeFound(const wxNmosNodeRegistrationEvent& event);
		void OnNmosRegistrationNodeRemoved(const wxNmosNodeRegistrationEvent& event);
		void OnNmosRegistrationNodeChanged(const wxNmosNodeRegistrationEvent& event);
		void OnNmosRegistrationModeChanged(const wxNmosNodeRegistrationEvent& event);

		void OnNmosQueryNodeFound(const wxNmosClientQueryEvent& event);
		void OnNmosQueryNodeRemoved(const wxNmosClientQueryEvent& event);
		void OnNmosQueryNodeChanged(const wxNmosClientQueryEvent& event);

		void OnSettingChanged(SettingEvent& event);

		static const wxColour CLR_NODE_OK;
		static const wxColour CLR_NODE_BAD;
		static const wxColour CLR_NODE_SELECTED;

        DECLARE_EVENT_TABLE()
};

#endif
