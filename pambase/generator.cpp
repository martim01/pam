#include "generator.h"
#include "timedbuffer.h"
#include "audio.h"
#include <wx/log.h>
#include <iterator>
#include "playout.h"
#include "settings.h"
#include "soundfile.h"
#include "soundcardmanager.h"
#include <wx/xml/xml.h>
#include "wmlogevent.h"


using namespace std;

Sequence::Sequence(int nChannels) : m_nChannels(nChannels)
{
    m_itPosition = m_lstSequence.end();
}
void Sequence::AppendGenFreq(float dFrequency, float ddBFS, int nCycles, int nType)
{

    float dAmplitude(0.0);
    if(ddBFS > -80.0)
    {
        dAmplitude = pow(10.0, ddBFS/20.0);
    }

    double dSamples = static_cast<double>(nCycles)*48000.0/dFrequency;


    m_lstSequence.push_back(genfreq(dFrequency, dAmplitude, static_cast<int>(dSamples),nType));
    if(m_itPosition == m_lstSequence.end())
    {
        m_itPosition = m_lstSequence.begin();
    }
}

list<genfreq>::iterator Sequence::GetSequenceBegin()
{
    return m_lstSequence.begin();
}

list<genfreq>::iterator Sequence::GetSequenceEnd()
{
    return m_lstSequence.end();
}

const std::list<genfreq>::iterator& Sequence::GetSequencePosition()
{
    return m_itPosition;
}

void Sequence::AdvanceSequence()
{
    (*m_itPosition).nSampleCount = 0;
    //(*m_itPosition).nPhase = 0;
    ++m_itPosition;
    if(m_itPosition == m_lstSequence.end())
    {
        m_itPosition = m_lstSequence.begin();
    }
}


Generator::Generator() :
    m_dSampleRate(48000),
    m_nPhase(0),
    m_pSoundfile(0)
{
    m_pPink[0] = 0;
    m_pPink[1] = 0;
    srand(time(0));
}

Generator::~Generator()
{
    ClearSequences();
    ClearFrequences();
    CloseFile();
    ClosePink();
}

void Generator::SetSampleRate(unsigned int nSampleRate)
{
    m_dSampleRate = nSampleRate;
    m_nPhase = 0;
}


void Generator::Generate(unsigned int nSize)
{

    if(m_nGenerator == FILE)
    {
        ReadSoundFile(nSize);
    }
    else
    {

        timedbuffer* pData = new timedbuffer(nSize);
        float dSize(m_mSequences.size());

        switch(m_nGenerator)
        {
            case SEQUENCE:
                GenerateSequences(pData);
                break;
            case FREQUENCY:
                GenerateFrequency(pData->GetWritableBuffer(), nSize);
                break;
            case NOISE_PINK:
                GeneratePinkNoise(pData->GetWritableBuffer(), nSize);
                break;
            case NOISE_WHITE:
                GenerateWhiteNoise(pData->GetWritableBuffer(), nSize);
                break;
        }

        pData->SetDuration(pData->GetBufferSize()*4);

        wxLogDebug(wxT("Generated: %d"), pData->GetBufferSize());
        SoundcardManager::Get().AddOutputSamples(pData);
        delete pData;
    }
}

void Generator::GenerateSequences(timedbuffer* pData)
{
    for(int i = 0; i < pData->GetBufferSize(); i++)
    {
        pData->GetWritableBuffer()[i] = 0.0;
    }
    for(map<wxString, Sequence*>::iterator itSequence = m_mSequences.begin(); itSequence != m_mSequences.end(); ++itSequence)
    {
        GenerateSequence(itSequence->second, pData->GetWritableBuffer(), pData->GetBufferSize());
    }
    m_nPhase += (pData->GetBufferSize()/2);
    if(m_nPhase >= static_cast<unsigned long>(m_dSampleRate))
    {
        m_nPhase -= static_cast<unsigned long>(m_dSampleRate);
    }
}

