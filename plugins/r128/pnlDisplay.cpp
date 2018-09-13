#include "pnlDisplay.h"

//(*InternalHeaders(pnlDisplay)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "r128builder.h"

//(*IdInit(pnlDisplay)
const long pnlDisplay::ID_M_PLBL1 = wxNewId();
const long pnlDisplay::ID_M_PLST1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlDisplay,wxPanel)
	//(*EventTable(pnlDisplay)
	//*)
END_EVENT_TABLE()

pnlDisplay::pnlDisplay(wxWindow* parent,R128Builder* pBuilder,wxWindowID id,const wxPoint& pos,const wxSize& size) : m_pBuilder(pBuilder)
{
	//(*Initialize(pnlDisplay)
	Create(parent, id, wxDefaultPosition, wxSize(200,30), wxTAB_TRAVERSAL, _T("id"));
	SetForegroundColour(wxColour(255,255,255));
	SetBackgroundColour(wxColour(0,0,0));
	wxFont thisFont(11,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Tahoma"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_pLbl1 = new wmLabel(this, ID_M_PLBL1, _("Meters To Show"), wxPoint(0,0), wxSize(200,30), 0, _T("ID_M_PLBL1"));
	m_pLbl1->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl1->SetBackgroundColour(wxColour(0,64,0));
	m_plstShow = new wmList(this, ID_M_PLST1, wxPoint(0,30), wxSize(200,200), wmList::STYLE_SELECT|wmList::STYLE_SELECT_MULTI, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstShow->SetBackgroundColour(wxColour(0,0,0));
	m_plstShow->SetPressedButtonColour(wxColour(wxT("#00FF00")));
	m_plstShow->SetSelectedButtonColour(wxColour(wxT("#009300")));

	Connect(ID_M_PLST1,wxEVT_LIST_SELECTED,(wxObjectEventFunction)&pnlDisplay::OnlstShowSelected);
	//*)
	m_plstShow->AddButton(wxT("Momentary"));
	m_plstShow->AddButton(wxT("Short"));
	m_plstShow->AddButton(wxT("Integrated"));
	m_plstShow->AddButton(wxT("True Peak"));
	m_plstShow->AddButton(wxT("Phase"));



	if(m_pBuilder->ReadSetting(wxT("Show_Short"), wxT("1")) == wxT("1"))
    {
        m_plstShow->SelectButton(1, true);
    }
    if(m_pBuilder->ReadSetting(wxT("Show_Momentary"), wxT("1")) == wxT("1"))
    {
        m_plstShow->SelectButton(0, true);
    }
    if(m_pBuilder->ReadSetting(wxT("Show_Live"), wxT("1")) == wxT("1"))
    {
        m_plstShow->SelectButton(2, true);
    }
    if(m_pBuilder->ReadSetting(wxT("Show_True"), wxT("1")) == wxT("1"))
    {
        m_plstShow->SelectButton(3, true);
    }
    if(m_pBuilder->ReadSetting(wxT("Show_Phase"), wxT("1")) == wxT("1"))
    {
        m_plstShow->SelectButton(4, true);
    }
}

pnlDisplay::~pnlDisplay()
{
	//(*Destroy(pnlDisplay)
	//*)
}


void pnlDisplay::OnlstShowSelected(wxCommandEvent& event)
{
    switch(event.GetInt())
    {
    case 0:
        m_pBuilder->WriteSetting(wxT("Show_Momentary"), wxString::Format(wxT("%d"), event.GetExtraLong()));
        break;
    case 1:
        m_pBuilder->WriteSetting(wxT("Show_Short"), wxString::Format(wxT("%d"), event.GetExtraLong()));
        break;

    case 2:
        m_pBuilder->WriteSetting(wxT("Show_Live"), wxString::Format(wxT("%d"), event.GetExtraLong()));
        break;
    case 3:
        m_pBuilder->WriteSetting(wxT("Show_True"), wxString::Format(wxT("%d"), event.GetExtraLong()));
        break;
    case 4:
        m_pBuilder->WriteSetting(wxT("Show_Phase"), wxString::Format(wxT("%d"), event.GetExtraLong()));
        break;
    }
}
