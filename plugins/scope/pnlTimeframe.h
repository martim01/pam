#ifndef PNLTIMEFRAME_H
#define PNLTIMEFRAME_H

//(*Headers(pnlTimeframe)
#include "wmbutton.h"
#include <wx/panel.h>
//*)
class ScopeBuilder;

class pnlTimeframe: public wxPanel
{
	public:

		pnlTimeframe(wxWindow* parent,ScopeBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlTimeframe();

		//(*Declarations(pnlTimeframe)
		wmButton* m_pbtnScopeIn1;
		wmButton* m_pbtnScopeOut100;
		wmButton* m_pbtnScopeIn10;
		wmButton* m_btnScopeIn100;
		wmButton* m_pbtnScopeOut1;
		wmButton* m_pbtnScopeOut10;
		wmButton* m_pbtnScopeIn1000;
		wmButton* m_pbtnScopeOut1000;
		//*)

	protected:

		//(*Identifiers(pnlTimeframe)
		static const long ID_M_PBTN9;
		static const long ID_M_PBTN8;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PBTN4;
		static const long ID_M_PBTN5;
		static const long ID_M_PBTN6;
		static const long ID_M_PBTN7;
		//*)

	private:

		//(*Handlers(pnlTimeframe)
		void OnbtnScopeIn1000Click(wxCommandEvent& event);
		void OnbtnScopeOut1000Click(wxCommandEvent& event);
		void OnbtnScopeIn100Click(wxCommandEvent& event);
		void OnbtnScopeOut100Click(wxCommandEvent& event);
		void OnbtnScopeIn10Click(wxCommandEvent& event);
		void OnbtnScopeOut10Click(wxCommandEvent& event);
		void OnbtnScopeIn1Click(wxCommandEvent& event);
		void OnbtnScopeOut1Click(wxCommandEvent& event);
		//*)

		ScopeBuilder* m_pBuilder;

		void AlterTimeframe(double dMod);
		DECLARE_EVENT_TABLE()
};

#endif
