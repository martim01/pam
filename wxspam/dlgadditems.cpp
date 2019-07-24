#include "dlgadditems.h"
#include <wx/tokenzr.h>

using namespace std;

//(*InternalHeaders(dlgAddItems)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgAddItems)
const long dlgAddItems::ID_M_PLST1 = wxNewId();
const long dlgAddItems::ID_STATICTEXT1 = wxNewId();
const long dlgAddItems::ID_TEXTCTRL1 = wxNewId();
const long dlgAddItems::ID_STATICTEXT2 = wxNewId();
const long dlgAddItems::ID_CHOICE1 = wxNewId();
const long dlgAddItems::ID_CHECKBOX2 = wxNewId();
const long dlgAddItems::ID_COLOURPICKERCTRL1 = wxNewId();
const long dlgAddItems::ID_CHECKBOX3 = wxNewId();
const long dlgAddItems::ID_COLOURPICKERCTRL2 = wxNewId();
const long dlgAddItems::ID_CHECKBOX4 = wxNewId();
const long dlgAddItems::ID_COLOURPICKERCTRL3 = wxNewId();
const long dlgAddItems::ID_CHECKBOX5 = wxNewId();
const long dlgAddItems::ID_COLOURPICKERCTRL4 = wxNewId();
const long dlgAddItems::ID_CHECKBOX6 = wxNewId();
const long dlgAddItems::ID_COLOURPICKERCTRL5 = wxNewId();
const long dlgAddItems::ID_CHECKBOX7 = wxNewId();
const long dlgAddItems::ID_COLOURPICKERCTRL6 = wxNewId();
const long dlgAddItems::ID_STATICTEXT3 = wxNewId();
const long dlgAddItems::ID_CHOICE2 = wxNewId();
const long dlgAddItems::ID_STATICTEXT4 = wxNewId();
const long dlgAddItems::ID_TEXTCTRL2 = wxNewId();
const long dlgAddItems::ID_STATICTEXT5 = wxNewId();
const long dlgAddItems::ID_CHOICE3 = wxNewId();
const long dlgAddItems::ID_BUTTON1 = wxNewId();
const long dlgAddItems::ID_BUTTON2 = wxNewId();
const long dlgAddItems::ID_BUTTON3 = wxNewId();
const long dlgAddItems::ID_BUTTON4 = wxNewId();
const long dlgAddItems::ID_BUTTON5 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgAddItems,wxDialog)
	//(*EventTable(dlgAddItems)
	//*)
END_EVENT_TABLE()

