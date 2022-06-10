#include "fftdiffmeter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "uirect.h"
#include "fftdiffbuilder.h"
#include "settings.h"
#include "timedbuffer.h"


using namespace std;

BEGIN_EVENT_TABLE(fftdiffMeter, pmControl)
    EVT_PAINT(fftdiffMeter::OnPaint)
    EVT_SIZE(fftdiffMeter::OnSize)
    EVT_LEFT_UP(fftdiffMeter::OnLeftUp)
END_EVENT_TABLE()

wxIMPLEMENT_DYNAMIC_CLASS(fftdiffMeter, pmControl);



fftdiffMeter::fftdiffMeter(wxWindow *parent, fftdiffBuilder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_pBuilder(pBuilder)
{
    Create(parent, id, pos, size);
   

}

bool fftdiffMeter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
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

fftdiffMeter::~fftdiffMeter()
{

}


void fftdiffMeter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());

}


void fftdiffMeter::OnSize(wxSizeEvent& event)
{

    Refresh();

}

void fftdiffMeter::SetAudioData(const timedbuffer* pBuffer)
{
	
}

void fftdiffMeter::InputSession(const session& aSession)
{
	
}
	
void fftdiffMeter::OutputChannels(const std::vector<char>& vChannels)
{
	
}



void fftdiffMeter::OnLeftUp(wxMouseEvent& event)
{
	m_pBuilder->Maximize((GetSize().x<=600));

}
