#include "soundfile.h"

#include <wx/log.h>

SoundFile::SoundFile(void) : m_bLooped(false)
{
}

SoundFile::~SoundFile(void)
{
	Close();
}

/*	opens a file for reading
	checks to make sure it is a wav file
	then reads in the format chunk and positions itself ready to read in data
Return: true if it is a wav file of PCM type and data chunk found
*/
bool SoundFile::OpenToRead(const wxString& sFileName)
{
	//make sure ofstream is not open
	if(m_of.is_open())
		return false;

	//attempt to open the file
	m_if.open(sFileName.mb_str(),std::ios_base::binary | std::ios_base::in);
	if(!m_if.is_open())
	{
	    return false;
	}

	m_if.seekg (0, std::ios::end);
	m_nFileLength = m_if.tellg();
	m_if.seekg (0, std::ios::beg);

	//make sure file is a RIFF file
	if(!CheckHeader("RIFF"))
    {
        wxLogDebug(wxT("Not RIFF"));
        return false;
    }
	//Get file length
	m_if.read(reinterpret_cast<char*>(&m_nLengthAfterRiff),4);

	//make sure file is a WAVE file
	if(!CheckHeader("WAVE"))
    {
        wxLogDebug(wxT("Not WAVE"));
        return false;
    }

	//find and read in the format
	if(!FindHeader("fmt "))
	{
	    wxLogDebug(wxT("Could not find fmt "));
        return false;
	}
	m_if.read(reinterpret_cast<char*>(&m_wavformat),20);

	//make sure not a compressed wav format
	if(m_wavformat.wFormatTag != 1)	//PCM
	{
	    wxLogDebug(wxT("NOT PCM"));
	    return false;
	}


	//find the data header and read in length of audio data
	if(!FindHeader("data"))
	{
	    wxLogDebug(wxT("Could not find data"));
	    return false;
	}


	m_if.read(reinterpret_cast<char*>(&m_nAudioByteLength),4);
	m_nAudioDataPointer = m_if.tellg();

    //any errors in stream?
    if(!m_if)
        return false;

	return true;
}

/* Checks that the next 4 bytes read in == sExpected
Return: true if next 4 bytes == sExpected
*/
bool SoundFile::CheckHeader(const std::string& sExpected)
{
	char text[5];
	m_if.read(&text[0],4);
	text[4] = 0;

	if(sExpected.compare(text) != 0)
		return false;
	return true;
}

/*	Searches from the current position attempting to find the chunk sHeader
Return:	true if chunk found
*/
bool SoundFile::FindHeader(const std::string& sHeader)
{

	char text[5];
	do
	{
		//read in the header
		m_if.read(&text[0],4);
		text[4] = 0;
		if(sHeader.compare(text)==0)
			return true;

		//not found so read in length of chunk
		//m_if.read((char*)&nChunkLength,4);
		//m_if.seekg(nChunkLength);
		m_if.seekg(-3, std::ios::cur);
	}while(m_if.tellg() < (int)m_nFileLength);



	return false;
}

/*	Reads in vBuffer.size() bytes
Return:	true if success
*/
bool SoundFile::ReadAudio(std::vector<char>& vBuffer, unsigned int nLoop)
{
	if(!m_if.is_open())
		return false;

	//make sure we've not read all the audio
	int nAudioPointer = (int)m_if.tellg() - m_nAudioDataPointer;

	std::vector<char>::pointer ptr = &vBuffer[0];

	if(nLoop == 0)
	{
        if(vBuffer.size() + nAudioPointer > m_nAudioByteLength)
        {
            vBuffer.resize(m_nAudioByteLength - nAudioPointer);
            m_bLooped = true;
        }
        else
        {
            m_bLooped = false;
        }
        m_if.read(ptr,(int)vBuffer.size());
    }
	else
	{
	    if(vBuffer.size() + nAudioPointer > m_nAudioByteLength)
	    {
	        m_if.read(ptr,m_nAudioByteLength - nAudioPointer);
            wxASSERT(m_if.gcount()%2 == 0);
            Seek(0);
            ptr = &vBuffer[m_nAudioByteLength - nAudioPointer];
            m_if.read(ptr, vBuffer.size() - (m_nAudioByteLength - nAudioPointer));
            m_bLooped = true;
	    }
	    else
	    {
	        m_if.read(ptr,(int)vBuffer.size());
	        m_bLooped = false;
	    }
	}


    //any errors reading?
    if(!m_if.good() && !m_if.eof())
        return false;
	return true;
}


