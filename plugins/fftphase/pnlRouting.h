#ifndef PNLROUTING_H
#define PNLROUTING_H

//(*Headers(pnlRouting)
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)
#include <vector>
#include "session.h"

class fftphaseBuilder;
class pnlRouting: public wxPanel
{
	public:

		pnlRouting(wxWindow* parent,fftphaseBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlRouting();

		//(*Declarations(pnlRouting)
		wmList* m_plstY;
		wmLabel* m_pLbl1;
		wmList* m_plstX;
		wmList* m_plstMonitor;
		wmLabel* m_pLbl2;
		//*)

		void SetChannels(const std::vector<subsession::channelGrouping>& vChannels);

		void SetAxis(unsigned int nLeft, unsigned int nRight);

	protected:

		//(*Identifiers(pnlRouting)
		static const long ID_M_PLBL2;
		static const long ID_M_PLST1;
		static const long ID_M_PLST2;
		static const long ID_M_PLBL1;
		static const long ID_M_PLST3;
		//*)

	private:

		//(*Handlers(pnlRouting)
		void OnlstYSelected(wxCommandEvent& event);
		void OnlstXSelected(wxCommandEvent& event);
		void OnlstMonitorSelected(wxCommandEvent& event);
		//*)

		void EnableLists(bool bEnable);
		fftphaseBuilder* m_pBuilder;


		DECLARE_EVENT_TABLE()
};

#endif
