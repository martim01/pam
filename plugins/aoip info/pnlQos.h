#ifndef PNLQOS_H
#define PNLQOS_H

//(*Headers(pnlQos)
#include "histogram.h"
#include "historygraph.h"
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
//*)
#include "aoipinfobuilder.h"
#include <memory>
#include <limits>

struct rtpFrame;
class SettingEvent;

class pnlQos: public wxPanel
{
	public:

		pnlQos(wxWindow* parent, const wxString& sGroup, AoIPInfoBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlQos();

		//(*Declarations(pnlQos)
		Histogram* m_pHistogram;
		HistoryGraph* m_pGraph;
		wmButton* m_pbtnClear;
		wmButton* m_pbtnCurrentGraph;
		wmButton* m_pbtnGraphType;
		wmButton* m_pbtnRange;
		wmLabel* m_pLbl10;
		wmLabel* m_pLbl11;
		wmLabel* m_pLbl12;
		wmLabel* m_pLbl13;
		wmLabel* m_pLbl14;
		wmLabel* m_pLbl15;
		wmLabel* m_pLbl16;
		wmLabel* m_pLbl17;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl39;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl45;
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl5;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl7;
		wmLabel* m_pLbl8;
		wmLabel* m_pLbl9;
		wmLabel* m_plblQoSInter;
		wmLabel* m_plblQoSInterAv;
		wmLabel* m_plblQoSInterMax;
		wmLabel* m_plblQoSInterMin;
		wmLabel* m_plblQoSJitter;
		wmLabel* m_plblQoSKb;
		wmLabel* m_plblQoSKbAv;
		wmLabel* m_plblQoSKbMax;
		wmLabel* m_plblQoSKbMin;
		wmLabel* m_plblQoSLost;
		wmLabel* m_plblQoSPacketsAv;
		wmLabel* m_plblQoSPacketsMax;
		wmLabel* m_plblQoSPacketsMin;
		wmLabel* m_plblQoSReceived;
		wmLabel* m_plblQoSTime;
		wmLabel* m_plblRTCP_Last;
		wmLabel* m_plblRTCP_NTP;
		wmLabel* m_plblTSDF;
		wxPanel* Panel1;
		//*)
		void QoSUpdated(qosData* pData);
		void SetAudioData(const timedbuffer* pTimedBuffer);
		void RtpFrame(std::shared_ptr<const rtpFrame> pFrame);

	protected:

		//(*Identifiers(pnlQos)
		static const long ID_CUSTOM12;
		static const long ID_CUSTOM1;
		static const long ID_PANEL4;
		static const long ID_M_PLBL90;
		static const long ID_M_PBTN29;
		static const long ID_M_PBTN1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN3;
		static const long ID_M_PLBL89;
		static const long ID_M_PLBL35;
		static const long ID_M_PLBL85;
		static const long ID_M_PLBL32;
		static const long ID_M_PLBL31;
		static const long ID_M_PLBL30;
		static const long ID_M_PLBL29;
		static const long ID_M_PLBL33;
		static const long ID_M_PLBL28;
		static const long ID_M_PLBL27;
		static const long ID_M_PLBL26;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL25;
		static const long ID_M_PLBL24;
		static const long ID_M_PLBL23;
		static const long ID_M_PLBL22;
		static const long ID_M_PLBL21;
		static const long ID_M_PLBL20;
		static const long ID_M_PLBL19;
		static const long ID_M_PLBL18;
		static const long ID_M_PLBL17;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL15;
		static const long ID_M_PLBL16;
		static const long ID_M_PLBL14;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL12;
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL11;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		//*)


		void ShowGraph(const wxString& sGraph);
        void SetGraphType(const wxString& sType);

	private:

		//(*Handlers(pnlQos)
		void OnbtnRangeClick(wxCommandEvent& event);
		void OnbtnClearClick(wxCommandEvent& event);
		//*)

        void OnSettingChanged(SettingEvent& event);

        wxString m_sGroup;
		wxString m_sGraph;

		double m_dInitialLatency=0.0;
		unsigned short m_nLatencyCounter = 0;

		double m_dTotalMaxInterPacket = -1.0;
		double m_dTotalMinInterPacket = std::numeric_limits<double>::max();
		DECLARE_EVENT_TABLE()
};

#endif
