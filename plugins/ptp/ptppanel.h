#pragma once

//(*Headers(ptpPanel)
#include "wmlabel.h"
#include "wmlist.h"
#include "wmswitcherpanel.h"
#include <wx/notebook.h>
#include <wx/panel.h>
//*)
#include <ctype.h>
#include <vector>
#include <wx/timer.h>
#include "pmpanel.h"
#include "macdb.h"
#include "pnlFlags.h"
#include <memory>

class ptpBuilder;
class timedbuffer;
class session;

namespace ptpmonkey
{
    class PtpV2Clock;
};


class ptpPanel: public pmPanel
{
	public:

		wxDECLARE_DYNAMIC_CLASS(ptpPanel);

		/** @brief default constructor
        **/
		ptpPanel() : pmPanel(){}

		/** @brief Creates the panel
        *   @param parent pointer to the parent window
        *   @param pBuilder pointer to the ptpBuilder object that built the plugin
		*   @param id the wxWindowId of the control
        *   @param pos the top-left point of the control
        *   @param size the width and height of the control
        **/
		ptpPanel(wxWindow* parent,ptpBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ptpPanel();

		//(*Declarations(ptpPanel)
		pnlFlags* m_ppnlAnnounceFlags;
		pnlFlags* m_ppnlFollowFlags;
		pnlFlags* m_ppnlRequestFlags;
		pnlFlags* m_ppnlResponseFlags;
		pnlFlags* m_ppnlSyncFlags;
		wmLabel* m_pblAddress;
		wmLabel* m_plblAccuracy;
		wmLabel* m_plblAnnCount;
		wmLabel* m_plblAnnRate;
		wmLabel* m_plblClass;
		wmLabel* m_plblDelayAverage;
		wmLabel* m_plblDelayCount;
		wmLabel* m_plblDelayMech;
		wmLabel* m_plblDelayRange;
		wmLabel* m_plblDelayRate;
		wmLabel* m_plblFollowCount;
		wmLabel* m_plblFollowRate;
		wmLabel* m_plblIdentity;
		wmLabel* m_plblOffsetAverage;
		wmLabel* m_plblOffsetRange;
		wmLabel* m_plblPriority1;
		wmLabel* m_plblPriority2;
		wmLabel* m_plblResponseCount;
		wmLabel* m_plblResponseRate;
		wmLabel* m_plblSource;
		wmLabel* m_plblState;
		wmLabel* m_plblSteps;
		wmLabel* m_plblSyncCount;
		wmLabel* m_plblSyncRate;
		wmLabel* m_plblTime;
		wmLabel* m_plblUTC;
		wmLabel* m_plblVariance;
		wmLabel* m_ptitleAccuracy;
		wmLabel* m_ptitleAddress;
		wmLabel* m_ptitleAnnRate;
		wmLabel* m_ptitleAnnouncements;
		wmLabel* m_ptitleClass;
		wmLabel* m_ptitleCount;
		wmLabel* m_ptitleDelay;
		wmLabel* m_ptitleDelayAverage;
		wmLabel* m_ptitleDelayCount;
		wmLabel* m_ptitleDelayMech;
		wmLabel* m_ptitleDelayRange;
		wmLabel* m_ptitleDelayRate;
		wmLabel* m_ptitleDelayResponses;
		wmLabel* m_ptitleFollowCount;
		wmLabel* m_ptitleFollowRate;
		wmLabel* m_ptitleFollowUp;
		wmLabel* m_ptitleIdentity;
		wmLabel* m_ptitleOffsetAverage;
		wmLabel* m_ptitleOffsetRange;
		wmLabel* m_ptitlePam;
		wmLabel* m_ptitlePriority1;
		wmLabel* m_ptitlePriority2;
		wmLabel* m_ptitleResponseCount;
		wmLabel* m_ptitleResponseRate;
		wmLabel* m_ptitleSource;
		wmLabel* m_ptitleState;
		wmLabel* m_ptitleSteps;
		wmLabel* m_ptitleSync;
		wmLabel* m_ptitleSyncCount;
		wmLabel* m_ptitleSyncRate;
		wmLabel* m_ptitleTime;
		wmLabel* m_ptitleUtc;
		wmLabel* m_ptitleVariance;
		wmList* m_plstClocks;
		wmSwitcherPanel* m_pswp;
		wxPanel* m_ppnlAnnouncements;
		wxPanel* m_ppnlFollowUp;
		wxPanel* m_ppnlLocal;
		wxPanel* m_ppnlMaster;
		wxPanel* m_ppnlRequests;
		wxPanel* m_ppnlResponses;
		wxPanel* m_ppnlSlave;
		wxPanel* m_ppnlSync;
		//*)

		/**	@brief Called when there is some audio data to pass to the meter
		*	@param pBuffer const pointer to the timedbuffer containing the audio data
		**/
        void SetAudioData(const timedbuffer* pBuffer);

		/** @brief Called when there is a change to an input session (e.g. samplerate, bit depth, number of channels)
		*	@param aSession session object containing the new session information
		**/
		void InputSession(const session& aSession);

		/** @brief Called when the output channel mapping changes
		*	@param vChannels vector containing the output channels. The size of the vector reflects the number of output channels and each entry represents the particular channel
		**/
		void OutputChannels(const std::vector<char>& vChannels);

	protected:

		//(*Identifiers(ptpPanel)
		static const long ID_M_PLST1;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL12;
		static const long ID_M_PLBL13;
		static const long ID_M_PLBL14;
		static const long ID_M_PLBL30;
		static const long ID_M_PLBL24;
		static const long ID_M_PLBL25;
		static const long ID_M_PLBL26;
		static const long ID_M_PLBL27;
		static const long ID_M_PLBL4;
		static const long ID_M_PLBL15;
		static const long ID_M_PLBL5;
		static const long ID_M_PLBL16;
		static const long ID_M_PLBL7;
		static const long ID_M_PLBL17;
		static const long ID_M_PLBL8;
		static const long ID_M_PLBL18;
		static const long ID_M_PLBL9;
		static const long ID_M_PLBL19;
		static const long ID_M_PLBL6;
		static const long ID_M_PLBL20;
		static const long ID_M_PLBL11;
		static const long ID_M_PLBL21;
		static const long ID_M_PLBL10;
		static const long ID_M_PLBL22;
		static const long ID_PANEL9;
		static const long ID_PANEL1;
		static const long ID_M_PLBL23;
		static const long ID_M_PLBL31;
		static const long ID_M_PLBL32;
		static const long ID_M_PLBL33;
		static const long ID_M_PLBL34;
		static const long ID_PANEL10;
		static const long ID_PANEL2;
		static const long ID_M_PLBL28;
		static const long ID_M_PLBL29;
		static const long ID_M_PLBL36;
		static const long ID_M_PLBL37;
		static const long ID_M_PLBL38;
		static const long ID_PANEL11;
		static const long ID_PANEL3;
		static const long ID_PANEL5;
		static const long ID_M_PLBL40;
		static const long ID_M_PLBL41;
		static const long ID_M_PLBL42;
		static const long ID_M_PLBL43;
		static const long ID_M_PLBL44;
		static const long ID_M_PLBL46;
		static const long ID_M_PLBL47;
		static const long ID_PANEL13;
		static const long ID_PANEL4;
		static const long ID_M_PLBL48;
		static const long ID_M_PLBL49;
		static const long ID_M_PLBL50;
		static const long ID_M_PLBL51;
		static const long ID_M_PLBL52;
		static const long ID_PANEL12;
		static const long ID_PANEL7;
		static const long ID_PANEL6;
		static const long ID_M_PSWP1;
		static const long ID_M_PLBL54;
		static const long ID_M_PLBL55;
		static const long ID_M_PLBL56;
		static const long ID_M_PLBL57;
		static const long ID_M_PLBL61;
		static const long ID_M_PLBL58;
		static const long ID_M_PLBL62;
		static const long ID_M_PLBL59;
		static const long ID_M_PLBL63;
		static const long ID_M_PLBL60;
		static const long ID_M_PLBL64;
		static const long ID_PANEL8;
		//*)

		void OnLeftUp(wxMouseEvent& event);

	private:

		//(*Handlers(ptpPanel)
		void OnlstClocksSelected(wxCommandEvent& event);
		//*)

		void OnTimer(wxTimerEvent& event);

		void OnInfoLeftUp(wxMouseEvent& event);
		void ConnectLeftUp(wxWindow* pParent);

		void ShowClockDetails();
		void ClearClockDetails();
		void ShowTime();
        void OnClockAdded(wxCommandEvent& event);
        void OnClockUpdated(wxCommandEvent& event);
        void OnClockRemoved(wxCommandEvent& event);
        void OnClockTime(wxCommandEvent& event);
        void OnClockMaster(wxCommandEvent& event);
        void OnClockSlave(wxCommandEvent& event);
        void OnClockMessage(wxCommandEvent& event);
        void AddClock(wxString sClock);
        wxString ConvertRate(unsigned char nRate);

        ptpBuilder* m_pBuilder;

        unsigned char m_nDomain;
		wxString m_sSelectedClock;
		std::shared_ptr<const ptpmonkey::PtpV2Clock> m_pClock;
		std::shared_ptr<const ptpmonkey::PtpV2Clock> m_pLocalClock;

        std::map<unsigned char, wxString> m_mAccuracy;
        std::map<unsigned char, wxString> m_mTimeSource;
        std::map<std::int8_t, wxString> m_mRate;

        wxTimer m_timer;
        bool m_bRunning;

        MacDb m_dbMac;

        static const wxColour CLR_MASTER;
        static const wxColour CLR_MASTER_SELECTED;
        static const wxColour CLR_SLAVE;
        static const wxColour CLR_SLAVE_SELECTED;

		DECLARE_EVENT_TABLE()
};

