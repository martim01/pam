#ifndef PNLGRAPH_H
#define PNLGRAPH_H

//(*Headers(pnlGraph)
#include "wmbutton.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)

class AoIPInfoBuilder;

class pnlGraph: public wxPanel
{
	public:

		pnlGraph(wxWindow* parent, AoIPInfoBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlGraph();

		//(*Declarations(pnlGraph)
		wmButton* m_pbtnClear;
		wmList* m_plstGraph;
		//*)

	protected:

		//(*Identifiers(pnlGraph)
		static const long ID_M_PLST16;
		static const long ID_M_PBTN29;
		//*)

	private:

		//(*Handlers(pnlGraph)
		void OnlstGraphSelected(wxCommandEvent& event);
		void OnbtnClearClick(wxCommandEvent& event);
		//*)
		AoIPInfoBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
