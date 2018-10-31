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

class pam2App : public wxApp
{
    public:
        virtual bool OnInit();

};

#endif // PAM2APP_H
