#ifndef PNLFLAGS_H
#define PNLFLAGS_H

//(*Headers(pnlFlags)
#include "wmlabel.h"
#include <wx/panel.h>
//*)

class pnlFlags: public wxPanel
{
	public:

		pnlFlags(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, int nStyle=0, const wxString& sEmpty=wxEmptyString);
		virtual ~pnlFlags();

		void ShowFlags(unsigned short nFlags);

		//(*Declarations(pnlFlags)
		wmLabel* m_plblFlags2Step;
		wmLabel* m_plblFlags59;
		wmLabel* m_plblFlags61;
		wmLabel* m_plblFlagsAlternate;
		wmLabel* m_plblFlagsFreq;
		wmLabel* m_plblFlagsProfile1;
		wmLabel* m_plblFlagsProfile2;
		wmLabel* m_plblFlagsScale;
		wmLabel* m_plblFlagsTime;
		wmLabel* m_plblFlagsUTC;
		wmLabel* m_plblFlagsUnicast;
		//*)

	protected:

		//(*Identifiers(pnlFlags)
		static const long ID_M_PLBL35;
		static const long ID_M_PLBL39;
		static const long ID_M_PLBL45;
		static const long ID_M_PLBL53;
		static const long ID_M_PLBL65;
		static const long ID_M_PLBL66;
		static const long ID_M_PLBL67;
		static const long ID_M_PLBL68;
		static const long ID_M_PLBL69;
		static const long ID_M_PLBL70;
		static const long ID_M_PLBL71;
		//*)

	private:

		//(*Handlers(pnlFlags)
		//*)

		static const wxColour CLR_OFF;
		static const wxColour CLR_ON;

		DECLARE_EVENT_TABLE()
};

#endif
