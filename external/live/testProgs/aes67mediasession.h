#pragma once

#include "MediaSession.hh"

class Aes67MediaSubsession;


class Aes67MediaSession : public MediaSession
{
    public:
        static Aes67MediaSession* createNew(UsageEnvironment& env, char const* sdpDescription);

    protected:

        Aes67MediaSession(UsageEnvironment& env);
        //~Aes67MediaSession();

        MediaSubsession* createNewMediaSubsession();
};


class Aes67MediaSubsession : public MediaSubsession
{
    public:
        unsigned int GetSyncTime()
        {
            return m_nSyncTime;
        }
//        unsigned int GetFirstTimeStamp()
//        {
//            return m_nFirstTimestamp;
//        }

        //unsigned int GetExpectedTimestamp();
        //unsigned int GetExpectedTimestamp(const timeval& tv);
//        timeval GetTransmissionTime(unsigned int nRTPTimestamp);

    protected:
        friend class Aes67MediaSession;
        Aes67MediaSubsession(MediaSession& parent);
        virtual Boolean createSourceObjects(int useSpecialRTPoffset);



        Boolean parseSDPAttribute_Sync();
        Boolean parseSDPAttribute_Domain();
        Boolean parseSDPAttribute_Deviation();

        unsigned int m_nSyncTime;
        unsigned int m_nFirstTimestamp;


};