void Generator::GenerateSequence(Sequence* pSeq, float* pBuffer, unsigned int nSize)
{
    wxLogDebug(wxT("Geneate: Sequence"));

    unsigned int nPhase = m_nPhase;

    for(int i = 0; i < nSize; i+=2)
    {
        float dAmplitude(0.0);
        switch(pSeq->GetSequencePosition()->nType)
        {
            case SINE:
                dAmplitude = GenerateSin((*pSeq->GetSequencePosition()),nPhase);
                break;
            case SQUARE:
                dAmplitude = GenerateSquare((*pSeq->GetSequencePosition()),nPhase);
                break;
            case SAW:
                dAmplitude = GenerateSaw((*pSeq->GetSequencePosition()),nPhase);
                break;
            case TRIANGLE:
                dAmplitude = GenerateTriangle((*pSeq->GetSequencePosition()),nPhase);
                break;
        }
        if((pSeq->GetChannels() & Sequence::LEFT))
        {
            pBuffer[i] += dAmplitude;
        }
        if((pSeq->GetChannels() & Sequence::RIGHT))
        {
            pBuffer[i+1] += dAmplitude;
        }

        nPhase++;
        if(nPhase == static_cast<unsigned long>(m_dSampleRate))
        {
            nPhase = 0;
        }

        if(pSeq->GetSequencePosition()->nSamplesToPlay > 0)
        {
            pSeq->GetSequencePosition()->nSampleCount++;
            if(pSeq->GetSequencePosition()->nSampleCount == pSeq->GetSequencePosition()->nSamplesToPlay)
            {
                pSeq->AdvanceSequence();
            }
        }

    }
}

void Generator::GenerateFrequency(float* pBuffer, unsigned int nSize)
{

    for(int i = 0; i < nSize; i+=2)
    {
        float dAmplitude(0.0);
        switch(m_queueFreq.front().nType)
        {
            case SINE:
                dAmplitude = GenerateSin(m_queueFreq.front(), m_nPhase);
                break;
            case SQUARE:
                dAmplitude = GenerateSquare(m_queueFreq.front(), m_nPhase);
                break;
            case SAW:
                dAmplitude = GenerateSaw(m_queueFreq.front(), m_nPhase);
                break;
            case TRIANGLE:
                dAmplitude = GenerateTriangle(m_queueFreq.front(), m_nPhase);
                break;
        }

        pBuffer[i] = dAmplitude;
        pBuffer[i+1] = dAmplitude;


        m_nPhase++;
        if(m_nPhase == static_cast<unsigned long>(m_dSampleRate))
        {
            m_nPhase = 0;
        }

        m_queueFreq.front().nPhase++;
        if(m_queueFreq.front().nPhase == static_cast<unsigned long>(m_dSampleRate))
        {
            m_queueFreq.front().nPhase = 0;
        }

        m_queueFreq.front().nSampleCount++;
        if(m_queueFreq.front().nSampleCount == m_queueFreq.front().nSamplesToPlay)
        {
           m_queueFreq.front().nSampleCount = 0;
            if(m_queueFreq.size() > 1)
            {
                m_queueFreq.pop();
            }
        }
    }
}

float Generator::GenerateSin(const genfreq& gfreq, float dPhase)
{
    float dIn(2.0 * M_PI * gfreq.dFrequency*(dPhase/m_dSampleRate));
    float dSin = sin(dIn) * gfreq.dAmplitude;

    return dSin;
}

float Generator::GenerateSquare(const genfreq& gfreq, float dPhase)
{
    float dAmp(sin(2.0 * M_PI * gfreq.dFrequency*(dPhase/m_dSampleRate)));
    if(dAmp >0 )
    {
        return gfreq.dAmplitude ;
    }
    else if(dAmp < 0)
    {
        return -gfreq.dAmplitude ;
    }
    return 0;
}

float Generator::GenerateSaw(const genfreq& gfreq, float dPhase)
{
    return - (2.0*gfreq.dAmplitude*atan(tan(M_PI_2 - (M_PI * gfreq.dFrequency*(dPhase/m_dSampleRate)) )))/M_PI;
}

float Generator::GenerateTriangle(const genfreq& gfreq, float dPhase)
{
    return (2.0*gfreq.dAmplitude*asin(sin(2 * M_PI * gfreq.dFrequency*(dPhase/m_dSampleRate))))/M_PI;
}


void Generator::AddSequence(const wxString& sName, Sequence* pSeq)
{


    m_mSequences.insert(make_pair(sName, pSeq));
}

void Generator::ClearSequences()
{
    for(map<wxString, Sequence*>::iterator itSeq = m_mSequences.begin(); itSeq != m_mSequences.end(); ++itSeq)
    {
        delete itSeq->second;
    }
    m_mSequences.clear();
}

void Generator::DeleteSequence(const wxString& sName)
{
    map<wxString, Sequence*>::iterator itSeq = m_mSequences.find(sName);
    if(itSeq != m_mSequences.end())
    {
        delete itSeq->second;
        m_mSequences.erase(sName);
    }
}


void Generator::SetFrequency(float dFrequency, float ddBFS, int nType)
{
    ClearSequences();
    CloseFile();
    ClosePink();

    m_nGenerator = FREQUENCY;

    float dAmplitude = 0.0;
    if(ddBFS > -80.0)
    {
        dAmplitude = pow(10.0, ddBFS/20.0);
    }

    m_queueFreq.push(genfreq(dFrequency, dAmplitude, 48000.0/dFrequency, nType));


}

