#ifndef PNLPEAKLOG_H
#define PNLPEAKLOG_H

//(*Headers(pnlPeakLog)
#include "wmbutton.h"
#include "wmlabel.h"
#include "levelgraph.h"
#include <wx/panel.h>
#include "wmlist.h"
//*)

class timedbuffer;
struct session;
class loud;
class PeakLogBuilder;

class pnlPeakLog: public wxPanel
{
	public:

		pnlPeakLog(wxWindow* parent, PeakLogBuilder* pBuilder, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlPeakLog();

		//(*Declarations(pnlPeakLog)
		wmLabel* m_pLbl4;
		wmLabel* m_pLbl3;
		LevelGraph* m_pLevelGraph_Day;
		LevelGraph* m_pLevelGraph_Minute;
		wmButton* m_pbtnClear;
		wmList* m_plstGraphs;
		wmLabel* m_plblTitle;
		wmLabel* m_pLbl1;
		wmButton* m_pbtnType;
		LevelGraph* m_pLevelGraph_Hour;
		wmLabel* m_pLbl2;
		LevelGraph* m_pLevelGraph_Second;
		//*)

        void InputSession(const session& aSession);
        void SetAudioData(const timedbuffer* pBuffer);
		void SetLogType(bool bLUFS);

		void PlotChanged(const wxString& sCSV);

	protected:

		//(*Identifiers(pnlPeakLog)
		static const long ID_M_PLBL37;
		static const long ID_CUSTOM7;
		static const long ID_CUSTOM8;
		static const long ID_CUSTOM9;
		static const long ID_CUSTOM12;
		static const long ID_M_PLBL1;
		static const long ID_M_PLBL2;
		static const long ID_M_PLBL3;
		static const long ID_M_PLBL4;
		static const long ID_M_PLST1;
		static const long ID_M_PBTN2;
		static const long ID_M_PBTN1;
		//*)

	private:

		//(*Handlers(pnlPeakLog)
		void OnlstGraphsSelected(wxCommandEvent& event);
		void OnbtnClearClick(wxCommandEvent& event);
		void OnbtnTypeClick(wxCommandEvent& event);
		//*)

		PeakLogBuilder* m_pBuilder;

        void ClearGraphs();
        void AddLines(LevelGraph* pGraph);
        static const wxString GRAPH_LINES[8];
		static const wxColour COLOUR_LINES[8];

		bool m_bLoudness;
//		loud* m_ploud;
		unsigned int m_nChannels;

		DECLARE_EVENT_TABLE()
};

#endif
