#ifndef PNLCHANNELDELAY_H
#define PNLCHANNELDELAY_H

//(*Headers(pnlChannelDelay)
#include "offsetgraph.h"
#include "wmedit.h"
#include "wmkeyboard.h"
#include "wmlabel.h"
#include "wmlist.h"
#include <wx/panel.h>
//*)

class timedbuffer;
struct session;
class ChannelDelayBuilder;
class SettingEvent;

class pnlChannelDelay: public wxPanel
{
	public:

		pnlChannelDelay(wxWindow* parent,ChannelDelayBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlChannelDelay();

        void SetData(const timedbuffer* pBuffer);
        void SetTotalSamples(size_t nSamples);

        void InputSession(const session& aSession);


		//(*Declarations(pnlChannelDelay)
		OffsetGraph* m_pGraph;
		wmEdit* m_pedtMaxDelay;
		wmKeyboard* m_pKbd1;
		wmLabel* m_pLbl12;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl5;
		wmLabel* m_pLbl6;
		wmLabel* m_plblCurrent;
		wmList* m_plstChannel1;
		wmList* m_plstChannel2;
		//*)

	protected:

		//(*Identifiers(pnlChannelDelay)
		static const long ID_CUSTOM9;
		static const long ID_M_PLBL37;
		static const long ID_M_PKBD1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PEDT1;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL6;
		static const long ID_M_PLST2;
		static const long ID_M_PLBL7;
		static const long ID_M_PLST1;
		//*)

	private:

		//(*Handlers(pnlChannelDelay)
		void OnedtMaxDelayTextEnter(wxCommandEvent& event);
		void OnlstChannel1Selected(wxCommandEvent& event);
		void OnlstChannel2Selected(wxCommandEvent& event);
		//*)

        void OnSettingChanged(SettingEvent& event);

		void OnOffsetDone(wxCommandEvent& event);
		void CalculateOffset();

		std::vector<float> m_vBufferL;
		std::vector<float> m_vBufferR;
        size_t m_nTotalSamples;

        unsigned int m_nTotalChannels;

        unsigned int m_nChannel[2];
        wxString m_sChannel[2];

		double m_dSampleRate = 48000.0;

		int m_nOffset = 0;

        ChannelDelayBuilder* m_pBuilder;


		DECLARE_EVENT_TABLE()
};

#endif
