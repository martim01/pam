#pragma once
#include <wx/event.h>
#include "dlldefine.h"
#include <map>
#include <memory>

typedef std::map<unsigned char, unsigned char*> mExtension_t;

struct rtpFrame
{
    wxString sEndpoint;
    wxString sGroup;
    unsigned long nSSRC = 0;

    timeval timePresentation;
    timeval timeTransmission;
    timeval timeLatency;
    unsigned int nTimestamp = 0;
    int nTimestampDifference = 0;
    
    unsigned long nFrameSize = 0;
    u_int8_t* pBuffer = nullptr;
    u_int8_t nBytesPerSample = 0;
    double dTSDF = 0.0;
    mExtension_t* mExt = nullptr;
};


class PAMBASE_IMPEXPORT RtpFrameEvent : public wxCommandEvent
{

public:

    /** @brief	Constructor
    *   @param commandType should be wxEVT_NI...
    *   @param id the ID of the button list control
    **/
    explicit RtpFrameEvent(std::shared_ptr<const rtpFrame> pFrame);

    RtpFrameEvent();
    /** @brief	Copy Constructor
    *   @param event a wxNIEvent
    **/
    RtpFrameEvent(const RtpFrameEvent& event);

    /** @brief	Destructor
    **/
    virtual ~RtpFrameEvent()=default;

    /** @brief	Creates a copy of the wxNIEvent
    *   @return <i>wxNIEvent</i>
    **/
    virtual wxEvent *Clone() const { return new RtpFrameEvent(*this); }

    std::shared_ptr<const rtpFrame> GetFrame() const;

    //wxDECLARE_DYNAMIC_CLASS(RtpFrameEvent)


protected:

    std::shared_ptr<const rtpFrame> m_pFrame;
};

typedef void (wxEvtHandler::*RtpFrameEventFunction)(RtpFrameEvent&);


#ifdef WXSPAM
DECLARE_EXPORTED_EVENT_TYPE(WXEXPORT, wxEVT_RTP_FRAME, -1)
#else
wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_RTP_FRAME,RtpFrameEvent);
#endif

