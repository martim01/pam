#ifndef PNLLOG_H
#define PNLLOG_H
#include <vector>
#include "wxtouchscreenhtml.h"

//(*Headers(pnlLog)
#include <wx/panel.h>
#include <wx/html/htmlwin.h>
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
		wxTouchScreenHtml* m_phtmlLog;
		//wxHtmlWindow* m_phtmlLog;
		//*)



	protected:

        void OnLog(wmLogEvent& event);
        void Log(wxString sLogEntry);
        friend class pnlLogControl;

		//(*Identifiers(pnlLog)
		static const long ID_HTMLWINDOW1;
		//*)

		void Home();
		void End();
		void PageUp();
		void PageDown();
		void ScrollLock(bool bLock);
		void Clear();


	private:
	    void ShowLog();

		//(*Handlers(pnlLog)
		//		void OnlclLogColumnEndDrag(wxListEvent& event);
		//*)

        long m_nPosition;

        wxString m_sTableStart;
        wxString m_sTableMiddle;
        wxString m_sTableEnd;
        pnlLogControl* m_pControl;

        bool m_bScrollLock;
		DECLARE_EVENT_TABLE()
};

#endif
