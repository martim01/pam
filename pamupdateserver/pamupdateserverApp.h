/***************************************************************
 * Name:      pamupdateserverApp.h
 * Purpose:   Defines Application Class
 * Author:    Matthew Martin (matthew.martin@bbc.co.uk)
 * Created:   2018-06-11
 * Copyright: Matthew Martin ()
 * License:
 **************************************************************/

#ifndef PAMUPDATESERVERAPP_H
#define PAMUPDATESERVERAPP_H

#include <wx/app.h>

class pamupdateserverApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // PAMUPDATESERVERAPP_H
