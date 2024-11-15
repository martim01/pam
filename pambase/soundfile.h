#pragma once

#include <wx/string.h>

#include <memory>
#include "dlldefine.h"
#include <vector>

class timedbuffer;
class SndfileHandle;

class PAMBASE_IMPEXPORT SoundFile
{
public:
	SoundFile(void);
	~SoundFile(void);

	bool OpenToRead(const wxString& sFileName);
    bool ReadAudio(float* pBuffer, int nSize, unsigned int nLoop);
    bool ReadAudio(std::vector<float>& vBuffer, unsigned int nLoop);

	bool Close(void);

    bool Looped()
    {
        return m_bLooped;
    }

    int	 GetFormat () const;
    int	 GetChannels () const;
    int	 GetSampleRate () const;

    unsigned int GetLength();

	bool OpenToWrite(const wxString& sFileName, unsigned short nChannels, unsigned long nSampleRate, unsigned short nBitLength);
	bool WriteAudio(std::shared_ptr<const timedbuffer> pBuffer);
	bool WriteAudio(const timedbuffer* pBuffer);

private:
   SndfileHandle* m_pHandle;
   bool m_bLooped;
   unsigned int m_nWritten;
};
