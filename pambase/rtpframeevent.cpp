#include "rtpframeevent.h"


#ifdef WXSPAM
DEFINE_EVENT_TYPE(wxEVT_RTP_FRAME)
#else
wxDEFINE_EVENT(wxEVT_RTP_FRAME, RtpFrameEvent);
#endif // WXSPAM

//wxDECLARE_DYNAMIC_CLASS(RtpFrameEvent, wxCommandEvent)

RtpFrameEvent::RtpFrameEvent() : wxCommandEvent(wxEVT_RTP_FRAME, wxID_ANY)
{

}

RtpFrameEvent::RtpFrameEvent(std::shared_ptr<const rtpFrame> pFrame) : wxCommandEvent(wxEVT_RTP_FRAME,wxID_ANY),
m_pFrame(pFrame)
{

}


RtpFrameEvent::RtpFrameEvent(const RtpFrameEvent& event) : wxCommandEvent(event),
	m_pFrame(event.GetFrame())
{
}
std::shared_ptr<const rtpFrame> RtpFrameEvent::GetFrame() const
{
    return m_pFrame;
}