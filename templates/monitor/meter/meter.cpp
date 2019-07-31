#include "[CLASS_PREFIX]meter.h"
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/log.h>
#include "uirect.h"
#include "[CLASS_PREFIX]builder.h"
#include "settings.h"
#include "timedbuffer.h"


using namespace std;

BEGIN_EVENT_TABLE([CLASS_PREFIX]Meter, pmControl)
    EVT_PAINT([CLASS_PREFIX]Meter::OnPaint)
    EVT_SIZE([CLASS_PREFIX]Meter::OnSize)
    EVT_LEFT_UP([CLASS_PREFIX]Meter::OnLeftUp)
END_EVENT_TABLE()

wxIMPLEMENT_DYNAMIC_CLASS([CLASS_PREFIX]Meter, pmControl);



[CLASS_PREFIX]Meter::[CLASS_PREFIX]Meter(wxWindow *parent, [CLASS_PREFIX]Builder* pBuilder, wxWindowID id, const wxPoint& pos, const wxSize& size) : pmControl(),
    m_pBuilder(pBuilder)
{
    Create(parent, id, pos, size);
   

}

bool [CLASS_PREFIX]Meter::Create(wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
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

[CLASS_PREFIX]Meter::~[CLASS_PREFIX]Meter()
{

}


void [CLASS_PREFIX]Meter::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxBrush br(*wxBLACK);
    dc.SetBrush(br);
    dc.DrawRectangle(GetClientRect());

}


void [CLASS_PREFIX]Meter::OnSize(wxSizeEvent& event)
{

    Refresh();

}

void [CLASS_PREFIX]Meter::SetAudioData(const timedbuffer* pBuffer)
{
	
}

void [CLASS_PREFIX]Meter::InputSession(const session& aSession)
{
	
}
	
void [CLASS_PREFIX]Meter::OutputChannels(const std::vector<char>& vChannels)
{
	
}



void [CLASS_PREFIX]Meter::OnLeftUp(wxMouseEvent& event)
{
	m_pBuilder->Maximize((GetSize().x<=600));

}
