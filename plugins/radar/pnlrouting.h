#ifndef PNLROUTING_H
#define PNLROUTING_H

//(*Headers(pnlRouting)
#include <wx/panel.h>
#include "wmlist.h"
//*)
#include "session.h"

class RadarBuilder;

class pnlRouting: public wxPanel
{
	public:

		pnlRouting(wxWindow* parent,RadarBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlRouting();

		//(*Declarations(pnlRouting)
		wmList* m_plstFFT_Routing;
		//*)

		void SetChannels(const std::vector<subsession::channelGrouping>& vChannels);

	protected:

		//(*Identifiers(pnlRouting)
		static const long ID_M_PLST3;
		//*)

	private:

		//(*Handlers(pnlRouting)
		void OnlstFFT_RoutingSelected(wxCommandEvent& event);
		//*)
		RadarBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