bool SoundFile::ReadAudio(std::vector<short>& vBuffer, unsigned int nLoop)
{
	if(!m_if.is_open())
		return false;

	//make sure we've not read all the audio
	int nAudioPointer = (int)m_if.tellg() - m_nAudioDataPointer;

	std::vector<short>::pointer ptr = &vBuffer[0];

	if(nLoop == 0)
	{
        if(vBuffer.size()*2 + nAudioPointer > m_nAudioByteLength)
        {
            vBuffer.resize((m_nAudioByteLength - nAudioPointer)/2);
            m_bLooped = true;
        }
        else
        {
            m_bLooped = false;
        }
        m_if.read(reinterpret_cast<char*>(ptr),(int)vBuffer.size()*2);
    }
	else
	{
	    if(vBuffer.size()*2 + nAudioPointer > m_nAudioByteLength)
	    {
	        m_if.read(reinterpret_cast<char*>(ptr), (m_nAudioByteLength - nAudioPointer));
            //wxASSERT(m_if.gcount()%2 == 0);

            Seek(0);
            ptr = &vBuffer[(m_nAudioByteLength - nAudioPointer)/2];
            m_if.read(reinterpret_cast<char*>(ptr), vBuffer.size()*2 - (m_nAudioByteLength - nAudioPointer));
            m_bLooped = true;
	    }
	    else
	    {
	        m_if.read(reinterpret_cast<char*>(ptr),(int)vBuffer.size()*2);
	        m_bLooped = false;
	    }
	}


    //any errors reading?
    if(!m_if.good() && !m_if.eof())
        return false;
	return true;
}




/* Moves the file pointer to nSamples from the start of the audio data if possible
Return: true if success
*/
bool SoundFile::Seek(unsigned int nSamples)
{
	if(!m_if.is_open())
		return false;
	//work out how many bytes
	unsigned int nBytes = nSamples * m_wavformat.wChannels * m_wavformat.wBitsPerSample;
	nBytes += m_nAudioDataPointer;
	//make sure not at end of file
	if(nBytes > m_nAudioByteLength)
		return false;

	m_if.seekg(nBytes,std::ios::beg);

    if(!m_if)
        return false;

	return true;
}
/* Moves the file pointer to nSamples from the current position if possible
Return: true if success
*/
bool SoundFile::Offset(int nSamples)
{
	if(!m_if.is_open())
		return false;
	//work out how many bytes
	unsigned int nBytes = nSamples * m_wavformat.wChannels * m_wavformat.wBitsPerSample;
	//make sure not at end of file nor at beginning
	if((int)m_if.tellg() + nBytes > m_nAudioByteLength ||
		(int)m_if.tellg() - nBytes < 0)
		return false;


	m_if.seekg(nBytes);

    if(!m_if)
        return false;

	return true;
}

bool SoundFile::Close(void)
{
	if(m_if.is_open())
		m_if.close();
	if(m_of.is_open())
		m_of.close();

	return true;
}

const WAVFORMAT& SoundFile::GetFormat(void)
{
	return m_wavformat;
}

