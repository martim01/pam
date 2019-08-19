#include "wmipeditpnl.h"
#include <wx/tokenzr.h>

//(*InternalHeaders(wmipeditpnl)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(wmipeditpnl)
const long wmipeditpnl::ID_M_PEDT1 = wxNewId();
const long wmipeditpnl::ID_M_PEDT2 = wxNewId();
const long wmipeditpnl::ID_M_PEDT3 = wxNewId();
const long wmipeditpnl::ID_M_PEDT4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wmipeditpnl,pmPanel)
	//(*EventTable(wmipeditpnl)
	//*)
END_EVENT_TABLE()

#ifdef WXSPAM
IMPLEMENT_DYNAMIC_CLASS(wmipeditpnl, pmControl)
#else
wxIMPLEMENT_DYNAMIC_CLASS(wmipeditpnl, pmPanel);
#endif // WXSPAM

wmipeditpnl::wmipeditpnl() : pmPanel(),
    m_pEdt1(0),
    m_pEdt2(0),
    m_pEdt3(0),
    m_pEdt4(0)
{
}

wmipeditpnl::wmipeditpnl(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, unsigned long nStyle, const wxString& sEmpty) : pmPanel(),
    m_pEdt1(0),
    m_pEdt2(0),
    m_pEdt3(0),
    m_pEdt4(0)
{
	//(*Initialize(wmipeditpnl)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetForegroundColour(wxColour(0,0,0));
	SetBackgroundColour(wxColour(255,255,255));
	m_pEdt1 = new wmEdit(this, ID_M_PEDT1, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pEdt1->SetMaxLength(3);
	m_pEdt1->SetValidation(5);
	m_pEdt1->SetBorderStyle(2,3);
	m_pEdt2 = new wmEdit(this, ID_M_PEDT2, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_M_PEDT2"));
	m_pEdt2->SetMaxLength(3);
	m_pEdt2->SetValidation(5);
	m_pEdt2->SetBorderStyle(2,3);
	m_pEdt3 = new wmEdit(this, ID_M_PEDT3, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_M_PEDT3"));
	m_pEdt3->SetMaxLength(3);
	m_pEdt3->SetValidation(5);
	m_pEdt3->SetBorderStyle(2,3);
	m_pEdt4 = new wmEdit(this, ID_M_PEDT4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_M_PEDT4"));
	m_pEdt4->SetMaxLength(3);
	m_pEdt4->SetValidation(5);
	m_pEdt4->SetBorderStyle(2,3);

	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt1Text);
	Connect(ID_M_PEDT2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt2Text);
	Connect(ID_M_PEDT3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt3Text);
	Connect(ID_M_PEDT4,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt4Text);
	Connect(wxEVT_SET_FOCUS,(wxObjectEventFunction)&wmipeditpnl::OnSetFocus);
	Connect(wxEVT_SIZE,(wxObjectEventFunction)&wmipeditpnl::OnResize);
	//*)

	Connect(ID_M_PEDT1,wxEVT_TEXT_BACKSPACE,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt1TextBack);
	Connect(ID_M_PEDT2,wxEVT_TEXT_BACKSPACE,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt2TextBack);
	Connect(ID_M_PEDT3,wxEVT_TEXT_BACKSPACE,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt3TextBack);
	Connect(ID_M_PEDT4,wxEVT_TEXT_BACKSPACE,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt4TextBack);

	Connect(ID_M_PEDT1,wxEVT_TEXT_TAB,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt1TextTab);
	Connect(ID_M_PEDT2,wxEVT_TEXT_TAB,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt2TextTab);
	Connect(ID_M_PEDT3,wxEVT_TEXT_TAB,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt3TextTab);
	Connect(ID_M_PEDT4,wxEVT_TEXT_TAB,(wxObjectEventFunction)&wmipeditpnl::Onm_pEdt4TextTab);

	m_pEdt1->SetFocusedBackground(wxColour(255,255,100));
	m_pEdt2->SetFocusedBackground(wxColour(255,255,100));
	m_pEdt3->SetFocusedBackground(wxColour(255,255,100));
	m_pEdt4->SetFocusedBackground(wxColour(255,255,100));


	m_pEdt1->SetValidation(wmEdit::IP);
	m_pEdt2->SetValidation(wmEdit::IP);
	m_pEdt3->SetValidation(wmEdit::IP);
	m_pEdt4->SetValidation(wmEdit::IP);


	SetSize(size);
	SetPosition(pos);

	SizeRects();

}

wmipeditpnl::~wmipeditpnl()
{
	//(*Destroy(wmipeditpnl)
	//*)
}


void wmipeditpnl::Onm_pEdt1Text(wxCommandEvent& event)
{
    CheckEdit(m_pEdt1, m_pEdt2);
}

void wmipeditpnl::Onm_pEdt2Text(wxCommandEvent& event)
{
    CheckEdit(m_pEdt2, m_pEdt3);
}

void wmipeditpnl::Onm_pEdt3Text(wxCommandEvent& event)
{
    CheckEdit(m_pEdt3, m_pEdt4);
}

void wmipeditpnl::Onm_pEdt4Text(wxCommandEvent& event)
{
    CheckEdit(m_pEdt4, NULL);
}


void wmipeditpnl::CheckEdit(wmEdit* pCurrent, wmEdit* pNext)
{
    if(pCurrent->GetValue().Find(wxT('.')) != wxNOT_FOUND)
    {
        pCurrent->SetValue(pCurrent->GetValue().BeforeFirst(wxT('.')));
        if(pNext)
        {
            pNext->SetFocus();
            pNext->SetInsertPos(0);
        }
    }
    else
    {
        unsigned long nValue;
        pCurrent->GetValue().ToULong(&nValue);
        if(nValue > 255)
        {
            pCurrent->SetValue(wxEmptyString);
        }

        if(pCurrent->GetInsertPos() == 3 && pNext)
        {
            pNext->SetFocus();
            pNext->SetInsertPos(0);
        }
    }
}

void wmipeditpnl::OnSetFocus(wxFocusEvent& event)
{
    m_pEdt1->SetFocus();
}


wxString wmipeditpnl::GetValue()
{
    unsigned long nValue;
    if(m_pEdt1->GetValue().ToULong(&nValue) && nValue < 255 &&
       m_pEdt2->GetValue().ToULong(&nValue) && nValue < 255 &&
       m_pEdt3->GetValue().ToULong(&nValue) && nValue < 255 &&
       m_pEdt4->GetValue().ToULong(&nValue) && nValue < 255)
    {
        return wxString::Format(wxT("%s.%s.%s.%s"), m_pEdt1->GetValue().c_str(), m_pEdt2->GetValue().c_str(),m_pEdt3->GetValue().c_str(),m_pEdt4->GetValue().c_str());
    }
    else
    {
        return wxEmptyString;
    }
}

void wmipeditpnl::SetValue(const wxString& sValue)
{
    wxArrayString as(wxStringTokenize(sValue, wxT(".")));
    if(as.GetCount() != 4)
    {
        m_pEdt1->SetValue(wxEmptyString);
        m_pEdt2->SetValue(wxEmptyString);
        m_pEdt3->SetValue(wxEmptyString);
        m_pEdt4->SetValue(wxEmptyString);
    }
    else
    {
        unsigned long nValue[4] = {0,0,0,0};
        for(int i = 0; i < 4; i++)
        {
            as[i].ToULong(&nValue[i]);
            if(nValue[i] > 255)
            {
                nValue[i] = 255;
            }
        }
        m_pEdt1->SetValue(wxString::Format(wxT("%d"), nValue[0]));
        m_pEdt2->SetValue(wxString::Format(wxT("%d"), nValue[1]));
        m_pEdt3->SetValue(wxString::Format(wxT("%d"), nValue[2]));
        m_pEdt4->SetValue(wxString::Format(wxT("%d"), nValue[3]));
    }
}


void wmipeditpnl::Onm_pEdt1TextBack(wxCommandEvent& event)
{

}

void wmipeditpnl::Onm_pEdt2TextBack(wxCommandEvent& event)
{
    m_pEdt1->SetFocus();
    m_pEdt1->DeleteChar();
}

void wmipeditpnl::Onm_pEdt3TextBack(wxCommandEvent& event)
{
    m_pEdt2->SetFocus();
    m_pEdt2->DeleteChar();
}

void wmipeditpnl::Onm_pEdt4TextBack(wxCommandEvent& event)
{
    m_pEdt3->SetFocus();
    m_pEdt3->DeleteChar();
}


void wmipeditpnl::Onm_pEdt1TextTab(wxCommandEvent& event)
{
    if(event.GetInt() == 0)
    {
        m_pEdt2->SetFocus();
        m_pEdt2->SetInsertPos(0);
    }
}

void wmipeditpnl::Onm_pEdt2TextTab(wxCommandEvent& event)
{
    if(event.GetInt() == 0)
    {
        m_pEdt3->SetFocus();
        m_pEdt3->SetInsertPos(0);
    }
    else
    {
        m_pEdt1->SetFocus();
        m_pEdt1->SetInsertPos(0);
    }

}

void wmipeditpnl::Onm_pEdt3TextTab(wxCommandEvent& event)
{
    if(event.GetInt() == 0)
    {
        m_pEdt4->SetFocus();
        m_pEdt4->SetInsertPos(0);
    }
    else
    {if(event.GetInt() == 0)
    {
        m_pEdt2->SetFocus();
        m_pEdt2->SetInsertPos(0);
    }
    else
    {
        m_pEdt1->SetFocus();
        m_pEdt1->SetInsertPos(0);
    }
        m_pEdt2->SetFocus();
        m_pEdt2->SetInsertPos(0);
    }
}

void wmipeditpnl::Onm_pEdt4TextTab(wxCommandEvent& event)
{
    if(event.GetInt() != 0)
    {
        m_pEdt3->SetFocus();
        m_pEdt3->SetInsertPos(0);
    }
}

void wmipeditpnl::OnResize(wxSizeEvent& event)
{
    SizeRects();
}

void wmipeditpnl::SizeRects()
{
    unsigned int nWidth = GetClientRect().GetWidth()/4;
    m_pEdt1->SetSize(wxSize(nWidth, GetClientRect().GetHeight()));
    m_pEdt1->SetPosition(wxPoint(0,0));

    m_pEdt2->SetSize(wxSize(nWidth, GetClientRect().GetHeight()));
    m_pEdt2->SetPosition(wxPoint(nWidth,0));

    m_pEdt3->SetSize(wxSize(nWidth, GetClientRect().GetHeight()));
    m_pEdt3->SetPosition(wxPoint(nWidth*2,0));

    m_pEdt4->SetSize(wxSize(nWidth, GetClientRect().GetHeight()));
    m_pEdt4->SetPosition(wxPoint(nWidth*3,0));

}

bool wmipeditpnl::SetBackgroundColour(const wxColour& colour)
{
    wxPanel::SetBackgroundColour(colour);
    if(m_pEdt1)
    {
        m_pEdt1->SetBackgroundColour(GetBackgroundColour());
        m_pEdt2->SetBackgroundColour(GetBackgroundColour());
        m_pEdt3->SetBackgroundColour(GetBackgroundColour());
        m_pEdt4->SetBackgroundColour(GetBackgroundColour());
    }
    return true;
}

bool wmipeditpnl::SetForegroundColour(const wxColour& colour)
{
    wxPanel::SetForegroundColour(colour);
    if(m_pEdt1)
    {
        m_pEdt1->SetForegroundColour(GetForegroundColour());
        m_pEdt2->SetForegroundColour(GetForegroundColour());
        m_pEdt3->SetForegroundColour(GetForegroundColour());
        m_pEdt4->SetForegroundColour(GetForegroundColour());
    }
    return true;
}

bool wmipeditpnl::Enable(bool bEnable)
{
    if(m_pEdt1)
    {
        m_pEdt1->Enable(bEnable);
        m_pEdt2->Enable(bEnable);
        m_pEdt3->Enable(bEnable);
        m_pEdt4->Enable(bEnable);
    }
    return true;
}
