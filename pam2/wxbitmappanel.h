#pragma once
#include <wx/custombgwin.h>

class wxBitmapPanel : public wxCustomBackgroundWindow<wxPanel>
{
    public:
        wxBitmapPanel(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, unsigned long nStyle, const wxString& sstr)
        {
            Create(parent, id, pos, size, nStyle, sstr);
        }

};
