#include "bobpanel.h"
#include "bobbuilder.h"
#include "timedbuffer.h"
#include "session.h"

//(*InternalHeaders(bobPanel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(bobPanel)
//*)

wxIMPLEMENT_DYNAMIC_CLASS(bobPanel, pmPanel)

BEGIN_EVENT_TABLE(bobPanel,pmPanel)
	//(*EventTable(bobPanel)
	//*)
END_EVENT_TABLE()


bobPanel::bobPanel(wxWindow* parent,bobBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize(bobPanel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	//*)
	
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&bobPanel::OnLeftUp);

	SetSize(size);
	SetPosition(pos);
}

bobPanel::~bobPanel()
{
	//(*Destroy(bobPanel)
	//*)
}

void bobPanel::SetAudioData(const timedbuffer* pBuffer)
{
	
}

void bobPanel::InputSession(const session& aSession)
{
	
}

void bobPanel::OutputChannels(const std::vector<char>& vChannels)
{
	
}


void bobPanel::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}

