#ifndef PNLAOIPINFO_H
#define PNLAOIPINFO_H

//(*Headers(pnlAoIPInfo)
#include "wmlabel.h"
#include <wx/panel.h>
//*)

struct session;
struct qosData;
class timedbuffer;

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
		wmLabel* m_plblDuration;
		wmLabel* m_pLbl7;
		wmLabel* m_plblQoSInterMin;
		wmLabel* m_plblQoSPacketsAv;
		wmLabel* m_pLbl24;
		wmLabel* m_plblSessionChannels;
		wmLabel* m_plblQoSReceived;
		wmLabel* m_plblQoSPacketsMax;
		wmLabel* m_pLbl11;
		wmLabel* m_plblLatency;
		wmLabel* m_plblSessionBits;
		wmLabel* m_plblQoSKbMin;
		wmLabel* m_plblCurrentTimestamp;
		wmLabel* m_pLbl16;
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl30;
		wmLabel* m_plblTimestampOut;
		wmLabel* m_pLbl32;
		wmLabel* m_pLbl28;
		wmLabel* m_plblQoSKbMax;
		wmLabel* m_plblQoSKbAv;
		wmLabel* m_pLbl12;
		wmLabel* m_plblSessionFrequency;
		wmLabel* m_pLbl20;
		wmLabel* m_pLbl27;
		wmLabel* m_pLbl26;
		wmLabel* m_plblQoSInterMax;
		wmLabel* m_pLbl10;
		wmLabel* m_pLbl22;
		wmLabel* m_pLbl5;
		wmLabel* m_plblSessionType;
		wmLabel* m_plblTransmissionTime;
		wmLabel* m_plblTimestampIn;
		wmLabel* m_pLbl6;
		wmLabel* m_pLbl29;
		wmLabel* m_pLbl8;
		wmLabel* m_plblLatencyNetwork;
		wmLabel* m_pLbl2;
		wmLabel* m_plblQoSJitter;
		wmLabel* m_plblSyncTimestamp;
		wmLabel* m_pLbl14;
		wmLabel* m_plblSessionSource;
		wmLabel* m_plblQoSLost;
		wmLabel* m_pLbl33;
		wmLabel* m_pLbl9;
		wmLabel* m_pLbl34;
		wmLabel* m_pLbl19;
		wmLabel* m_plblQoSInterAv;
		wmLabel* m_pLbl15;
		wmLabel* m_pLbl18;
		wmLabel* m_pLbl31;
		wmLabel* m_plblQoSPacketsMin;
		wmLabel* m_plblSessionName;
		wmLabel* m_plblInput;
		wmLabel* m_pLbl21;
		wmLabel* m_pLbl13;
		//*)

        void QoSUpdated(qosData* pData);
		void SessionStarted(session* pSession);

        void AddData(const timedbuffer* pTimedBudder, unsigned int nFrameSize);


	protected:

		//(*Identifiers(pnlAoIPInfo)
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL59;
		static const long ID_M_PLBL38;
		static const long ID_M_PLBL39;
		static const long ID_M_PLBL44;
		static const long ID_M_PLBL45;
		static const long ID_M_PLBL46;
		static const long ID_M_PLBL40;
		static const long ID_M_PLBL60;
		static const long ID_M_PLBL41;
		static const long ID_M_PLBL42;
		static const long ID_M_PLBL47;
		static const long ID_M_PLBL48;
		static const long ID_M_PLBL49;
		static const long ID_M_PLBL52;
		static const long ID_M_PLBL54;
		static const long ID_M_PLBL61;
		static const long ID_M_PLBL55;
		static const long ID_M_PLBL57;
		static const long ID_M_PLBL62;
		static const long ID_M_PLBL43;
		static const long ID_M_PLBL50;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL51;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL53;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL56;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL58;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL11;
		static const long ID_M_PLBL12;
		static const long ID_M_PLBL13;
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
		static const long ID_M_PLBL33;
		static const long ID_M_PLBL34;
		static const long ID_M_PLBL35;
		static const long ID_M_PLBL36;
		static const long ID_M_PLBL37;
		//*)

	private:

		//(*Handlers(pnlAoIPInfo)
		//*)
		void SetTimestamp(const pairTime_t& tv, wmLabel* pLabel);
        void ShowLatency(const timedbuffer* pTimedBuffer);
        pairTime_t m_tvTransmission;
        pairTime_t m_tvPresentation;
        pairTime_t m_tvPlayback;


		DECLARE_EVENT_TABLE()
};

#endif