void Generator::ClearFrequences()
{
    while(m_queueFreq.empty() == false)
    {
        m_queueFreq.pop();
    }
}

void Generator::CloseFile()
{
    if(m_pSoundfile)
    {
        delete m_pSoundfile;
        m_pSoundfile = 0;
    }
}

bool Generator::SetFile()
{
    ClearSequences();
    ClearFrequences();
    CloseFile();
    ClosePink();

    m_nGenerator = FILE;

    wxString sFilePath;
    sFilePath << Settings::Get().GetWavDirectory() << wxT("/") << Settings::Get().Read(wxT("Output"), wxT("File"), wxEmptyString) << wxT(".wav");

    bool bOk(false);

    //Does the file exist
    if(wxFileExists(sFilePath))
    {
        m_pSoundfile = new SoundFile();
        bOk = m_pSoundfile->OpenToRead(sFilePath);
        if(bOk == false)
        {
            CloseFile();
            wmLog::Get()->Log(wxString::Format(wxT("Failed to open file '%s'"), sFilePath.c_str()));
        }
        else
        {
            wmLog::Get()->Log(wxString::Format(wxT("Opened file '%s'"), sFilePath.c_str()));
            wmLog::Get()->Log(wxString::Format(wxT("SampleRate = %d"), GetSampleRate()));
            wmLog::Get()->Log(wxString::Format(wxT("Channels = %d"), GetChannels()));

            Generate(8192);
        }
    }
    else
    {
        wmLog::Get()->Log(wxString::Format(wxT("File '%s' does not exist or generator not established"), sFilePath.c_str()));
    }

    return bOk;

}

unsigned int Generator::GetSampleRate()
{
    if(m_pSoundfile)
    {
        return m_pSoundfile->GetSampleRate();
    }
    return m_dSampleRate;
}

unsigned int Generator::GetChannels()
{
    if(m_pSoundfile)
    {
        return m_pSoundfile->GetChannels();
    }
    return 2;
}


void Generator::ReadSoundFile(unsigned int nSize)
{
    if(m_pSoundfile)
    {
        timedbuffer* pData = new timedbuffer(nSize);
        if(m_pSoundfile->ReadAudio(pData->GetWritableBuffer(), pData->GetBufferSize(), 1))
        {
            pData->SetDuration(pData->GetBufferSize()*(m_pSoundfile->GetFormat()&0x0F));
            SoundcardManager::Get().AddOutputSamples(pData);
        }
    }
}

void Generator::SetNoise(int nColour, float ddBFS)
{
    ClosePink();
    CloseFile();
    ClearSequences();
    ClearFrequences();

    m_dNoiseAmplitude = pow(10.0, ddBFS/20.0);
    switch(nColour)
    {
    case PINK:
        InitPinkNoise();
        break;
    case WHITE:
        m_nGenerator = NOISE_WHITE;
        break;
    }
    Generate(8192);
}



void Generator::InitPinkNoise()
{
    m_nGenerator = NOISE_PINK;

    m_pPink[0] = new PinkNoise();
    m_pPink[1] = new PinkNoise();
    InitializePinkNoise(16);
}


unsigned long Generator::GenerateRandomNumber( void )
{
    static unsigned long randSeed = 22222;
    randSeed = (randSeed * 196314165) + 907633515;
    return randSeed;
}

void Generator::InitializePinkNoise(int numRows )
{
    for(int nChannel = 0; nChannel < 2; nChannel++)
    {
        int i;
        long pmax;
        m_pPink[nChannel]->pink_Index = 0;
        m_pPink[nChannel]->pink_IndexMask = (1<<numRows) - 1;
        /* Calculate maximum possible signed random value. Extra 1 for white noise always added. */
        pmax = (numRows + 1) * (1<<(PINK_RANDOM_BITS-1));
        m_pPink[nChannel]->pink_Scalar = 1.0f / pmax;
        /* Initialize rows. */
        for( i=0; i<numRows; i++ ) m_pPink[nChannel]->pink_Rows[i] = 0;
        m_pPink[nChannel]->pink_RunningSum = 0;
    }
}

