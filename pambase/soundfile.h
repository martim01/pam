#pragma once

#include <wx/string.h>


#include "dlldefine.h"


class timedbuffer;
class SndfileHandle;

class PAMBASE_IMPEXPORT SoundFile
{
public:
	SoundFile(void);
	~SoundFile(void);

	bool OpenToRead(const wxString& sFileName);
    bool ReadAudio(float* pBuffer, int nSize, unsigned int nLoop);

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
	bool WriteAudio(const timedbuffer* pBuffer);//, unsigned short nChannels, unsigned short nLef, unsigned short nRight);

private:
   SndfileHandle* m_pHandle;
   bool m_bLooped;
   unsigned int m_nWritten;
};
