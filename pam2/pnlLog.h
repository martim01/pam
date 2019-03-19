#ifndef PNLLOG_H
#define PNLLOG_H
#include <list>
#include "wmlistadv.h"


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

        wmListAdv* m_pLogList;


	protected:

        void OnLog(wmLogEvent& event);
        void Log(wxString sLogEntry);
        friend class pnlLogControl;

		void Home();
		void End();
		void PageUp();
		void PageDown();
		void ScrollLock(bool bLock);
		void Clear();


	private:

        long m_nPosition;

        pnlLogControl* m_pControl;

        bool m_bScrollLock;
		DECLARE_EVENT_TABLE()
};

#endif