/* Generate Pink noise values between -1.0 and +1.0 */
void Generator::GeneratePinkNoise(float* pBuffer, unsigned int nSize)
{
    long newRandom;
    long sum;
    float output;

    for(unsigned int i = 0; i < nSize; i++)
    {
        /* Increment and mask index. */
        m_pPink[i%2]->pink_Index = (m_pPink[i%2]->pink_Index + 1) & m_pPink[i%2]->pink_IndexMask;
        /* If index is zero, don't update any random values. */
        if( m_pPink[i%2]->pink_Index != 0 )
        {
            /* Determine how many trailing zeros in PinkIndex. */
            /* This algorithm will hang if n==0 so test first. */
            int numZeros = 0;
            int n = m_pPink[i%2]->pink_Index;
            while( (n & 1) == 0 )
            {
                n = n >> 1;
                numZeros++;
            }
            /* Replace the indexed ROWS random value.
             * Subtract and add back to RunningSum instead of adding all the random
             * values together. Only one changes each time.
             */
            m_pPink[i%2]->pink_RunningSum -= m_pPink[i%2]->pink_Rows[numZeros];
            newRandom = ((long)GenerateRandomNumber()) >> PINK_RANDOM_SHIFT;
            m_pPink[i%2]->pink_RunningSum += newRandom;
            m_pPink[i%2]->pink_Rows[numZeros] = newRandom;
        }

        /* Add extra white noise value. */
        newRandom = ((long)GenerateRandomNumber()) >> PINK_RANDOM_SHIFT;
        sum = m_pPink[i%2]->pink_RunningSum + newRandom;
        /* Scale to range of -1.0 to 0.9999. */
        output = m_pPink[i%2]->pink_Scalar * sum;
        pBuffer[i] = output*m_dNoiseAmplitude;

    }
}

void Generator::ClosePink()
{
    if(m_pPink[0])
    {
        delete m_pPink[0];
        delete m_pPink[1];
        m_pPink[0] = 0;
        m_pPink[1] = 0;
    }

}


void Generator::GenerateWhiteNoise(float* pBuffer, unsigned int nSize)
{
    int q = 15;
    float c1 = (1 << q) - 1;
    float c2 = ((int)(c1 / 3)) + 1;
    float c3 = 1.f / c1;

    /* random number in range 0 - 1 not including 1 */
    /* the white noise */
    for (int i = 0; i < nSize; i++)
    {
        float random1 = ((float)rand() / (float)(RAND_MAX + 1));
        float random2 = ((float)rand() / (float)(RAND_MAX + 1));
        pBuffer[i] = ((2.0 * ((random1 * c2) + (random2 * c2) + (random2 * c2)) - 3.0 * (c2 - 1.0)) * c3)*m_dNoiseAmplitude;
    }
}



bool Generator::LoadSequence(const wxString& sFile)
{
    ClearSequences();
    ClearFrequences();
    CloseFile();
    ClosePink();

    m_nGenerator = SEQUENCE;

    wxXmlDocument doc;
    if(doc.Load(wxString::Format(wxT("%s/generator/%s.xml"), Settings::Get().GetDocumentDirectory().c_str(), sFile.c_str())) && doc.GetRoot())
    {
        for(wxXmlNode* pSequenceNode = doc.GetRoot()->GetChildren(); pSequenceNode; pSequenceNode = pSequenceNode->GetNext())
        {
            if(pSequenceNode->GetName().CmpNoCase(wxT("sequence")) == 0)
            {
                unsigned long nChannels(0);
                pSequenceNode->GetAttribute(wxT("channels"), wxT("0")).ToULong(&nChannels);
                Sequence* pSequence = new Sequence(nChannels);

                for(wxXmlNode* pFreqGenNode = pSequenceNode->GetChildren(); pFreqGenNode; pFreqGenNode = pFreqGenNode->GetNext())
                {
                    if(pFreqGenNode->GetName().CmpNoCase(wxT("genfreq")) == 0)
                    {
                        double dFrequency, ddBFS;
                        long nCycles(0), nType(0);
                        if(pFreqGenNode->GetAttribute(wxT("frequency"), wxEmptyString).ToDouble(&dFrequency) && pFreqGenNode->GetAttribute(wxT("dBFS"), wxEmptyString).ToDouble(&ddBFS) && pFreqGenNode->GetAttribute(wxT("cycles"), wxEmptyString).ToLong(&nCycles) && pFreqGenNode->GetAttribute(wxT("type"), wxT("0")).ToLong(&nType))
                        {
                            pSequence->AppendGenFreq(dFrequency, ddBFS, nCycles, nType);
                        }
                    }
                }
                AddSequence(pSequenceNode->GetAttribute(wxT("name"), wxEmptyString), pSequence);
            }
        }
        Generate(8192);
        return true;
    }
    return false;

}

void Generator::Stop()
{
    ClearFrequences();
    ClearSequences();
    CloseFile();
    ClosePink();
}