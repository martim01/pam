#pragma once
#include <wx/thread.h>
#include "soundfile.h"
#include <queue>

class timedbuffer;

class RecordThread : public wxThread
{
    public:
        RecordThread();
        bool Init(const wxString& sFilename, unsigned int nChannels, unsigned int nSampleRate, unsigned int nBitRate);
        void* Entry();

        void AddToBuffer(const timedbuffer* pBuffer);

    private:

        timedbuffer* CopyBuffer(const timedbuffer* pBuffer);
        void ClearQueue();

        wxMutex m_mutex;
        SoundFile m_sf;

        std::queue<timedbuffer*> m_queueBuffer;
};
