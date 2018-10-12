#include "dlgLevels.h"
#include <wx/tokenzr.h>
#include <wx/valtext.h>

//(*InternalHeaders(dlgLevels)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(dlgLevels)
const long dlgLevels::ID_STATICTEXT1 = wxNewId();
const long dlgLevels::ID_TEXTCTRL1 = wxNewId();
const long dlgLevels::ID_STATICTEXT2 = wxNewId();
const long dlgLevels::ID_TEXTCTRL2 = wxNewId();
const long dlgLevels::ID_STATICTEXT3 = wxNewId();
const long dlgLevels::ID_TEXTCTRL3 = wxNewId();
const long dlgLevels::ID_STATICTEXT4 = wxNewId();
const long dlgLevels::ID_TEXTCTRL4 = wxNewId();
const long dlgLevels::ID_STATICTEXT5 = wxNewId();
const long dlgLevels::ID_TEXTCTRL5 = wxNewId();
const long dlgLevels::ID_STATICTEXT6 = wxNewId();
const long dlgLevels::ID_TEXTCTRL7 = wxNewId();
const long dlgLevels::ID_TEXTCTRL6 = wxNewId();
const long dlgLevels::ID_BUTTON1 = wxNewId();
const long dlgLevels::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(dlgLevels,wxDialog)
	//(*EventTable(dlgLevels)
	//*)
END_EVENT_TABLE()

dlgLevels::dlgLevels(wxWindow* parent,const levelDetails& details, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_details(details)
{
 //   wxIntegerValidator<long> valOffset(&m_details.nOffset);
 //   wxIntegerValidator<long> valScaling(&m_details.nScaling);
    wxTextValidator valOffset(wxFILTER_NUMERIC, &m_details.sOffset);
    wxTextValidator valScaling(wxFILTER_NUMERIC, &m_details.sScaling);
    wxTextValidator valOverMod(wxFILTER_NUMERIC, &m_details.sOverMod);

	//(*Initialize(dlgLevels)
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer1;
	wxStaticBoxSizer* StaticBoxSizer1;
	wxFlexGridSizer* FlexGridSizer1;

	Create(parent, id, _("Meter Levels"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxDefaultSize);
	Move(wxDefaultPosition);
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	FlexGridSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Title"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	FlexGridSizer1->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_txtTitle = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(200,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	FlexGridSizer1->Add(m_txtTitle, 1, wxALL|wxEXPAND, 2);
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Units"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	FlexGridSizer1->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_txtUnits = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	FlexGridSizer1->Add(m_txtUnits, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Offset"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	FlexGridSizer1->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_txtOffset = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, valOffset, _T("ID_TEXTCTRL3"));
	FlexGridSizer1->Add(m_txtOffset, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Scaling"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	FlexGridSizer1->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_txtScaling = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, valScaling, _T("ID_TEXTCTRL4"));
	FlexGridSizer1->Add(m_txtScaling, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("Reference"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	FlexGridSizer1->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_txtReference = new wxTextCtrl(this, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	FlexGridSizer1->Add(m_txtReference, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("OverMod"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	FlexGridSizer1->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	m_txtOverMod = new wxTextCtrl(this, ID_TEXTCTRL7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, valOverMod, _T("ID_TEXTCTRL7"));
	FlexGridSizer1->Add(m_txtOverMod, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(FlexGridSizer1, 0, wxALL|wxEXPAND, 2);
	StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Enter One Level (dbFS) Per Line"));
	m_txtLevels = new wxTextCtrl(this, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxSize(200,200), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	StaticBoxSizer1->Add(m_txtLevels, 1, wxALL|wxEXPAND, 2);
	BoxSizer1->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND, 2);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	btnCancel = new wxButton(this, ID_BUTTON1, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer2->Add(btnCancel, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	btnOK = new wxButton(this, ID_BUTTON2, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer2->Add(btnOK, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 2);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgLevels::OnbtnCancelClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&dlgLevels::OnbtnOKClick);
	//*)

    m_txtTitle->SetValue(m_details.sTitle);
    m_txtUnits->SetValue(m_details.sUnits);
    m_txtOffset->SetValue(m_details.sOffset);
    m_txtScaling->SetValue(m_details.sScaling);
    m_txtReference->SetValue(m_details.sReference);
    m_txtOverMod->SetValue(m_details.sOverMod);

    for(size_t i = 0; i < m_details.asLevels.GetCount(); i++)
    {
        m_txtLevels->AppendText(m_details.asLevels[i]+wxT("\n"));
    }
}

dlgLevels::~dlgLevels()
{
	//(*Destroy(dlgLevels)
	//*)
}


void dlgLevels::OnbtnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}

void dlgLevels::OnbtnOKClick(wxCommandEvent& event)
{
    m_details.sTitle = m_txtTitle->GetValue();
    m_details.sUnits = m_txtUnits->GetValue();
    m_details.sOffset = m_txtOffset->GetValue();
    m_details.sScaling = m_txtScaling->GetValue();
    m_details.sReference = m_txtReference->GetValue();
    m_details.sOverMod = m_txtOverMod->GetValue();

    m_details.asLevels = wxStringTokenize(m_txtLevels->GetValue(), wxT("\n"));
    for(size_t i = 0; i < m_details.asLevels.GetCount(); i++)
    {
        double dLevel;
        if(m_details.asLevels[i].ToDouble(&dLevel) == false)
        {
            m_details.asLevels[i] = wxT("0");
        }
    }


    EndModal(wxID_OK);
}
