#ifndef PNLRTCPTRANSMISSION_H
#define PNLRTCPTRANSMISSION_H

//(*Headers(pnlRTCPTransmission)
#include "levelgraph.h"
#include "wmbutton.h"
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
//*)

#include <map>
#include <memory>
#include <limits>
#include "historygraph.h"
#include "histogram.h"

class RTCPTransmissionEvent;

class pnlRTCPTransmission: public wxPanel
{
	public:

		pnlRTCPTransmission(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, int nStyle=0, const wxString& str="");
		virtual ~pnlRTCPTransmission();

		HistoryGraph* m_pGraph;
		Histogram* m_pHistogram;
		wmButton* m_pbtnClose;
		wmLabel* m_pLbl10;
		wmLabel* m_pLbl11;
		wmLabel* m_pLbl12;
		wmLabel* m_pLbl13;
		wmLabel* m_pLbl14;
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
		wmLabel* m_plblGraph;
		wmLabel* m_plblHostname;
		wmLabel* m_plblJitter;
		wmLabel* m_plblKbAv;
		wmLabel* m_plblKbMax;
		wmLabel* m_plblKbMin;
		wmLabel* m_plblPacketFirst;
		wmLabel* m_plblPacketLatest;
		wmLabel* m_plblPacketsAv;
		wmLabel* m_plblPacketsLost;
		wmLabel* m_plblPacketsMax;
		wmLabel* m_plblPacketsMin;
		wmLabel* m_plblPacketsTotal;
		wmLabel* m_plblRRFirst;
		wmLabel* m_plblRRGap;
		wmLabel* m_plblRRLast;
		wmLabel* m_plblRRSR;
		wmLabel* m_plblRoundtrip;
		wmLabel* m_plblSubscriber;
		wmLabel* m_plblSSRC;
		wmList* m_plstOptions;
		wmList* m_plstSubscribers;
		wmSwitcherPanel* m_pswpMain;
		wmSwitcherPanel* m_pswpOptions;
		wxPanel* Panel1;
		wxPanel* Panel2;
		wxPanel* Panel3;
		wxPanel* Panel4;
		wxPanel* pnlQoS;
		//*)

		wmList* m_plstType;
		wmButton* m_pbtnClear;
		wmButton* m_pbtnRange;

        wmList* m_plstGranularity;
        wmList* m_plstResolution;
        wmLabel* m_plblHR;
        wmLabel* m_plblHG;

	protected:

		//(*Identifiers(pnlRTCPTransmission)
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL11;
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL12;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL26;
		static const long ID_M_PLBL27;
		static const long ID_M_PLBL28;
		static const long ID_M_PLBL14;
		static const long ID_M_PLBL15;
		static const long ID_M_PLBL16;
		static const long ID_M_PLBL17;
		static const long ID_M_PLBL18;
		static const long ID_M_PLBL19;
		static const long ID_M_PLBL20;
		static const long ID_M_PLBL21;
		static const long ID_M_PLBL22;
		static const long ID_M_PLBL23;
		static const long ID_M_PLBL24;
		static const long ID_M_PLBL25;
		static const long ID_M_PLBL32;
		static const long ID_M_PLBL85;
		static const long ID_M_PLBL35;
		static const long ID_M_PLBL89;
		static const long ID_M_PLBL90;
		static const long ID_M_PLBL86;
		static const long ID_CUSTOM12;
		static const long ID_PANEL4;
		static const long ID_PANEL2;
		static const long ID_M_PLST1;
		static const long ID_PANEL1;
		static const long ID_M_PLST2;
		static const long ID_PANEL3;
		static const long ID_M_PSWP2;
		static const long ID_PANEL6;
		static const long ID_M_PSWP1;
		static const long ID_M_PBTN1;
		static const long ID_M_PLBL29;
		static const long ID_M_PLBL37;
		//*)

	private:

		//(*Handlers(pnlRTCPTransmission)
		void OnbtnCloseClick(wxCommandEvent& event);
		//*)

		void OnRTCPTransmissionEvent(const RTCPTransmissionEvent& event);

		void OnGraphSelected(const wxCommandEvent& event);
		void OnSubscriberSelected(const wxCommandEvent& event);
        void ShowSubscriber();

        void OnConnectionEvent(const wxCommandEvent& event);
        void OnDisconnectionEvent(const wxCommandEvent& event);

        void OnlstResolutionPressed(const wxCommandEvent& event);
        void OnlstGranularityPressed(const wxCommandEvent& event);


        void AddGraphs(const wxString& sSource);

        void OnbtnClearClick(wxCommandEvent& event);
        void OnbtnRangeClick(wxCommandEvent& event);
		void OnlstTypeSelected(wxCommandEvent& event);

        void ClearStats();

        void ShowGraph();

        void ClearGraphs(const wxString& sSubscriber);


        struct subscriber
        {
            subscriber() : pStats(nullptr), dtConnection(wxDateTime::Now()), dtLast(wxDateTime::Now()), dKbpsMax(-1), dKbpsMin(std::numeric_limits<double>::max()),
             dLostpsMax(-1), dLostpsMin(std::numeric_limits<double>::max()),nLastOctets(0){}
            RTCPTransmissionEvent* pStats;
            wxDateTime dtConnection;
            wxDateTime dtLast;
            double dKbpsMax;
            double dKbpsMin;
            double dLostpsMax;
            double dLostpsMin;
            int64_t nFirstOctets;
            int64_t nLastOctets;
            unsigned int nSSRC;

        };

        subscriber& AddSubscriber(const wxString& sIpAddress, unsigned int nSSRC);

        void StoreGraphs(const wxString& sIpAddress, subscriber& sub);

		std::map<wxString, subscriber> m_mSubscribers;

		wxString m_sSelected;
		wxString m_sGraph;

		static const wxString KBPS;
		static const wxString PLPS;
		static const wxString JITTER;
		static const wxString DELAY;
		static const wxString LOSS_RATIO;


		DECLARE_EVENT_TABLE()
};

#endif
