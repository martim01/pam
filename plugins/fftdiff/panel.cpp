#include "fftdiffpanel.h"
#include "fftdiffbuilder.h"
#include "timedbuffer.h"
#include "session.h"

//(*InternalHeaders(fftdiffPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(fftdiffPanel)
//*)

wxIMPLEMENT_DYNAMIC_CLASS(fftdiffPanel, pmPanel)

BEGIN_EVENT_TABLE(fftdiffPanel,pmPanel)
	//(*EventTable(fftdiffPanel)
	//*)
END_EVENT_TABLE()


fftdiffPanel::fftdiffPanel(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(fftdiffPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	//*)
	
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&fftdiffPanel::OnLeftUp);

	SetSize(size);
	SetPosition(pos);
}

fftdiffPanel::~fftdiffPanel()
{
	//(*Destroy(fftdiffPanel)
	//*)
}

void fftdiffPanel::SetAudioData(const timedbuffer* pBuffer)
{
	
}

void fftdiffPanel::InputSession(const session& aSession)
{
	
}

void fftdiffPanel::OutputChannels(const std::vector<char>& vChannels)
{
	
}


void fftdiffPanel::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}

