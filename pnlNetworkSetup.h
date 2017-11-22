#ifndef PNLNETWORKSETUP_H
#define PNLNETWORKSETUP_H

//(*Headers(pnlNetworkSetup)
#include "wmbutton.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmedit.h"
//*)

class pnlNetworkSetup: public wxPanel
{
	public:

		pnlNetworkSetup(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle = 0, const wxString& sEmpty = wxEmptyString);
		virtual ~pnlNetworkSetup();

		//(*Declarations(pnlNetworkSetup)
		wmLabel* m_pLbl4;
		wmButton* m_pbtnStaticDHCP;
		wmLabel* m_pLbl3;
		wmLabel* m_plblResult;
		wmKeyboard* m_pkbd;
		wmLabel* m_pLbl1;
		wmEdit* m_pedtAddress;
		wmButton* m_pbtnApply;
		wmLabel* m_pLbl2;
		wmEdit* m_pedtSubnet;
		wmEdit* m_pedtGateway;
		//*)

	protected:

		//(*Identifiers(pnlNetworkSetup)
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL1;
		static const long ID_M_PEDT1;
		static const long ID_M_PLBL2;
		static const long ID_M_PEDT2;
		static const long ID_M_PLBL4;
		static const long ID_M_PEDT3;
		static const long ID_M_PBTN2;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL3;
		//*)

	private:

		//(*Handlers(pnlNetworkSetup)
		void OntnStaticDHCPClick(wxCommandEvent& event);
		void OnedtAddressTextEnter(wxCommandEvent& event);
		void OnedtSubnetTextEnter(wxCommandEvent& event);
		void OnedtGatewayText(wxCommandEvent& event);
		void OnbtnApplyClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnedtGatewayTextEnter(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
