#ifndef PNLROUTIING_H
#define PNLROUTIING_H

//(*Headers(pnlRoutiing)
#include <wx/panel.h>
#include "wmlist.h"
//*)
#include "session.h"

class FFTBuilder;

class pnlRoutiing: public wxPanel
{
	public:

		pnlRoutiing(wxWindow* parent,FFTBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlRoutiing();

		wmList* m_plstFFT_Routing;

		void SetChannels(const std::vector<subsession::channelGrouping>& vChannels);

	protected:

		static const long ID_M_PLST3;

	private:

		FFTBuilder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
