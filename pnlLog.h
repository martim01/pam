#ifndef PNLLOG_H
#define PNLLOG_H
#include <vector>

//(*Headers(pnlLog)
#include <wx/panel.h>
#include "wmlist.h"
//*)

class pnlLogControl;
class wmLogEvent;

class pnlLog: public wxPanel
{
	public:

		pnlLog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlLog();

		void SetLogControl(pnlLogControl* pCtrl);

		//(*Declarations(pnlLog)
		wmList* m_plstLog;
		//*)



	protected:

        void OnLog(wmLogEvent& event);
        void Log(const wxString& sLogEntry);
        friend class pnlLogControl;

		//(*Identifiers(pnlLog)
		static const long ID_M_PLST1;
		//*)

		void Home();
		void End();
		void PageUp();
		void PageDown();
		void ScrollLock(bool bLock);
		void Clear();

		size_t GetCurrentPage() const;
		size_t GetPageCount() const;

	private:
	    void ShowLog();

		//(*Handlers(pnlLog)
		//		void OnlclLogColumnEndDrag(wxListEvent& event);
		void OnlstLogSelected(wxCommandEvent& event);
		//*)

        long m_nPosition;
        bool m_bScrollLock;

        struct logPage
        {
            logPage() : nLines(0), nEntries(0){}
            wxString sTime[23];
            wxString sEntry[23];
            unsigned int nId[23];

            unsigned int nEntries;
            unsigned int nLines;
        };
        std::vector<logPage> m_vLogPages;
        size_t m_nPage;

        pnlLogControl* m_pControl;
		DECLARE_EVENT_TABLE()
};

#endif
