#include "pnlwindow.h"

//(*InternalHeaders(pnlWindow)
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include "fftdiffbuilder.h"

//(*IdInit(pnlWindow)
const long pnlWindow::ID_M_PLST5 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlWindow,wxPanel)
	//(*EventTable(pnlWindow)
	//*)
END_EVENT_TABLE()

pnlWindow::pnlWindow(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));
	m_plstFFT_Window = new wmList(this, ID_M_PLST5, wxPoint(0,0), wxSize(190,140), wmList::STYLE_SELECT, 0, wxSize(-1,-1), 2, wxSize(5,5));
	m_plstFFT_Window->SetBackgroundColour(wxColour(0,0,0));


    m_plstFFT_Window->AddButton(wxT("None"));
    m_plstFFT_Window->AddButton(wxT("Hann"));
    m_plstFFT_Window->AddButton(wxT("Hamming"));
    m_plstFFT_Window->AddButton(wxT("Blackman"));
    m_plstFFT_Window->AddButton(wxT("Kaiser"));
    m_plstFFT_Window->AddButton(wxT("KaiserBessel"));
    m_plstFFT_Window->ConnectToSetting(m_pBuilder->GetSection(), "Window", size_t(4));

}

pnlWindow::~pnlWindow()
{
	//(*Destroy(pnlWindow)
	//*)
}


