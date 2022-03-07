#include "buttonpopup.h"
#include "wmbutton.h"
#include "dlgmask.h"
#include "settingevent.h"
#include "settings.h"


ButtonPopup::ButtonPopup(wxWindow *parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size long style, const wxValidator& validator, const wxString& name) :
    m_pButton(new wmButton(parent, id, label, pos, size, style, validator, name))
{

}

wmButton* ButtonPopup::GetButton()
{
    return m_pButton;
}
