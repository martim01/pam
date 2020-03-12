#ifndef PNLAOIPSELECTION_H
#define PNLAOIPSELECTION_H

//(*Headers(pnlAoIPSelection)
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
//*)

#include <list>
#include <map>

struct AoIPSource;

class pnlAoIPSelection: public wxPanel
{
	public:

		pnlAoIPSelection(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, int n=0, const wxString& s=wxEmptyString);
		virtual ~pnlAoIPSelection();

		//(*Declarations(pnlAoIPSelection)
		wmButton* m_pbtnBack;
		wmButton* m_pbtnDelete;
		wmButton* m_pbtnEnd;
		wmButton* m_pbtnHome;
		wmButton* m_pbtnNext;
		wmButton* m_pbtnPrevious;
		wmLabel* m_pLbl3;
		wmLabel* m_plblSearch;
		wmList* m_plstAlphabet;
		wmList* m_plstAlphebtical;
		wmList* m_plstDevices;
		wmList* m_plstTagged;
		wmList* m_plstTags;
		wmSwitcherPanel* m_pSwp1;
		wxPanel* Panel1;
		wxPanel* Panel2;
		wxPanel* Panel3;
		//*)

	protected:

		//(*Identifiers(pnlAoIPSelection)
		static const long ID_M_PLBL3;
		static const long ID_M_PBTN3;
		static const long ID_M_PLST1;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_PANEL1;
		static const long ID_M_PLST2;
		static const long ID_M_PLBL1;
		static const long ID_M_PLST3;
		static const long ID_M_PBTN6;
		static const long ID_PANEL2;
		static const long ID_M_PLST4;
		static const long ID_M_PLST5;
		static const long ID_PANEL3;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(pnlAoIPSelection)
		void OnlstDevicesSelected(wxCommandEvent& event);
		void OnbtnHomeClick(wxCommandEvent& event);
		void OnbtnPreviousClick(wxCommandEvent& event);
		void OnbtnNextClick(wxCommandEvent& event);
		void OnbtnEndClick(wxCommandEvent& event);
		void OnbtnBackClick(wxCommandEvent& event);
		void OnlstAlphabetSelected(wxCommandEvent& event);
		void OnlstTagsSelected(wxCommandEvent& event);
		void OnSwpPageChanged(wxBookCtrlEvent& event);
		void OnlstDevicesPaged(wxCommandEvent& event);
		void OnbtnDeleteClick(wxCommandEvent& event);
		//*)

		void OnShown(wxShowEvent& event);
		void ShowPagingButtons();
        void ShowMainScreen();

        void ShowPaged();
        void ShowAlphabetical();
        void ShowTagged();

        void DoAlphabetSearch(const wxString& sLetter);
        void DoTagSearch(const wxString& sTag);

        std::list<wxString> m_lstTag;
        std::map<unsigned int, AoIPSource> m_mTagged;
        wxString m_sSearch;
		DECLARE_EVENT_TABLE()
};

#endif
