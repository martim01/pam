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
        bool Init(const wxString& sFilename, unsigned int nChannels, unsigned int nSampleRate, unsigned int nBitRate);
        void* Entry();

        void AddToBuffer(const timedbuffer* pBuffer);

        void Stop();

    private:

        std::shared_ptr<const timedbuffer> CopyBuffer(const timedbuffer* pBuffer);
        void ClearQueue();

        wxMutex m_mutex;
        SoundFile m_sf;
        std::atomic<bool> m_bLoop;

        std::queue<std::shared_ptr<const timedbuffer>> m_queueBuffer;
};
