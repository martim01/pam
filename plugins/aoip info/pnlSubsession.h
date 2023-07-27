#ifndef PNLSUBSESSION_H
#define PNLSUBSESSION_H

//(*Headers(pnlSubsession)
#include "wmlabel.h"
#include <wx/panel.h>
//*)

#include "session.h"

class pnlSubsession: public wxPanel
{
	public:

		pnlSubsession(wxWindow* parent,const subsession& sub, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlSubsession();

		//(*Declarations(pnlSubsession)
		wmLabel* m_pLbl1;
		wmLabel* m_pLbl22;
		wmLabel* m_pLbl23;
		wmLabel* m_pLbl25;
		wmLabel* m_pLbl26;
		wmLabel* m_pLbl27;
		wmLabel* m_pLbl28;
		wmLabel* m_pLbl29;
		wmLabel* m_pLbl2;
		wmLabel* m_pLbl30;
		wmLabel* m_pLbl31;
		wmLabel* m_pLbl34;
		wmLabel* m_pLbl35;
		wmLabel* m_pLbl36;
		wmLabel* m_pLbl37;
		wmLabel* m_pLbl38;
		wmLabel* m_pLbl3;
		wmLabel* m_pLbl43;
		wmLabel* m_plblBuffer;
		wmLabel* m_plblContribution;
		wmLabel* m_plblCurrentTimestamp;
		wmLabel* m_plblFrameDuration;
		wmLabel* m_plblFrameSize;
		wmLabel* m_plblLatencyNetwork;
		wmLabel* m_plblSessionBits;
		wmLabel* m_plblSessionChannels;
		wmLabel* m_plblSessionFrequency;
		wmLabel* m_plblSessionSource;
		wmLabel* m_plblSubSyncDomain;
		wmLabel* m_plblSubSyncId;
		wmLabel* m_plblSubSyncType;
		wmLabel* m_plblSubSyncVersion;
		wmLabel* m_plblSubsessionId;
		wmLabel* m_plblSyncTimestamp;
		wmLabel* m_plblTimestampIn;
		wmLabel* m_plblTransmissionTime;
		//*)

		void SetAudioData(const timedbuffer* pTimedBuffer);

	protected:

		//(*Identifiers(pnlSubsession)
		static const long ID_M_PLBL39;
		static const long ID_M_PLBL42;
		static const long ID_M_PLBL44;
		static const long ID_M_PLBL47;
		static const long ID_M_PLBL45;
		static const long ID_M_PLBL48;
		static const long ID_M_PLBL46;
		static const long ID_M_PLBL62;
		static const long ID_M_PLBL49;
		static const long ID_M_PLBL84;
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
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL50;
		static const long ID_M_PLBL55;
		static const long ID_M_PLBL57;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL51;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL53;
		static const long ID_M_PLBL61;
		static const long ID_M_PLBL81;
		static const long ID_M_PLBL82;
		static const long ID_M_PLBL83;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL56;
		//*)

	private:

		//(*Handlers(pnlSubsession)
		//*)
		void OnPtpEvent(wxCommandEvent& event);

		subsession m_sub;
		DECLARE_EVENT_TABLE()
};

#endif
