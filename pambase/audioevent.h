#pragma once

#include <wx/event.h>
#include "dlldefine.h"

class timedbuffer;

class PAMBASE_IMPEXPORT AudioEvent : public wxCommandEvent
{

public:
    /** @brief	Constructor
    *   @param commandType should be wxEVT_NI...
    *   @param id the ID of the button list control
    **/
    AudioEvent(timedbuffer* pBuffer, int nCreator, unsigned long nFrameCount, unsigned long nSampleRate, bool bUnderrun, bool bOverrun);

    AudioEvent();
    /** @brief	Copy Constructor
    *   @param event a wxNIEvent
    **/
    AudioEvent(const AudioEvent& event);

    /** @brief	Destructor
    **/
    virtual ~AudioEvent(){}

    /** @brief	Creates a copy of the wxNIEvent
    *   @return <i>wxNIEvent</i>
    **/
    virtual wxEvent *Clone() const { return new AudioEvent(*this); }


    timedbuffer* GetBuffer() const;
    int GetCreator() const;
    unsigned long GetFrameCount() const;
    unsigned long GetSampleRate() const;
    int GetStatus() const;
    enum {OK, UNDERRUN, OVERRUN};
    enum {SOUNDCARD = 0, RTP, OUTPUT, FILE, GENERATOR };
protected:

    timedbuffer* m_pBuffer;
    int m_nCreator;
    unsigned long m_nFrameCount;
    unsigned long m_nSampleRate;
    int m_nStatus;
};

typedef void (wxEvtHandler::*AudioEventFunction)(AudioEvent&);


wxDECLARE_EXPORTED_EVENT(PAMBASE_IMPEXPORT, wxEVT_DATA,AudioEvent);


