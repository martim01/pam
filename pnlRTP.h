#ifndef PNLRTP_H
#define PNLRTP_H

//(*Headers(pnlRTP)
#include <wx/notebook.h>
#include "wmbutton.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/panel.h>
#include "wmedit.h"
#include "wmlist.h"
//*)

#include <set>

class wxServDisc;
class SapWatchThread;

class pnlRTP: public wxPanel
{
	public:

		pnlRTP(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, const wxString& sId=wxEmptyString);
		virtual ~pnlRTP();

		//(*Declarations(pnlRTP)
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl3;
		wmButton* m_pbtnConfirm;
		wmLabel* m_pLbl1;
		wmSwitcherPanel* m_pSwp1;
		wmButton* m_pbtnDiscover;
		wxPanel* Panel1;
		wmButton* m_pbtnAdd;
		wmEdit* m_pedtName;
		wmButton* m_pbtnCancel;
		wmLabel* m_pLbl5;
		wmButton* m_pbtnDelete;
		wmList* m_plstSources;
		wmEdit* m_pedtUrl;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl2;
		wmButton* m_pbtnUpdate;
		wmKeyboard* m_pkeyboard;
		wmButton* m_pbtnDeleteAll;
		wxPanel* Panel2;
		//*)

		void ListSources();

	protected:

		//(*Identifiers(pnlRTP)
		static const long ID_M_PLST1;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN7;
		static const long ID_M_PBTN6;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL3;
		static const long ID_PANEL1;
		static const long ID_M_PLBL1;
		static const long ID_M_PEDT1;
		static const long ID_M_PLBL2;
		static const long ID_M_PEDT2;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL6;
		static const long ID_PANEL2;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(pnlRTP)
		void OnbtnAddClick(wxCommandEvent& event);
		void OnedtNameTextEnter(wxCommandEvent& event);
		void OnedtUrlTextEnter(wxCommandEvent& event);
		void OnbtnConfirmClick(wxCommandEvent& event);
		void OnbtnUpdateClick(wxCommandEvent& event);
		void OnbtnDeleteClick(wxCommandEvent& event);
		void OnlstSourcesSelected(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnDiscoverClick(wxCommandEvent& event);
		void OnbtnDeleteAllHeld(wxCommandEvent& event);
		//*)

		void OnDiscovery(wxCommandEvent& event);
		void OnSap(wxCommandEvent& event);
		wxString m_sSelectedSource;
		wxString m_sSelectedUrl;

        wxServDisc* m_pServscan;
        SapWatchThread* m_pSapWatch;
        size_t m_nDiscovered;
        std::set<std::pair<wxString, wxString> > m_setDiscover;
		DECLARE_EVENT_TABLE()
};

#endif
