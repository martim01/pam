#pragma once

//(*Headers(LTCPanel)
#include "wmlabel.h"
#include <wx/panel.h>
//*)

#include <vector>
#include "pmpanel.h"

class LTCBuilder;
class timedbuffer;
class session;
class LtcDecoder;

class LTCPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(LTCPanel);

		LTCPanel(){}
		LTCPanel(wxWindow* parent,LTCBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~LTCPanel();

		//(*Declarations(LTCPanel)
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl3;
		wmLabel* m_plblLTCTime;
		wmLabel* m_pLbl1;
		wmLabel* m_plblFPS;
		wmLabel* m_plblLTCVolume;
		wmLabel* m_pLbl2;
		wmLabel* m_plblRaw;
		wmLabel* m_plblLTCDate;
		//*)

		void SetAudioData(const timedbuffer* pBuffer);
		void InputSession(const session& aSession);
		void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers(LTCPanel)
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(LTCPanel)
		//*)

		LTCBuilder* m_pBuilder;
		LtcDecoder* m_pDecoder;

		unsigned int m_nInputChannels;
		DECLARE_EVENT_TABLE()
};

