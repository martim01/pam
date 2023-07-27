#ifndef PNLAOIPINFO_H
#define PNLAOIPINFO_H

//(*Headers(pnlAoIPInfo)
#include "wmlabel.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
//*)

#include "timedbuffer.h"
#include "wmlistadv.h"
#include "pnlSubsession.h"
#include "pnlQos.h"
#include <map>

struct qosData;
struct session;

class AoIPInfoBuilder;

class pnlAoIPInfo: public wxPanel
{
	public:

		pnlAoIPInfo(wxWindow* parent, AoIPInfoBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlAoIPInfo();

		void ChangeGranularity(int nWhich);
		void ChangeResolution(int nWhich);

		//(*Declarations(pnlAoIPInfo)
		wmLabel* m_pLbl18;
		wmLabel* m_pLbl19;
		wmLabel* m_pLbl20;
		wmLabel* m_pLbl21;
		wmLabel* m_pLbl24;
		wmLabel* m_pLbl32;
		wmLabel* m_pLbl33;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl40;
		wmLabel* m_pLbl42;
		wmLabel* m_plblDescription;
		wmLabel* m_plblGroups;
		wmLabel* m_plblInput;
		wmLabel* m_plblSessionName;
		wmLabel* m_plblSessionType;
		wmLabel* m_plblSubsessionsAudio;
		wmLabel* m_plblSyncDomain;
		wmLabel* m_plblSyncId;
		wmLabel* m_plblSyncType;
		wmLabel* m_plblSyncVersion;
		wmSwitcherPanel* m_pswpInfo;
		wxPanel* pnlSDP;
		wxPanel* pnlSession;
		wxTextCtrl* m_ptxtSDP;
		//*)
        wmListAdv* m_pSdp;



		wmLabel* m_plblEpoch;
		wmLabel* m_plblRTCP_Last;
		wmLabel* m_plblRTCP_Last_Title;
		wmLabel* m_plblRTCP_NTP_Title;
		wmLabel* m_plblRTCP_NTP;

		void QoSUpdated(qosData* pData);
		void SessionStarted(const session& aSession);

        void SetAudioData(const timedbuffer* pTimedBudder);

        void RecalculateRange();

        double GetLatency() const { return m_dLatency; }
        unsigned int GetSampleRate() const { return m_nSampleRate; }
        unsigned int GetFrameSize() const { return m_nFrameSize; }
        double GetSlip() const {return m_dSlip;}

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
		static const long ID_M_PLBL77;
		static const long ID_M_PLBL78;
		static const long ID_M_PLBL80;
		static const long pnlSessionInfo;
		static const long ID_TEXTCTRL1;
		static const long ID_PANEL3;
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(pnlAoIPInfo)
		//*)

		void OnPtpEvent(wxCommandEvent& event);

		void OnInfoLeftUp(wxMouseEvent& event);
		void ConnectLeftUp();

		void SetTimestamp(const timeval& tv, wmLabel* pLabel, bool bDate = false);
        void ShowLatency(const timedbuffer* pTimedBuffer);

        AoIPInfoBuilder* m_pBuilder;
        timeval m_tvTransmission;
        timeval m_tvPresentation;
        timeval m_tvPlayback;

        unsigned int m_nSampleRate;
        unsigned int m_nFrameSize;
        const session* m_pSession;
        
        double m_dFrameDuration;
        double m_dSlip;

        double m_dInitialLatency;
        int m_nInitialLatencyCounter;

        double m_dLatency;

        enum {GRAPH_MIN=0, GRAPH_MAX};

        std::map<wxString, pnlSubsession*> m_mSubsessions;
        std::map<wxString, pnlQos*> m_mQos;

		DECLARE_EVENT_TABLE()
};

#endif
