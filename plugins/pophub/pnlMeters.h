#ifndef POPHUB_PNLMETERS_H
#define POPHUB_PNLMETERS_H

#include <wx/panel.h>

#include "wmlabel.h"
#include "wmlist.h"

class pophubBuilder;

class pnlMeters: public wxPanel
{
	public:

		pnlMeters(wxWindow* parent, pophubBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlMeters();

	protected:

		static const long ID_M_PLST14;

	private:

		void HideButtons(unsigned long nView);

		pophubBuilder* m_pBuilder;

		wmLabel* m_plblLeft;
		wmLabel* m_plblRight;

		wmList* m_plstLeft;
		wmList* m_plstRight;

		DECLARE_EVENT_TABLE()
};

#endif
