#ifndef POPHUB_PNLVIEW_H
#define POPHUB_PNLVIEW_H

#include <wx/panel.h>

#include "wmlist.h"

class pophubBuilder;

class pnlView: public wxPanel
{
	public:

		pnlView(wxWindow* parent,pophubBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlView();


	protected:

	private:
		pophubBuilder* m_pBuilder;

		wmList* m_plstView;

		DECLARE_EVENT_TABLE()
};

#endif