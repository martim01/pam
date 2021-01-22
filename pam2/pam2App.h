/***************************************************************
 * Name:      pam2App.h
 * Purpose:   Defines Application Class
 * Author:    Matthew Martin (martim01@outlook.com)
 * Created:   2017-11-01
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef PAM2APP_H
#define PAM2APP_H

#include <wx/app.h>
#include <wx/timer.h>

class pam2App : public wxApp
{
    public:
        virtual bool OnInit();

        /**  Captures events allowing modification or filtering before passing to normal application event handlers */
        int FilterEvent(wxEvent& event);
        void CheckHold(bool bCheck)
        {
            m_bCheckHold = bCheck;
        }

        void OnTimerHold(wxTimerEvent& event);
        wxTimer m_timerHold;
        bool m_bReset;
        wxString m_sInput;

        bool m_bCheckHold;
};

#endif // PAM2APP_H
