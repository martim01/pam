#include "dlgaddlines.h"

//(*InternalHeaders(dlgAddLines)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgAddLines)
const long dlgAddLines::ID_LISTBOX1 = wxNewId();
const long dlgAddLines::ID_STATICTEXT3 = wxNewId();
const long dlgAddLines::ID_TEXTCTRL1 = wxNewId();
const long dlgAddLines::ID_STATICTEXT1 = wxNewId();
const long dlgAddLines::ID_SPINCTRL3 = wxNewId();
const long dlgAddLines::ID_SPINCTRL4 = wxNewId();
const long dlgAddLines::ID_STATICTEXT2 = wxNewId();
const long dlgAddLines::ID_SPINCTRL5 = wxNewId();
const long dlgAddLines::ID_SPINCTRL6 = wxNewId();
const long dlgAddLines::ID_STATICTEXT4 = wxNewId();
const long dlgAddLines::ID_COLOURPICKERCTRL1 = wxNewId();
const long dlgAddLines::ID_STATICTEXT5 = wxNewId();
const long dlgAddLines::ID_SPINCTRL1 = wxNewId();
const long dlgAddLines::ID_STATICTEXT6 = wxNewId();
const long dlgAddLines::ID_CHOICE1 = wxNewId();
const long dlgAddLines::ID_CHECKBOX1 = wxNewId();
const long dlgAddLines::ID_STATICTEXT8 = wxNewId();
const long dlgAddLines::ID_CHOICE2 = wxNewId();
const long dlgAddLines::ID_STATICTEXT9 = wxNewId();
const long dlgAddLines::ID_CHOICE3 = wxNewId();
const long dlgAddLines::ID_STATICTEXT10 = wxNewId();
const long dlgAddLines::ID_SPINCTRL2 = wxNewId();
const long dlgAddLines::ID_BUTTON1 = wxNewId();
const long dlgAddLines::ID_BUTTON2 = wxNewId();
const long dlgAddLines::ID_BUTTON3 = wxNewId();
const long dlgAddLines::ID_M_PLNS1 = wxNewId();
const long dlgAddLines::ID_BUTTON4 = wxNewId();
const long dlgAddLines::ID_BUTTON5 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgAddLines,wxDialog)
	//(*EventTable(dlgAddLines)
	//*)
END_EVENT_TABLE()

