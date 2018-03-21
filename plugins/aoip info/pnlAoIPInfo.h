#ifndef PNLAOIPINFO_H
#define PNLAOIPINFO_H

//(*Headers(pnlAoIPInfo)
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include "levelgraph.h"
#include <wx/panel.h>
//*)

#include "timedbuffer.h"

struct qosData;
struct session;


class pnlAoIPInfo: public wxPanel
{
	public:

		pnlAoIPInfo(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlAoIPInfo();

		//(*Declarations(pnlAoIPInfo)
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl23;
		wmLabel* m_pLbl25;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl17;
		wmLabel* m_plblQoSTime;
		wxPanel* pnlSDP;
		wmLabel* m_pLbl7;
		wmLabel* m_plblSubSyncId;
		wmLabel* m_plblQoSInterMin;
		wmLabel* m_plblQoSPacketsAv;
		wmLabel* m_pLbl24;
		wmLabel* m_plblSessionChannels;
		wmLabel* m_plblQoSReceived;
		wmLabel* m_plblSubsessionVideo;
		wmLabel* m_plblQoSPacketsMax;
		wmLabel* m_pLbl11;
		wmLabel* m_plblLatency;
		wxPanel* pnlSubsession;
		wmLabel* m_plblSessionBits;
		wmLabel* m_plblQoSKbMin;
		wmLabel* m_plblCurrentTimestamp;
		wmLabel* m_pLbl16;
		wxTextCtrl* m_ptxtSDP;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl43;
		wmLabel* m_pLbl30;
		wmLabel* m_plblTimestampOut;
		wmLabel* m_pLbl32;
		wmLabel* m_pLbl28;
		wmLabel* m_plblQoSKbMax;
		wmLabel* m_plblQoSKbAv;
		wxPanel* Panel1;
		wmLabel* m_pLbl12;
		wmLabel* m_plblSessionFrequency;
		wmLabel* m_pLbl20;
		wmLabel* m_pLbl27;
		wmLabel* m_pLbl26;
		wmLabel* m_plblQoSInterMax;
		wmLabel* m_pLbl10;
		wmLabel* m_plblSyncId;
		wmLabel* m_pLbl42;
		wmLabel* m_pLbl22;
		wmLabel* m_pLbl40;
		wmLabel* m_pLbl41;
		wmLabel* m_pLbl5;
		wmLabel* m_plblSessionType;
		wxPanel* pnlQoS;
		wmLabel* m_plblTransmissionTime;
		wmLabel* m_plblTimestampIn;
		wmLabel* m_pLbl36;
		wmLabel* m_pLbl6;
		wmLabel* m_plblSubsessionsAudio;
		wmLabel* m_pLbl29;
		wmLabel* m_pLbl8;
		wmSwitcherPanel* m_pswpInfo;
		wmLabel* m_plblSubSyncVersion;
		wmLabel* m_pLbl35;
		wmLabel* m_plblLatencyNetwork;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl37;
		wmLabel* m_plblQoSJitter;
		wmLabel* m_plblSyncTimestamp;
		wmLabel* m_plblSubSyncType;
		wmLabel* m_pLbl14;
		wmLabel* m_plblSessionSource;
		wmLabel* m_plblQoSLost;
		wmLabel* m_plblFrameSize;
		wmLabel* m_pLbl33;
		wmLabel* m_pLbl9;
		wmLabel* m_pLbl34;
		wmLabel* m_plblGroups;
		wmLabel* m_pLbl19;
		wmLabel* m_plblQoSInterAv;
		wmLabel* m_pLbl15;
		wmLabel* m_pLbl31;
		wmLabel* m_plblSyncType;
		wmLabel* m_pLbl18;
		wmLabel* m_plblQoSPacketsMin;
		wxPanel* pnlSession;
		wmLabel* m_plblFrameDuration;
		LevelGraph* m_pGraph;
		wmLabel* m_plblSessionName;
		wmLabel* m_plblSyncDomain;
		wmLabel* m_plblSubSyncDomain;
		wmLabel* m_plblDescription;
		wmLabel* m_plblSyncVersion;
		wmLabel* m_plblInput;
		wmLabel* m_pLbl21;
		wmLabel* m_pLbl13;
		//*)

		void QoSUpdated(qosData* pData);
		void SessionStarted(const session& aSession);

        void SetAudioData(const timedbuffer* pTimedBudder);

        void ShowGraph(const wxString& sGraph);

        void ClearGraphs();

	protected:

		//(*Identifiers(pnlAoIPInfo)
		static const long ID_M_PLBL59;
		static const long ID_M_PLBL38;
		static const long ID_M_PLBL40;
		static const long ID_M_PLBL60;
		static const long ID_M_PLBL41;
		static const long ID_M_PLBL43;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL63;
		static const long ID_M_PLBL34;
		static const long ID_M_PLBL33;
		static const long ID_M_PLBL36;
		static const long ID_M_PLBL58;
		static const long ID_M_PLBL37;
		static const long ID_M_PLBL64;
		static const long ID_M_PLBL65;
		static const long ID_M_PLBL66;
		static const long ID_M_PLBL75;
		static const long ID_M_PLBL76;
		static const long ID_M_PLBL77;
		static const long ID_M_PLBL78;
		static const long ID_M_PLBL79;
		static const long ID_M_PLBL80;
		static const long pnlSessionInfo;
		static const long ID_M_PLBL39;
		static const long ID_M_PLBL42;
		static const long ID_M_PLBL44;
		static const long ID_M_PLBL47;
		static const long ID_M_PLBL45;
		static const long ID_M_PLBL48;
		static const long ID_M_PLBL46;
		static const long ID_M_PLBL49;
		static const long ID_M_PLBL67;
		static const long ID_M_PLBL68;
		static const long ID_M_PLBL69;
		static const long ID_M_PLBL70;
		static const long ID_M_PLBL71;
		static const long ID_M_PLBL72;
		static const long ID_M_PLBL74;
		static const long ID_M_PLBL73;
		static const long ID_M_PLBL52;
		static const long ID_M_PLBL54;
		static const long ID_M_PLBL50;
		static const long ID_M_PLBL55;
		static const long ID_M_PLBL57;
		static const long ID_M_PLBL51;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL53;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL61;
		static const long ID_M_PLBL81;
		static const long ID_M_PLBL82;
		static const long ID_M_PLBL83;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL56;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL7;
		static const long ID_PANEL1;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL11;
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL12;
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
		static const long ID_M_PLBL26;
		static const long ID_M_PLBL27;
		static const long ID_M_PLBL28;
		static const long ID_M_PLBL29;
		static const long ID_M_PLBL30;
		static const long ID_M_PLBL31;
		static const long ID_M_PLBL32;
		static const long ID_M_PLBL35;
		static const long ID_CUSTOM12;
		static const long ID_PANEL4;
		static const long ID_PANEL2;
		static const long ID_TEXTCTRL1;
		static const long ID_PANEL3;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(pnlAoIPInfo)
		//*)

		void SetTimestamp(const pairTime_t& tv, wmLabel* pLabel);
        void ShowLatency(const timedbuffer* pTimedBuffer);
        pairTime_t m_tvTransmission;
        pairTime_t m_tvPresentation;
        pairTime_t m_tvPlayback;

        unsigned int m_nSampleRate;
        unsigned int m_nFrameSize;

		DECLARE_EVENT_TABLE()
};

#endif
