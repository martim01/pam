#include "generator.h"
#include "timedbuffer.h"
#include "audio.h"
#include <wx/log.h>
#include <iterator>
#include "playout.h"
#include "soundfile.h"

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

    m_lstSequence.push_back(genfreq(dFrequency, dAmplitude, nCycles,nType));
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
    m_lstSequence.end();
}

const std::list<genfreq>::iterator& Sequence::GetSequencePosition()
{
    return m_itPosition;
}

void Sequence::AdvanceSequence()
{
    (*m_itPosition).dCycleCount = 0.0;
    //(*m_itPosition).nPhase = 0;
    ++m_itPosition;
    if(m_itPosition == m_lstSequence.end())
    {
        m_itPosition = m_lstSequence.begin();
    }
}


Generator::Generator(Playback* pPlayback) :
    m_pPlayback(pPlayback),
    m_dSampleRate(48000),
    m_nPhase(0),
    m_pSoundfile(0)
{

}

Generator::~Generator()
{
    ClearSequences();
    ClearFrequences();
    CloseFile();
}

void Generator::SetSampleRate(unsigned int nSampleRate)
{
    m_dSampleRate = nSampleRate;
    m_nPhase = 0;
}


void Generator::Generate(unsigned int nSize)
{
    if(m_pSoundfile)
    {
        ReadSoundFile(nSize);
    }
    else
    {
        timedbuffer* pData = new timedbuffer(nSize);
        float dSize(m_mSequences.size());

        if(m_mSequences.empty() == false)
        {
            for(int i = 0; i < nSize; i++)
            {
                pData->GetWritableBuffer()[i] = 0.0;
            }

            for(map<wxString, Sequence*>::iterator itSequence = m_mSequences.begin(); itSequence != m_mSequences.end(); ++itSequence)
            {
                GenerateSequence(itSequence->second, pData->GetWritableBuffer(), nSize);
            }

            m_nPhase += (nSize/2);
            if(m_nPhase >= static_cast<unsigned long>(m_dSampleRate))
            {
                m_nPhase -= static_cast<unsigned long>(m_dSampleRate);
            }
        }
        else if(m_queueFreq.empty() == false)
        {
            GenerateFrequency(pData->GetWritableBuffer(), nSize);
        }

        pData->SetDuration(pData->GetBufferSize()*4);
        m_pPlayback->AddSamples(pData);
    }
}

void Generator::GenerateSequence(Sequence* pSeq, float* pBuffer, unsigned int nSize)
{
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

        if(pSeq->GetSequencePosition()->nCycles > 0)
        {
            pSeq->GetSequencePosition()->dCycleCount+= (pSeq->GetSequencePosition()->dFrequency/m_dSampleRate);
            if(pSeq->GetSequencePosition()->dCycleCount >= pSeq->GetSequencePosition()->nCycles)
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

        m_queueFreq.front().dCycleCount+= (m_queueFreq.front().dFrequency/m_dSampleRate);
        if(m_queueFreq.front().dCycleCount >= 1.0)
        {
           m_queueFreq.front().dCycleCount -= 1.0;
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
    float dAmplitude = 0.0;
    if(ddBFS > -80.0)
    {
        dAmplitude = pow(10.0, ddBFS/20.0);
    }

    m_queueFreq.push(genfreq(dFrequency, dAmplitude, 1, nType));
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

bool Generator::SetFile(const wxString& sFilePath)
{
    ClearSequences();
    ClearFrequences();

    CloseFile();

    m_pSoundfile = new SoundFile();
    bool bOk(m_pSoundfile->OpenToRead(sFilePath));
    if(bOk == false)
    {
        CloseFile();
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
            m_pPlayback->AddSamples(pData);
        }
    }
}
