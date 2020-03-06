#ifndef PNLAOIPSELECTION_H
#define PNLAOIPSELECTION_H

//(*Headers(pnlAoIPSelection)
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)

class pnlAoIPSelection: public wxPanel
{
	public:

		pnlAoIPSelection(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, int n=0, const wxString& s=wxEmptyString);
		virtual ~pnlAoIPSelection();

		//(*Declarations(pnlAoIPSelection)
		wmButton* m_pbtnBack;
		wmButton* m_pbtnEnd;
		wmButton* m_pbtnHome;
		wmButton* m_pbtnNext;
		wmButton* m_pbtnPrevious;
		wmLabel* m_pLbl3;
		wmList* m_plstDevices;
		//*)

	protected:

		//(*Identifiers(pnlAoIPSelection)
		static const long ID_M_PLBL3;
		static const long ID_M_PLST1;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN3;
		//*)

	private:

		//(*Handlers(pnlAoIPSelection)
		void OnlstDevicesSelected(wxCommandEvent& event);
		void OnbtnHomeClick(wxCommandEvent& event);
		void OnbtnPreviousClick(wxCommandEvent& event);
		void OnbtnNextClick(wxCommandEvent& event);
		void OnbtnEndClick(wxCommandEvent& event);
		void OnbtnBackClick(wxCommandEvent& event);
		//*)

		void OnShown(wxShowEvent& event);
		void ShowPagingButtons();
        void ShowMainScreen();
		DECLARE_EVENT_TABLE()
};

#endif
