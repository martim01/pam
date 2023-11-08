#ifndef PNLLOG_H
#define PNLLOG_H
#include <list>
#include "wmlistadv.h"


//(*Headers(pnlLog)
#include <wx/panel.h>
#include <wx/html/htmlwin.h>
//*)

class SettingEvent;
class pnlLogControl;

class pnlLog: public wxPanel
{
	public:

		pnlLog(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlLog();

		void SetLogControl(pnlLogControl* pCtrl);

        void Filter(int nFilter);


	protected:

        wmListAdv* m_pLogList;
        void OnLog(wxCommandEvent& event);
        friend class pnlLogControl;

		void Home();
		void End();
		void PageUp();
		void PageDown();
		void ScrollLock(bool bLock);
		void Clear();

		void OnSettingChanged(SettingEvent& event);

	private:

        long m_nPosition;
        int m_nFilter;
        pnlLogControl* m_pControl;

        bool m_bScrollLock;
        size_t m_nLogOutput;
		size_t m_nLogToFile = -1;

        unsigned int m_nLogLevel;
		DECLARE_EVENT_TABLE()
};

#endif