dlgAddItems::dlgAddItems(wxWindow* parent,const lItems_t& lItems, const wxString& sDefaultColours,wxWindowID id,const wxPoint& pos,const wxSize& size) : m_lstItems(lItems)
{
	//(*Initialize(dlgAddItems)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer5;
	wxBoxSizer* BoxSizer6;
	wxFlexGridSizer* FlexGridSizer1;
	wxFlexGridSizer* FlexGridSizer2;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;

	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Items Preview"));

	StaticBoxSizer1->Add(m_pLst, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer2->Add(StaticBoxSizer1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, this, _("Item Details"));
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Text"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer3->Add(StaticText1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_ptxtText = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer3->Add(m_ptxtText, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer3, 0, wxALL|wxEXPAND, 2);
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("State"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	BoxSizer6->Add(StaticText2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pchState = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	m_pchState->SetSelection( m_pchState->Append(_("Enabled")) );
	m_pchState->Append(_("Disabled"));
	m_pchState->Append(_("Hidden"));
	BoxSizer6->Add(m_pchState, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer6, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxVERTICAL, this, _("Colours"));
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	m_pchbxButton = new wxCheckBox(this, ID_CHECKBOX2, _("Button"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	m_pchbxButton->SetValue(false);
	FlexGridSizer1->Add(m_pchbxButton, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pcpcNormal = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL1, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL1"));
	m_pcpcNormal->Disable();
	FlexGridSizer1->Add(m_pcpcNormal, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pchbxButtonPressed = new wxCheckBox(this, ID_CHECKBOX3, _("Button: Pressed"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
	m_pchbxButtonPressed->SetValue(false);
	FlexGridSizer1->Add(m_pchbxButtonPressed, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pcpcPressed = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL2, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL2"));
	m_pcpcPressed->Disable();
	FlexGridSizer1->Add(m_pcpcPressed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pchbxButtonSelected = new wxCheckBox(this, ID_CHECKBOX4, _("Button: Selected"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
	m_pchbxButtonSelected->SetValue(false);
	FlexGridSizer1->Add(m_pchbxButtonSelected, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pcpcSelected = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL3, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL3"));
	m_pcpcSelected->Disable();
	FlexGridSizer1->Add(m_pcpcSelected, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pchbxText = new wxCheckBox(this, ID_CHECKBOX5, _("Text"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
	m_pchbxText->SetValue(false);
	FlexGridSizer1->Add(m_pchbxText, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pcpcTextNormal = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL4, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL4"));
	m_pcpcTextNormal->Disable();
	FlexGridSizer1->Add(m_pcpcTextNormal, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pchbxTextPressed = new wxCheckBox(this, ID_CHECKBOX6, _("Text: Pressed"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
	m_pchbxTextPressed->SetValue(false);
	FlexGridSizer1->Add(m_pchbxTextPressed, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pcpcTextPressed = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL5, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL5"));
	m_pcpcTextPressed->Disable();
	FlexGridSizer1->Add(m_pcpcTextPressed, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pchbxTextSelected = new wxCheckBox(this, ID_CHECKBOX7, _("Text: Selected"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
	m_pchbxTextSelected->SetValue(false);
	FlexGridSizer1->Add(m_pchbxTextSelected, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pcpcTextSelected = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL6, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL6"));
	m_pcpcTextSelected->Disable();
	FlexGridSizer1->Add(m_pcpcTextSelected, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer3->Add(FlexGridSizer1, 1, wxALL|wxEXPAND, 0);
	StaticBoxSizer2->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Sliding"));
	FlexGridSizer2 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Direction"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer2->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pchDirection = new wxChoice(this, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
	m_pchDirection->SetSelection( m_pchDirection->Append(_("None")) );
	m_pchDirection->Append(_("Up"));
	m_pchDirection->Append(_("Down"));
	m_pchDirection->Append(_("Left"));
	m_pchDirection->Append(_("Right"));
	FlexGridSizer2->Add(m_pchDirection, 1, wxALL|wxEXPAND, 2);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Text"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer2->Add(StaticText4, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_ptxtSlide = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer2->Add(m_ptxtSlide, 1, wxALL|wxEXPAND, 2);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Arrow"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer2->Add(StaticText5, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pchArrow = new wxChoice(this, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
	m_pchArrow->Append(_("Show"));
	m_pchArrow->SetSelection( m_pchArrow->Append(_("Hide")) );
	FlexGridSizer2->Add(m_pchArrow, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer4->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(StaticBoxSizer4, 0, wxALL|wxEXPAND, 2);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	m_pbtnAdd = new wxButton(this, ID_BUTTON1, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer4->Add(m_pbtnAdd, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pbtnUpdate = new wxButton(this, ID_BUTTON2, _("Update"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	m_pbtnUpdate->Disable();
	BoxSizer4->Add(m_pbtnUpdate, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pbtnDelete = new wxButton(this, ID_BUTTON3, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	m_pbtnDelete->Disable();
	BoxSizer4->Add(m_pbtnDelete, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(BoxSizer4, 0, wxALL|wxEXPAND, 2);
	BoxSizer2->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 2);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	m_pbtnSave = new wxButton(this, ID_BUTTON4, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
	BoxSizer5->Add(m_pbtnSave, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pbtnCancel = new wxButton(this, ID_BUTTON5, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
	BoxSizer5->Add(m_pbtnCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer5, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnchbxButtonClick);
	Connect(ID_CHECKBOX3,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnchbxButtonPressedClick);
	Connect(ID_CHECKBOX4,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnchbxButtonSelectedClick);
	Connect(ID_CHECKBOX5,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnchbxTextClick);
	Connect(ID_CHECKBOX6,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnchbxTextPressedClick);
	Connect(ID_CHECKBOX7,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnchbxTextSelectedClick);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnbtnAddClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnbtnUpdateClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnbtnDeleteClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnbtnSaveClick);
	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddItems::OnbtnCancelClick);
	//*)

	m_nSelected = -1;

    //m_ptxtText->SetValue(sDefaultColours);

	wxArrayString as(wxStringTokenize(sDefaultColours, wxT(",")));
	if(as.GetCount() > 0 && as[0] != wxEmptyString)
	{
	    m_pLst->SetButtonColour(wxColour(as[0]));
	}
	if(as.GetCount() > 1 && as[1] != wxEmptyString)
	{
	    m_pLst->SetPressedButtonColour(wxColour(as[1]));
	}
	if(as.GetCount() > 2 && as[2] != wxEmptyString)
	{
        m_pLst->SetSelectedButtonColour(wxColour(as[2]));
	}

	if(as.GetCount() > 3 && as[3] != wxEmptyString)
	{
	    m_pLst->SetTextButtonColour(wxColour(as[3]));
    }
	if(as.GetCount() > 4 && as[4] != wxEmptyString)
	{
        m_pLst->SetTextPressedButtonColour(wxColour(as[4]));
    }
	if(as.GetCount() > 5 && as[5] != wxEmptyString)
	{
        m_pLst->SetTextSelectedButtonColour(wxColour(as[5]));
	}

	for(list<button_wxs>::const_iterator itItem = m_lstItems.begin(); itItem != m_lstItems.end(); ++itItem)
    {
        size_t nIndex = m_pLst->AddButton((*itItem).sText, wxNullBitmap, 0, (*itItem).nEnabled, (*itItem).clrButton, (*itItem).clrPressed, (*itItem).clrSelected);
        m_pLst->SetTextButtonColour(nIndex, (*itItem).clrText);
        m_pLst->SetTextPressedButtonColour(nIndex, (*itItem).clrTextPressed);
        m_pLst->SetTextSelectedButtonColour(nIndex, (*itItem).clrTextSelected);
        m_pLst->SetSlideAllowed(nIndex, (*itItem).nSlideAllowed, (*itItem).sSlide, false);
    }
}

dlgAddItems::~dlgAddItems()
{
	//(*Destroy(dlgAddItems)
	//*)
}


void dlgAddItems::OnLstItemSelected(wxCommandEvent& event)
{
    m_nSelected = event.GetInt();

    m_ptxtText->SetLabel(event.GetString());

    switch(m_pLst->GetEnabled(m_nSelected))
    {
        case wmList::wmENABLED:
            m_pchState->SetStringSelection(wxT("Enabled"));
            break;
        case wmList::wmDISABLED:
            m_pchState->SetStringSelection(wxT("Disabled"));
            break;
        case wmList::wmHIDDEN:
            m_pchState->SetStringSelection(wxT("Hidden"));
            break;
    }

    wxColour clr = m_pLst->GetButtonColour(m_nSelected);
    m_pcpcNormal->SetColour(clr);
    m_pchbxButton->SetValue(clr!=m_pLst->GetBackgroundColour());

    clr = m_pLst->GetButtonPressedColour(m_nSelected);
    m_pcpcPressed->SetColour(clr);
    m_pchbxButtonPressed->SetValue(clr!=m_pLst->GetButtonsPressedColour());

    clr = m_pLst->GetButtonSelectedColour(m_nSelected);
    m_pcpcSelected->SetColour(clr);
    m_pchbxButtonSelected->SetValue(clr!=m_pLst->GetButtonsSelectedColour());

    clr = m_pLst->GetButtonTextColour(m_nSelected);
    m_pcpcNormal->SetColour(clr);
    m_pchbxButton->SetValue(clr!=m_pLst->GetForegroundColour());

    clr = m_pLst->GetButtonTextPressedColour(m_nSelected);
    m_pcpcPressed->SetColour(clr);
    m_pchbxButtonPressed->SetValue(clr!=m_pLst->GetButtonsTextPressedColour());

    clr = m_pLst->GetButtonTextSelectedColour(m_nSelected);
    m_pcpcSelected->SetColour(clr);
    m_pchbxButtonSelected->SetValue(clr!=m_pLst->GetButtonsTextSelectedColour());


    switch(m_pLst->GetSlideDirection(m_nSelected))
    {
        case wmList::SLID_NONE:
            m_pchDirection->SetStringSelection(wxT("None"));
            break;
        case wmList::SLID_LEFT:
            m_pchDirection->SetStringSelection(wxT("Left"));
            break;
        case wmList::SLID_RIGHT:
            m_pchDirection->SetStringSelection(wxT("Right"));
            break;
        case wmList::SLID_UP:
            m_pchDirection->SetStringSelection(wxT("Up"));
            break;
        case wmList::SLID_DOWN:
            m_pchDirection->SetStringSelection(wxT("Down"));
            break;
    }
    m_ptxtSlide->SetValue(m_pLst->GetSlideText(m_nSelected));



    m_pbtnUpdate->Enable();
    m_pbtnDelete->Enable();



}

void dlgAddItems::OnbtnAddClick(wxCommandEvent& event)
{
    wxColour clr[6];
    if(m_pchbxButton->IsChecked())
    {
        clr[0] = m_pcpcNormal->GetColour();
    }
    else
    {
        clr[0] = wxNullColour;
    }

    if(m_pchbxButtonPressed->IsChecked())
    {
        clr[1] = m_pcpcPressed->GetColour();
    }
    else
    {
        clr[1] = wxNullColour;
    }

    if(m_pchbxButtonSelected->IsChecked())
    {
        clr[2] = m_pcpcSelected->GetColour();
    }
    else
    {
        clr[2] = wxNullColour;
    }

    if(m_pchbxText->IsChecked())
    {
        clr[3] = m_pcpcTextNormal->GetColour();
    }
    else
    {
        clr[3] = wxNullColour;
    }

    if(m_pchbxTextPressed->IsChecked())
    {
        clr[4] = m_pcpcTextPressed->GetColour();
    }
    else
    {
        clr[4] = wxNullColour;
    }

    if(m_pchbxTextSelected->IsChecked())
    {
        clr[5] = m_pcpcTextSelected->GetColour();
    }
    else
    {
        clr[5] = wxNullColour;
    }
    unsigned short nState(wmList::wmENABLED);

    if(m_pchState->GetStringSelection() == wxT("Disabled"))
    {
        nState = wmList::wmDISABLED;
    }
    else if(m_pchState->GetStringSelection() == wxT("Hidden"))
    {
        nState = wmList::wmHIDDEN;
    }

    size_t nIndex = m_pLst->AddButton(m_ptxtText->GetValue(), wxNullBitmap, 0, wmList::wmENABLED, clr[0], clr[1], clr[2]);

    if(clr[3].IsOk())
        m_pLst->SetTextButtonColour(nIndex, clr[3]);
    if(clr[4].IsOk())
        m_pLst->SetTextPressedButtonColour(nIndex, clr[4]);
    if(clr[5].IsOk())
        m_pLst->SetTextSelectedButtonColour(nIndex, clr[5]);

    unsigned short nSlide(wmList::SLID_NONE);
    if(m_pchDirection->GetStringSelection() == wxT("Up"))
    {
        nSlide = wmList::SLID_UP;
        m_pLst->SetSlideAllowed(nIndex, wmList::SLID_UP, m_ptxtSlide->GetValue(), false);
    }
    else if(m_pchDirection->GetStringSelection() == wxT("Down"))
    {
        nSlide = wmList::SLID_DOWN;
        m_pLst->SetSlideAllowed(nIndex, wmList::SLID_DOWN, m_ptxtSlide->GetValue(), false);
    }
    else if(m_pchDirection->GetStringSelection() == wxT("Left"))
    {
        nSlide = wmList::SLID_LEFT;
        m_pLst->SetSlideAllowed(nIndex, wmList::SLID_LEFT, m_ptxtSlide->GetValue(), false);
    }
    else if(m_pchDirection->GetStringSelection() == wxT("Right"))
    {
        nSlide = wmList::SLID_RIGHT;
        m_pLst->SetSlideAllowed(nIndex, wmList::SLID_RIGHT, m_ptxtSlide->GetValue(), false);
    }

    button_wxs item;
    item.nIndex = nIndex;
    item.sText = m_ptxtText->GetValue();
    item.clrButton = clr[0];
    item.clrPressed = clr[1];
    item.clrSelected = clr[2];
    item.clrText = clr[3];
    item.clrTextPressed = clr[4];
    item.clrTextSelected = clr[5];

    item.nSlideAllowed = nSlide;
    item.sSlide = m_ptxtSlide->GetValue();
    m_lstItems.push_back(item);

    m_pLst->Refresh();
}

void dlgAddItems::OnbtnUpdateClick(wxCommandEvent& event)
{
    wxColour clr[6];
    if(m_pchbxButton->IsChecked())
    {
        clr[0] = m_pcpcNormal->GetColour();
    }
    else
    {
        clr[0] = wxNullColour;
    }

    if(m_pchbxButtonPressed->IsChecked())
    {
        clr[1] = m_pcpcPressed->GetColour();
    }
    else
    {
        clr[1] = wxNullColour;
    }

    if(m_pchbxButtonSelected->IsChecked())
    {
        clr[2] = m_pcpcSelected->GetColour();
    }
    else
    {
        clr[2] = wxNullColour;
    }

    if(m_pchbxText->IsChecked())
    {
        clr[3] = m_pcpcTextNormal->GetColour();
    }
    else
    {
        clr[3] = wxNullColour;
    }

    if(m_pchbxTextPressed->IsChecked())
    {
        clr[4] = m_pcpcTextPressed->GetColour();
    }
    else
    {
        clr[4] = wxNullColour;
    }

    if(m_pchbxTextSelected->IsChecked())
    {
        clr[5] = m_pcpcTextSelected->GetColour();
    }
    else
    {
        clr[5] = wxNullColour;
    }
    unsigned short nState(wmList::wmENABLED);

    if(m_pchState->GetStringSelection() == wxT("Disabled"))
    {
        nState = wmList::wmDISABLED;
    }
    else if(m_pchState->GetStringSelection() == wxT("Hidden"))
    {
        nState = wmList::wmHIDDEN;
    }

    m_pLst->SetButtonText(m_nSelected, m_ptxtText->GetValue());
    m_pLst->SetButtonColour(m_nSelected, clr[0]);
    m_pLst->SetPressedButtonColour(m_nSelected, clr[1]);
    m_pLst->SetSelectedButtonColour(m_nSelected, clr[1]);

    m_pLst->SetTextButtonColour(m_nSelected, clr[3]);
    m_pLst->SetTextPressedButtonColour(m_nSelected, clr[4]);
    m_pLst->SetTextSelectedButtonColour(m_nSelected, clr[5]);

    unsigned short nSlide(wmList::SLID_NONE);
    if(m_pchDirection->GetStringSelection() == wxT("Up"))
    {
        m_pLst->SetSlideAllowed(m_nSelected, wmList::SLID_UP, m_ptxtSlide->GetValue(), false);
    }
    else if(m_pchDirection->GetStringSelection() == wxT("Down"))
    {
        m_pLst->SetSlideAllowed(m_nSelected, wmList::SLID_DOWN, m_ptxtSlide->GetValue(), false);
    }
    else if(m_pchDirection->GetStringSelection() == wxT("Left"))
    {
        m_pLst->SetSlideAllowed(m_nSelected, wmList::SLID_LEFT, m_ptxtSlide->GetValue(), false);
    }
    else if(m_pchDirection->GetStringSelection() == wxT("Right"))
    {
        m_pLst->SetSlideAllowed(m_nSelected, wmList::SLID_RIGHT, m_ptxtSlide->GetValue(), false);
    }

    for(lItems_t::iterator itItem = m_lstItems.begin(); itItem != m_lstItems.end(); ++itItem)
    {
        if((*itItem).nIndex == m_nSelected)
        {
            (*itItem).sText = m_ptxtText->GetValue();
            (*itItem).clrButton = clr[0];
            (*itItem).clrPressed = clr[1];
            (*itItem).clrSelected = clr[2];
            (*itItem).clrText = clr[3];
            (*itItem).clrTextPressed = clr[4];
            (*itItem).clrTextSelected = clr[5];

            (*itItem).nSlideAllowed = nSlide;
            (*itItem).sSlide = m_ptxtSlide->GetValue();
            break;
        }
    }
    m_pLst->Refresh();
}

void dlgAddItems::OnbtnDeleteClick(wxCommandEvent& event)
{
    for(lItems_t::iterator itItem = m_lstItems.begin(); itItem != m_lstItems.end(); ++itItem)
    {
        if((*itItem).nIndex == m_nSelected)
        {
            m_pLst->DeleteButton(m_nSelected);
            m_nSelected = -1;
            m_lstItems.erase(itItem);
            break;
        }
    }

    m_pbtnDelete->Enable(false);
    m_pbtnUpdate->Enable(false);

    m_pLst->Refresh();
}

void dlgAddItems::OnchbxButtonClick(wxCommandEvent& event)
{
    m_pcpcNormal->Enable(m_pchbxButton->IsChecked());
}

void dlgAddItems::OnchbxButtonPressedClick(wxCommandEvent& event)
{
    m_pcpcPressed->Enable(m_pchbxButtonPressed->IsChecked());
}

void dlgAddItems::OnchbxButtonSelectedClick(wxCommandEvent& event)
{
    m_pcpcSelected->Enable(m_pchbxButtonSelected->IsChecked());
}

void dlgAddItems::OnchbxTextClick(wxCommandEvent& event)
{
    m_pcpcTextNormal->Enable(m_pchbxText->IsChecked());
}

void dlgAddItems::OnchbxTextPressedClick(wxCommandEvent& event)
{
    m_pcpcTextPressed->Enable(m_pchbxTextPressed->IsChecked());
}

void dlgAddItems::OnchbxTextSelectedClick(wxCommandEvent& event)
{
    m_pcpcTextSelected->Enable(m_pchbxTextSelected->IsChecked());
}


void dlgAddItems::OnbtnSaveClick(wxCommandEvent& event)
{

    EndModal(wxID_OK);
}

void dlgAddItems::OnbtnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}
