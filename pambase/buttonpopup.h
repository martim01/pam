#pragma once

class wmButton;

class ButtonPopup  : public wxEvtHandler
{
    public:
        ButtonPopup(wxWindow *parent,
                 wxWindowID id,
                 const wxString& label = wxEmptyString,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0, const wxValidator& validator = wxDefaultValidator, const wxString& name =wxT("button"));

        wmButton* GetButton();

    protected:
        void OnButtonPressed(wxCommandEvent& event);
        wmButton* m_pButton;
};
