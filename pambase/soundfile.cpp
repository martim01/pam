#include "soundfile.h"
#include "timedbuffer.h"
#include "sndfile.hh"

SoundFile::SoundFile(void) : m_pHandle(0), m_bLooped(false), m_nWritten(0)
{

}

SoundFile::~SoundFile(void)
{
    if(m_pHandle)
    {
        delete m_pHandle;
    }
}

bool SoundFile::OpenToRead(const wxString& sFileName)
{
	if(m_pHandle)
	{
		Close();
	}
    m_pHandle = new SndfileHandle(sFileName.mb_str(), SFM_READ, 0,0,0);
    return (m_pHandle);
}

bool SoundFile::ReadAudio(float* pBuffer, int nSize, unsigned int nLoop)
{
    if(m_pHandle)
    {
        sf_count_t nRead = m_pHandle->read(pBuffer, nSize);
        if(nLoop  != 0)
        {
            while(nRead < nSize)
            {
                m_bLooped = true;

                int nPointer = nRead;
                nSize -= nRead;

                m_pHandle->seek(SEEK_SET, 0);
                nRead = m_pHandle->read(&pBuffer[nPointer], nSize);
            }
        }
        return true;
    }
    return false;
}

bool SoundFile::Close(void)
{
    delete m_pHandle;
    m_pHandle = 0;
    return true;
}


bool SoundFile::OpenToWrite(const wxString& sFileName, unsigned short nChannels, unsigned long nSampleRate, unsigned short nBitLength)
{
    int nFormat = SF_FORMAT_WAV;
    switch(nBitLength)
    {
        case 16:
            nFormat |= SF_FORMAT_PCM_16;
            break;
        case 24:
            nFormat |= SF_FORMAT_PCM_24;
            break;
        case 32:
            nFormat |= SF_FORMAT_PCM_32;
            break;
        default:
            nFormat |= SF_FORMAT_FLOAT;
    }

    m_pHandle = new SndfileHandle(sFileName.mb_str(), SFM_WRITE, nFormat, nChannels, nSampleRate);
    m_nWritten = 0;
    return m_pHandle;
}

bool SoundFile::WriteAudio(const timedbuffer* pBuffer)//, unsigned short nChannels, unsigned short nLef, unsigned short nRight)
{
    if(m_pHandle)
    {
        m_nWritten += m_pHandle->write(pBuffer->GetBuffer(), pBuffer->GetBufferSize());
        return true;
    }
    return false;
}


unsigned int SoundFile::GetLength()
{
    if(m_pHandle)
    {
        return  m_nWritten;
    }
    return 0;
}


int	 SoundFile::GetFormat () const
{
    if(m_pHandle)
    {
        return m_pHandle->format();
    }
    return 0;
}

int	 SoundFile::GetChannels () const
{
    if(m_pHandle)
    {
        return m_pHandle->channels();
    }
    return 0;
}

int	 SoundFile::GetSampleRate () const
{
    if(m_pHandle)
    {
        return m_pHandle->samplerate();
    }
    return 0;
}
