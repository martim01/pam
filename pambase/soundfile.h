#ifndef SOUNDFILE_H
#define SOUNDFILE_H


#include <fstream>
#include <vector>
#include <wx/string.h>
#include "dlldefine.h"

typedef struct tagWAVFORMAT
{
	long           chunkSize;
	short          wFormatTag;
	unsigned short wChannels;
	unsigned long  dwSamplesPerSec;
	unsigned long  dwAvgBytesPerSec;
	unsigned short wBlockAlign;
	unsigned short wBitsPerSample;
}WAVFORMAT;


class timedbuffer;


class PAMBASE_IMPEXPORT SoundFile
{
public:
	SoundFile(void);
	~SoundFile(void);

protected:
	std::ifstream m_if;
	std::ofstream m_of;
	std::vector<char> m_vBuffer;

	unsigned int m_nLengthAfterRiff;
	unsigned int m_nFileLength;
	unsigned int m_nAudioByteLength;
	WAVFORMAT m_wavformat;

	unsigned int m_nAudioDataPointer;
	bool m_bLooped;


public:
	bool OpenToRead(const wxString& sFileName);
	bool ReadAudio(std::vector<char>& vBuffer, unsigned int nLoop);
	bool ReadAudio(std::vector<short>& vBuffer, unsigned int nLoop);




	bool Seek(unsigned int nSamples);
	bool Offset(int nSamples);
	bool Close(void);

    bool ReadEOF()
    {   return m_if.eof();  }
    unsigned int GetNumberOfBytesRead()
    {   return m_if.gcount();   }

    bool ReadIsOk()
    {
        return m_if.good();
    }
    size_t GetLastReadCount()
    {
        return m_if.gcount();
    }

    bool Looped()
    {
        return m_bLooped;
    }

protected:
	bool CheckHeader(const std::string& sExpected);
	bool FindHeader(const std::string& sHeader);


public:
	const WAVFORMAT& GetFormat(void);
	bool OpenToWrite(const wxString& sFileName, unsigned short nChannels, unsigned long nSampleRate, unsigned short nBitLength);
	bool WriteAudio(std::vector<char>& vBuffer);
	bool WriteAudio(char* pBuffer, int nSize);
	bool WriteAudio(const float* pBuffer, int nSize);
	bool WriteAudio(const timedbuffer* pBuffer, unsigned short nChannels, unsigned short nLef, unsigned short nRight);
	unsigned int GetNumberOfSamples()
	{	return m_nAudioByteLength / (m_wavformat.wChannels * m_wavformat.wBitsPerSample);	}
	unsigned int GetAudioByteLength()
	{	return m_nAudioByteLength;	}

};

#endif
