#ifndef PNLGRAPH_H
#define PNLGRAPH_H

//(*Headers(pnlGraph)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class AoIPInfoBuilder;

class pnlGraph: public wxPanel
{
	public:

		pnlGraph(wxWindow* parent, AoIPInfoBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlGraph();

		//(*Declarations(pnlGraph)
		wmList* m_plstGraph;
		//*)

	protected:

		//(*Identifiers(pnlGraph)
		static const long ID_M_PLST16;
		//*)

	private:

		//(*Handlers(pnlGraph)
		void OnlstGraphSelected(wxCommandEvent& event);
		//*)
		AoIPInfoBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
