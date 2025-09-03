#ifndef POPHUB_PNLPHASE_H
#define POPHUB_PNLPHASE_H

#include <wx/panel.h>

#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"

class pophubBuilder;

class pnlPhase: public wxPanel
{
	public:

		pnlPhase(wxWindow* parent,pophubBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlPhase();


	protected:


	private:

		pophubBuilder* m_pBuilder;

		wmButton* m_pbtnDisplay;
		wmButton* m_pbtnDown;
		wmButton* m_pbtnUp;
		wmLabel* m_plblThresholdTitle;
		wmLabel* m_plblThreshold;
		wmList* m_plstThresholds;
		
		DECLARE_EVENT_TABLE()
};

#endif