dlgAddLines::dlgAddLines(wxWindow* parent, mLines_t& mLines, wxWindowID id) :
    m_mLines(mLines)
{
	//(*Initialize(dlgAddLines)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer6;
	wxFlexGridSizer* FlexGridSizer2;
	wxFlexGridSizer* FlexGridSizer3;
	wxFlexGridSizer* FlexGridSizer4;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer2;
	wxStaticBoxSizer* StaticBoxSizer3;
	wxStaticBoxSizer* StaticBoxSizer4;
	wxStaticBoxSizer* StaticBoxSizer5;
	wxStaticBoxSizer* StaticBoxSizer6;

	Create(parent, id, _("Add Lines"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("id"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticBoxSizer5 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Lines"));
	m_plbxLines = new wxListBox(this, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
	StaticBoxSizer5->Add(m_plbxLines, 1, wxALL|wxEXPAND, 2);
	BoxSizer2->Add(StaticBoxSizer5, 1, wxALL|wxEXPAND, 2);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, this, _("Line Details"));
	BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Name"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	BoxSizer6->Add(StaticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	m_ptxtName = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer6->Add(m_ptxtName, 1, wxALL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer1->Add(BoxSizer6, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer2 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Percentage Positions"));
	FlexGridSizer2 = new wxFlexGridSizer(0, 3, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Start"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pspcStartX = new wxSpinCtrl(this, ID_SPINCTRL3, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL3"));
	m_pspcStartX->SetValue(_T("0"));
	FlexGridSizer2->Add(m_pspcStartX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pspcStartY = new wxSpinCtrl(this, ID_SPINCTRL4, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL4"));
	m_pspcStartY->SetValue(_T("0"));
	FlexGridSizer2->Add(m_pspcStartY, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("End"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer2->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pspcEndX = new wxSpinCtrl(this, ID_SPINCTRL5, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL5"));
	m_pspcEndX->SetValue(_T("0"));
	FlexGridSizer2->Add(m_pspcEndX, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pspcEndY = new wxSpinCtrl(this, ID_SPINCTRL6, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 0, 100, 0, _T("ID_SPINCTRL6"));
	m_pspcEndY->SetValue(_T("0"));
	FlexGridSizer2->Add(m_pspcEndY, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer2->Add(FlexGridSizer2, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer1->Add(StaticBoxSizer2, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Pen"));
	FlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Colour"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer3->Add(StaticText4, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pcpcColour = new wxColourPickerCtrl(this, ID_COLOURPICKERCTRL1, wxColour(0,0,0), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_COLOURPICKERCTRL1"));
	FlexGridSizer3->Add(m_pcpcColour, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Width"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer3->Add(StaticText5, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pspcWidth = new wxSpinCtrl(this, ID_SPINCTRL1, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 1, 100, 0, _T("ID_SPINCTRL1"));
	m_pspcWidth->SetValue(_T("0"));
	FlexGridSizer3->Add(m_pspcWidth, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Pen"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer3->Add(StaticText6, 0, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	m_pchPen = new wxChoice(this, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	m_pchPen->SetSelection( m_pchPen->Append(_("Solid")) );
	m_pchPen->Append(_("Dot"));
	m_pchPen->Append(_("Long Dash"));
	m_pchPen->Append(_("Short Dash"));
	m_pchPen->Append(_("Dot Dash"));
	FlexGridSizer3->Add(m_pchPen, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	FlexGridSizer3->Add(-1,-1,1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pcbDraw = new wxCheckBox(this, ID_CHECKBOX1, _("Draw"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, wxDefaultValidator, _T("ID_CHECKBOX1"));
	m_pcbDraw->SetValue(true);
	FlexGridSizer3->Add(m_pcbDraw, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer3->Add(FlexGridSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer1->Add(StaticBoxSizer3, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer4 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Arrows"));
	FlexGridSizer4 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Start"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	FlexGridSizer4->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pchStartArrow = new wxChoice(this, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE2"));
	m_pchStartArrow->SetSelection( m_pchStartArrow->Append(_("None")) );
	m_pchStartArrow->Append(_("Arrow"));
	m_pchStartArrow->Append(_("Solid Arrow"));
	m_pchStartArrow->Append(_("Circle"));
	m_pchStartArrow->Append(_("Diamond"));
	FlexGridSizer4->Add(m_pchStartArrow, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("End"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	FlexGridSizer4->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pchEndArrow = new wxChoice(this, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE3"));
	m_pchEndArrow->SetSelection( m_pchEndArrow->Append(_("None")) );
	m_pchEndArrow->Append(_("Arrow"));
	m_pchEndArrow->Append(_("Solid Arrow"));
	m_pchEndArrow->Append(_("Circle"));
	m_pchEndArrow->Append(_("Diamond"));
	FlexGridSizer4->Add(m_pchEndArrow, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("Size"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	FlexGridSizer4->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_pspcArrowSize = new wxSpinCtrl(this, ID_SPINCTRL2, _T("0"), wxDefaultPosition, wxDefaultSize, 0, 1, 100, 0, _T("ID_SPINCTRL2"));
	m_pspcArrowSize->SetValue(_T("0"));
	FlexGridSizer4->Add(m_pspcArrowSize, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer4->Add(FlexGridSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer1->Add(StaticBoxSizer4, 0, wxALL|wxEXPAND, 2);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	btnAdd = new wxButton(this, ID_BUTTON1, _("Add"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer4->Add(btnAdd, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	btnUpdate = new wxButton(this, ID_BUTTON2, _("Update"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	btnUpdate->Disable();
	BoxSizer4->Add(btnUpdate, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	btnDelete = new wxButton(this, ID_BUTTON3, _("Delete"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	btnDelete->Disable();
	BoxSizer4->Add(btnDelete, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	StaticBoxSizer1->Add(BoxSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer2->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	StaticBoxSizer6 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Preview"));

	StaticBoxSizer6->Add(m_pLns, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(StaticBoxSizer6, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	btnSave = new wxButton(this, ID_BUTTON4, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
	BoxSizer3->Add(btnSave, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	btnCancel = new wxButton(this, ID_BUTTON5, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
	BoxSizer3->Add(btnCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer3, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&dlgAddLines::OnlbxLinesSelect);
	Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&dlgAddLines::OntxtNameText);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddLines::OnbtnAddClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddLines::OnbtnUpdateClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddLines::OnbtnDeleteClick);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddLines::OnbtnSaveClick);
	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgAddLines::OnbtnCancelClick);
	//*)

	for(mLines_t::iterator itLine = mLines.begin(); itLine != mLines.end(); ++itLine)
    {
        if(m_pLns->AddPercentageLine(itLine->first, itLine->second))
        {
            m_plbxLines->AppendString(itLine->first);
        }
    }
}

dlgAddLines::~dlgAddLines()
{
	//(*Destroy(dlgAddLines)
	//*)
}


void dlgAddLines::OnbtnAddClick(wxCommandEvent& event)
{
    long nStartX, nEndX, nStartY, nEndY;

    int nPen;
    if(m_pchPen->GetStringSelection() == wxT("Solid"))
    {
        nPen = wxSOLID;
    }
    else if(m_pchPen->GetStringSelection() == wxT("Dot"))
    {
        nPen = wxDOT;
    }
    else if(m_pchPen->GetStringSelection() == wxT("Long Dash"))
    {
        nPen = wxLONG_DASH;
    }
    else if(m_pchPen->GetStringSelection() == wxT("Short Dash"))
    {
        nPen = wxSHORT_DASH;
    }
    else if(m_pchPen->GetStringSelection() == wxT("Dot Dash"))
    {
        nPen = wxDOT_DASH;
    }

    if(m_pLns->AddPercentageLine(m_ptxtName->GetValue(),
                                 wxPoint(m_pspcStartX->GetValue(), m_pspcStartY->GetValue()),
                                 wxPoint(m_pspcEndX->GetValue(), m_pspcEndY->GetValue()),
                                 m_pcpcColour->GetColour(),
                                 m_pspcWidth->GetValue(),
                                 nPen, wmLine::LINESTYLE_FLAT,
                                 DecodeArrow(m_pchStartArrow->GetStringSelection()),
                                 DecodeArrow(m_pchEndArrow->GetStringSelection()),
                                 m_pcbDraw, m_pspcArrowSize->GetValue()))
    {
        m_plbxLines->AppendString(m_ptxtName->GetValue());
    }
}

unsigned long dlgAddLines::DecodeArrow(const wxString& sArrow)
{
    unsigned long nArrow = wmLine::LINEEND_NONE;

    if(sArrow == wxT("None"))
    {
        nArrow = wmLine::LINEEND_NONE;
    }
    else if(sArrow == wxT("Arrow"))
    {
        nArrow = wmLine::LINEEND_ARROW;
    }
    else if(sArrow == wxT("Solid Arrow"))
    {
        nArrow = wmLine::LINEEND_SARROW;
    }
    else if(sArrow == wxT("Circle"))
    {
        nArrow = wmLine::LINEEND_CIRCLE;
    }
    else if(sArrow == wxT("Diamond"))
    {
        nArrow = wmLine::LINEEND_SQUARE;
    }
    return nArrow;
}


void dlgAddLines::OnbtnUpdateClick(wxCommandEvent& event)
{
    long nStartX, nEndX, nStartY, nEndY;

    int nPen;
    if(m_pchPen->GetStringSelection() == wxT("Solid"))
    {
        nPen = wxSOLID;
    }
    else if(m_pchPen->GetStringSelection() == wxT("Dot"))
    {
        nPen = wxDOT;
    }
    else if(m_pchPen->GetStringSelection() == wxT("Long Dash"))
    {
        nPen = wxLONG_DASH;
    }
    else if(m_pchPen->GetStringSelection() == wxT("Short Dash"))
    {
        nPen = wxSHORT_DASH;
    }
    else if(m_pchPen->GetStringSelection() == wxT("Dot Dash"))
    {
        nPen = wxDOT_DASH;
    }

    m_pLns->UpdatePercentageLine(m_ptxtName->GetValue(),
                                 wxPoint(m_pspcStartX->GetValue(), m_pspcStartY->GetValue()),
                                 wxPoint(m_pspcEndX->GetValue(), m_pspcEndY->GetValue()),
                                 m_pcpcColour->GetColour(),
                                 m_pspcWidth->GetValue(),
                                 nPen, wmLine::LINESTYLE_FLAT,
                                 DecodeArrow(m_pchStartArrow->GetStringSelection()),
                                 DecodeArrow(m_pchEndArrow->GetStringSelection()),
                                 m_pcbDraw, m_pspcArrowSize->GetValue());
}

void dlgAddLines::OnbtnDeleteClick(wxCommandEvent& event)
{
    if(m_pLns->DeletePercentageLine(m_ptxtName->GetValue()))
    {
        m_plbxLines->Delete(m_plbxLines->FindString(m_ptxtName->GetValue(), true));
    }
}

void dlgAddLines::OnlbxLinesSelect(wxCommandEvent& event)
{

    m_ptxtName->SetValue(m_plbxLines->GetStringSelection());

    m_pspcStartX->SetValue(m_pLns->GetStartPercentage(m_plbxLines->GetStringSelection()).x);
    m_pspcStartY->SetValue(m_pLns->GetStartPercentage(m_plbxLines->GetStringSelection()).y);

    m_pspcEndX->SetValue(m_pLns->GetEndPercentage(m_plbxLines->GetStringSelection()).x);
    m_pspcEndY->SetValue(m_pLns->GetEndPercentage(m_plbxLines->GetStringSelection()).y);

    m_pcpcColour->SetColour(m_pLns->GetColour(m_plbxLines->GetStringSelection()));
    m_pspcWidth->SetValue(m_pLns->GetWidth(m_plbxLines->GetStringSelection()));

    switch(m_pLns->GetPenStyle(m_plbxLines->GetStringSelection()))
    {
        case wxDOT:
            m_pchPen->SetStringSelection(wxT("Dot"));
            break;
        case wxLONG_DASH:
            m_pchPen->SetStringSelection(wxT("Long Dash"));
            break;
        case wxSHORT_DASH:
            m_pchPen->SetStringSelection(wxT("Short Dash"));
            break;
        case wxDOT_DASH:
            m_pchPen->SetStringSelection(wxT("Dot Dash"));
            break;
        default:
            m_pchPen->SetStringSelection(wxT("Solid"));
    }

    SelectArrow(m_pchStartArrow, m_pLns->GetStartArrow(m_plbxLines->GetStringSelection()));
    SelectArrow(m_pchEndArrow, m_pLns->GetEndArrow(m_plbxLines->GetStringSelection()));

    m_pspcArrowSize->SetValue(m_pLns->GetArrowSize(m_plbxLines->GetStringSelection()));

    btnUpdate->Enable();
    btnDelete->Enable();

}



void dlgAddLines::SelectArrow(wxChoice* pChoice, int nArrow)
{
    switch(nArrow)
    {
        case wmLine::LINEEND_ARROW:
            pChoice->SetStringSelection(wxT("Arrow"));
            break;
        case wmLine::LINEEND_SARROW:
            pChoice->SetStringSelection(wxT("Solid Arrow"));
            break;
        case wmLine::LINEEND_CIRCLE:
            pChoice->SetStringSelection(wxT("Circle"));
            break;
        case wmLine::LINEEND_SQUARE:
            pChoice->SetStringSelection(wxT("Diamond"));
            break;
        default:
            pChoice->SetStringSelection(wxT("None"));
    }
}


void dlgAddLines::OnbtnSaveClick(wxCommandEvent& event)
{
    m_mLines.clear();
    m_mLines.insert(m_pLns->GetLinesBegin(), m_pLns->GetLinesEnd());
    EndModal(wxID_OK);

}

void dlgAddLines::OnbtnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void dlgAddLines::OntxtNameText(wxCommandEvent& event)
{
    if(m_pLns->LineExists(m_ptxtName->GetValue()))
    {
        btnUpdate->Enable();
        btnDelete->Enable();
    }
    else
    {
        btnUpdate->Enable(false);
        btnDelete->Enable(false);
    }
}
