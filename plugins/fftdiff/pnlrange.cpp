#include "pnlrange.h"
#include "settings.h"
#include "settingevent.h"

//(*InternalHeaders(pnlRange)
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include "fftdiffbuilder.h"


BEGIN_EVENT_TABLE(pnlRange,wxPanel)
	//(*EventTable(pnlRange)
	//*)
END_EVENT_TABLE()

pnlRange::pnlRange(wxWindow* parent,fftdiffBuilder* pBuilder, wxWindowID id,const wxPoint& pos,const wxSize& size) :
    m_pBuilder(pBuilder)
{
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	SetBackgroundColour(wxColour(0,0,0));

    m_plstRange = new wmList(this, wxNewId(), wxPoint(5,0), wxSize(190,168), wmList::STYLE_SELECT, wmList::SCROLL_NONE, wxSize(-1,40), 4);
    m_plstRange->SetBackgroundColour(*wxBLACK);
    for(unsigned int i = 1; i < 17; i++)
    {
        m_plstRange->AddButton(wxString::Format("%u dB", i*5), wxNullBitmap, (void*)(i*10));
    }
    m_plstRange->ConnectToSetting(m_pBuilder->GetSection(), "Range", (void*)40);


}

pnlRange::~pnlRange()
{
	//(*Destroy(pnlRange)
	//*)
}