bool SoundFile::OpenToWrite(const wxString& sFileName, unsigned short nChannels, unsigned long nSampleRate, unsigned short nBitLength)
{
	//make sure ifstream is not open
	if(m_if.is_open())
		return false;

	//attempt to open/create the file
	m_of.open(sFileName.mb_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	if(!m_of.is_open())
		return false;

	//write our header information
	std::string sChunkHeader;
	unsigned int nChunkLength;
	//RIFF
	sChunkHeader = "RIFF";
	nChunkLength = 0;
	m_of.write(sChunkHeader.c_str(),4);
	m_of.write((char*)&nChunkLength,4);
	//WAVE
	sChunkHeader = "WAVE";
	m_of.write(sChunkHeader.c_str(),4);

	//fmt
	sChunkHeader = "fmt ";
	m_of.write(sChunkHeader.c_str(),4);

	WAVFORMAT wf;
	wf.chunkSize = 16;
	wf.wFormatTag = 1;
	wf.wChannels = nChannels;
	wf.dwSamplesPerSec = nSampleRate;
	wf.wBitsPerSample = nBitLength;
	wf.wBlockAlign = wf.wChannels * (wf.wBitsPerSample / 8);
	wf.dwAvgBytesPerSec = wf.dwSamplesPerSec * wf.wBlockAlign;

	m_of.write((char*)&wf,20);

	//data
	sChunkHeader = "data";
	nChunkLength = 0;
	m_of.write(sChunkHeader.c_str(),4);
	m_of.write((char*)&nChunkLength,4);

	m_nAudioDataPointer = m_of.tellp();

	//set audio bytes written to zero
	m_nAudioByteLength = 0;

    //any errors in stream?
    if(!m_of)
        return false;
	return true;
}

bool SoundFile::WriteAudio(std::vector<char>& vBuffer)
{
	if(!m_of.is_open())
		return false;

	//write the data
	std::vector<char>::pointer ptr = &vBuffer[0];
	m_of.seekp(0,std::ios::end);
	m_of.write(ptr,(int)vBuffer.size());

	m_nAudioByteLength += (int)vBuffer.size();

	//rewrite the length bits
	unsigned int nLen = m_of.tellp();
	m_of.seekp(4,std::ios::beg);
	m_of.write((char*)&nLen,4);

	//skip the "WAVE" bit and the fmt chunk and the data bit
	m_of.seekp(m_nAudioDataPointer-4,std::ios::beg);
	m_of.write((char*)&m_nAudioByteLength,4);

    //any errors in writing?
    if(!m_of)
        return false;

	return true;

}

bool SoundFile::WriteAudio(char* pBuffer, int nSize)
{
	if(!m_of.is_open())
		return false;

	//write the data

	m_of.seekp(0,std::ios::end);
	m_of.write(pBuffer,nSize);

	m_nAudioByteLength += nSize;

	//rewrite the length bits
	unsigned int nLen = m_of.tellp();
	m_of.seekp(4,std::ios::beg);
	m_of.write((char*)&nLen,4);

	//skip the "WAVE" bit and the fmt chunk and the data bit
	m_of.seekp(m_nAudioDataPointer-4,std::ios::beg);
	m_of.write((char*)&m_nAudioByteLength,4);

     //any errors in writing?
    if(!m_of)
        return false;

	return true;

}


bool SoundFile::WriteAudio(const float* pBuffer, int nSize)
{
	if(!m_of.is_open())
		return false;

	//write the data

	m_of.seekp(0,std::ios::end);

	for(int i = 0; i < nSize; i++)
    {
        short nValue = static_cast<int>(pBuffer[i]*32768.0);
        m_of.write((char*)&nValue, 2);
        m_nAudioByteLength +=2;
    }


	//rewrite the length bits
	unsigned int nLen = m_of.tellp();
	m_of.seekp(4,std::ios::beg);
	m_of.write((char*)&nLen,4);

	//skip the "WAVE" bit and the fmt chunk and the data bit
	m_of.seekp(m_nAudioDataPointer-4,std::ios::beg);
	m_of.write((char*)&m_nAudioByteLength,4);

     //any errors in writing?
    if(!m_of)
        return false;

	return true;

}
