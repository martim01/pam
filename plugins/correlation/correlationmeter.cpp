#include "correlationmeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "uirect.h"
#include "correlationbuilder.h"
#include "settings.h"
#include "timedbuffer.h"


using namespace std;

BEGIN_EVENT_TABLE(correlationMeter, pmControl)
    EVT_PAINT(correlationMeter::OnPaint)
    EVT_SIZE(correlationMeter::OnSize)
    EVT_LEFT_UP(correlationMeter::OnLeftUp)
END_EVENT_TABLE()

wxIMPLEMENT_DYNAMIC_CLASS(correlationMeter, pmControl);



correlationMeter::correlationMeter(wxWindow *parent, correlationBuilder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_pBuilder(pBuilder)
{
    Create(parent, id, pos, size);
   

}

bool correlationMeter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
{
    wxSize szInit(size);
    wxSize bestSize = DoGetBestSize();
    if(size.x<=0)
        szInit.SetWidth(bestSize.x);
    if(size.y <= 0)
        szInit.SetHeight(bestSize.y);

    if(!wxWindow::Create(parent,id,pos,szInit,wxWANTS_CHARS, wxT("fft")))
        return false;

    SetMinSize(size);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);


    return true;
}

correlationMeter::~correlationMeter()
{

}


void correlationMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());

}


void correlationMeter::OnSize(wxSizeEvent& event)
{

    Refresh();

}

void correlationMeter::SetAudioData(const timedbuffer* pBuffer)
{
	
}

void correlationMeter::InputSession(const session& aSession)
{
	
}
	
void correlationMeter::OutputChannels(const std::vector<char>& vChannels)
{
	
}



void correlationMeter::OnLeftUp(wxMouseEvent& event)
{
	m_pBuilder->Maximize((GetSize().x<=600));

}
