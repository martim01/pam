#ifndef PNLLOG_H
#define PNLLOG_H
#include <vector>

//(*Headers(pnlLog)
#include <wx/listctrl.h>
#include <wx/panel.h>
//*)

class pnlLogControl;

class pnlLog: public wxPanel
{
	public:

		pnlLog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlLog();

		void SetLogControl(pnlLogControl* pCtrl);

		//(*Declarations(pnlLog)
		wxListCtrl* m_plclLog;
		//*)

		void Log(const wxString& sLogEntry);

	protected:

        friend class pnlLogControl;

		//(*Identifiers(pnlLog)
		static const long ID_LISTCTRL1;
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
		void OnlclLogColumnEndDrag(wxListEvent& event);
		//*)

        long m_nPosition;
        bool m_bScrollLock;

        struct logPage
        {
            logPage() : nLines(0){}
            wxString sTime[27];
            wxString sEntry[27];

            unsigned int nLines;
        };
        std::vector<logPage> m_vLogPages;
        size_t m_nPage;

        pnlLogControl* m_pControl;
		DECLARE_EVENT_TABLE()
};

#endif
