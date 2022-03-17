#ifndef PNLGROUPS_H
#define PNLGROUPS_H

//(*Headers(pnlGroups)
#include <wx/panel.h>
#include "wmlist.h"
//*)
#include "session.h"

class R128Builder;

class pnlGroups: public wxPanel
{
	public:

		pnlGroups(wxWindow* parent,R128Builder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlGroups();

		wmList* m_plstGroups;

		void SetChannels(const std::vector<subsession::channelGrouping>& vChannels);

	protected:

		static const long ID_M_PLST3;

	private:

		R128Builder* m_pBuilder;

		DECLARE_EVENT_TABLE()
};

#endif
