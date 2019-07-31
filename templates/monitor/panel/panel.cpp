#include "[CLASS_PREFIX]panel.h"
#include "[CLASS_PREFIX]builder.h"
#include "timedbuffer.h"
#include "session.h"

//(*InternalHeaders([CLASS_PREFIX]Panel)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit([CLASS_PREFIX]Panel)
//*)

wxIMPLEMENT_DYNAMIC_CLASS([CLASS_PREFIX]Panel, pmPanel)

BEGIN_EVENT_TABLE([CLASS_PREFIX]Panel,pmPanel)
	//(*EventTable([CLASS_PREFIX]Panel)
	//*)
END_EVENT_TABLE()


[CLASS_PREFIX]Panel::[CLASS_PREFIX]Panel(wxWindow* parent,[CLASS_PREFIX]Builder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) : pmPanel(),
    m_pBuilder(pBuilder)
{
	//(*Initialize([CLASS_PREFIX]Panel)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	//*)
	
	Connect(wxEVT_LEFT_UP,(wxObjectEventFunction)&[CLASS_PREFIX]Panel::OnLeftUp);

	SetSize(size);
	SetPosition(pos);
}

[CLASS_PREFIX]Panel::~[CLASS_PREFIX]Panel()
{
	//(*Destroy([CLASS_PREFIX]Panel)
	//*)
}

void [CLASS_PREFIX]Panel::SetAudioData(const timedbuffer* pBuffer)
{
	
}

void [CLASS_PREFIX]Panel::InputSession(const session& aSession)
{
	
}

void [CLASS_PREFIX]Panel::OutputChannels(const std::vector<char>& vChannels)
{
	
}


void [CLASS_PREFIX]Panel::OnLeftUp(wxMouseEvent& event)
{
    m_pBuilder->Maximize((GetSize().x <= 600));
}

