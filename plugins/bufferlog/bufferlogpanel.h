#pragma once

//(*Headers(bufferlogPanel)
#include "wmlabel.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

#include <vector>
#include "pmpanel.h"

class bufferlogBuilder;
class timedbuffer;
class session;

class bufferlogPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(bufferlogPanel);

		bufferlogPanel(){}

		bufferlogPanel(wxWindow* parent,bufferlogBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~bufferlogPanel();

		//(*Declarations(bufferlogPanel)
		wmLabel* m_plblLess;
		wmLabel* m_plblBuffer;
		wmLabel* m_pLbl1;
		wmList* m_plstBuffer;
		wmLabel* m_pLbl2;
		//*)

		void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);
		void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers(bufferlogPanel)
		static const long ID_M_PLST1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL3;
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(bufferlogPanel)
		//*)

		bufferlogBuilder* m_pBuilder;

		unsigned int m_nDefaultBuffer;
		unsigned int m_nLess;

		DECLARE_EVENT_TABLE()
};

