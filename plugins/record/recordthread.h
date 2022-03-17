#pragma once
#include <wx/thread.h>
#include "soundfile.h"
#include <queue>
#include <atomic>
#include <memory>

class timedbuffer;

class RecordThread : public wxThread
{
    public:
        RecordThread();
        bool Init(const wxString& sFilename, const std::vector<unsigned char>& vChannels, unsigned int nSampleRate, unsigned int nBitRate);
        void* Entry();

        void AddToBuffer(const timedbuffer* pBuffer);

        void Stop();

    private:

        timedbuffer* CopyBuffer(const timedbuffer* pBuffer);
        timedbuffer* FilterBuffer(const timedbuffer* pBuffer);

        void ClearQueue();

        wxMutex m_mutex;
        SoundFile m_sf;
        std::vector<unsigned char> m_vChannels;
        std::atomic<bool> m_bLoop;

        std::queue<timedbuffer*> m_queueBuffer;
};
