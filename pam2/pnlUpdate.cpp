#include "pnlUpdate.h"

//(*InternalHeaders(pnlUpdate)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "settings.h"
#include <wx/dir.h>
#include "folder.xpm"
#include <wx/log.h>

#ifdef __WXMSW__
#include <wx/volume.h>
#endif // __WXMSW__

using namespace std;

//(*IdInit(pnlUpdate)
const long pnlUpdate::ID_M_PLBL3 = wxNewId();
const long pnlUpdate::ID_PANEL4 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlUpdate,wxPanel)
	//(*EventTable(pnlUpdate)
	//*)
END_EVENT_TABLE()

pnlUpdate::pnlUpdate(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size, unsigned long n, const wxString& s)
{
	//(*Initialize(pnlUpdate)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_pLbl3 = new wmLabel(this, ID_M_PLBL3, _("Update"), wxPoint(0,5), wxSize(600,30), 0, _T("ID_M_PLBL3"));
	m_pLbl3->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl3->GetUiRect().SetGradient(0);
	m_pLbl3->SetForegroundColour(wxColour(255,255,255));
	m_pLbl3->SetBackgroundColour(wxColour(0,64,0));
	wxFont m_pLbl3Font(10,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl3->SetFont(m_pLbl3Font);
	m_pnlUSB = new pnlUSB(this, ID_PANEL4, wxPoint(0,40), wxSize(600,440), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
	m_pnlUSB->SetBackgroundColour(wxColour(0,0,0));
	//*)
	SetSize(size);
	SetPosition(pos);

	m_pnlUSB->StartCheck();

    Connect(wxEVT_SHOW, (wxObjectEventFunction)&pnlUpdate::OnShown);

}

pnlUpdate::~pnlUpdate()
{
	//(*Destroy(pnlUpdate)
	//*)
}


void pnlUpdate::OnlstTypeSelected(wxCommandEvent& event)
{
}
void pnlUpdate::OnbtnCheckClick(wxCommandEvent& event)
{
     #ifndef NDEBUG
        #ifdef __WXMSW__
            wxString sUpdate = wxT("C:\\developer\\matt\\pam2\\pamupdatemanager\\bin\\Debug\\pamupdatemanager");
        #else
          wxString sUpdate = wxT("pamupdatemanager");
        #endif
    #else
        #ifdef __WXMSW__
            wxString sUpdate = wxT("pamupdatemanager");
        #else
        wxString sUpdate = wxT("sudo pamupdatemanager");
        #endif // __WXMSW__
    #endif

    wxExecute(sUpdate);
}

void pnlUpdate::OnShown(wxShowEvent& event)
{
    if(Settings::Get().Read(wxT("Update"), wxT("Type"), wxT("Shared")) == wxT("USB"))
    {
        m_pnlUSB->StartCheck();
    }
}
