#ifndef PNLNETWORKSETUP_H
#define PNLNETWORKSETUP_H

//(*Headers(pnlNetworkSetup)
#include "wmbutton.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
//*)
#include "wmipeditpnl.h"
#include <wx/timer.h>

class pnlNetworkSetup: public wxPanel
{
	public:

		pnlNetworkSetup(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle = 0, const wxString& sEmpty = wxEmptyString);
		virtual ~pnlNetworkSetup();

		//(*Declarations(pnlNetworkSetup)
		wmButton* m_pbtnApply;
		wmButton* m_pbtnMask;
		wmButton* m_pbtnStaticDHCP;
		wmKeyboard* m_pkbd;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl5;
		wmLabel* m_plblConnected;
		wmLabel* m_plblResult;
		wmLabel* m_plblType;
		wmList* m_plstInterfaces;
		wmSwitcherPanel* m_pSwp1;
		wmipeditpnl* m_ppnlAddress;
		wmipeditpnl* m_ppnlGateway;
		wxPanel* Panel1;
		//*)

		wmButton* m_pbtnScan;
	protected:

		//(*Identifiers(pnlNetworkSetup)
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL4;
		static const long ID_PANEL3;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL3;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL5;
		static const long ID_M_PBTN2;
		static const long ID_PANEL1;
		static const long ID_M_PSWP1;
		static const long ID_PANEL2;
		static const long ID_M_PLST1;
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
		void OnbtnMaskClick(wxCommandEvent& event);
		void OnlstSubnetSelected(wxCommandEvent& event);
		void OnlstInterfacesSelected(wxCommandEvent& event);
		void Onm_pbtnApplyClick(wxCommandEvent& event);
		//*)
        void OnbtnScanClick(wxCommandEvent& event);
        void OnTimerRefresh(wxTimerEvent& event);
        void ShowConnectionDetails();

		wxString m_sInterface;
		wxTimer m_timerRefresh;

		DECLARE_EVENT_TABLE()
};

#endif
