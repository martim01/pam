#pragma once
#include <wx/string.h>
#include "MediaSession.hh"
#include "timedbuffer.h"

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
        unsigned long GetSyncTime()
        {
            return m_nSyncTime;
        }

        const pairTime_t& GetLastEpoch();

        wxString GetEndpoint();

    protected:
        friend class Aes67MediaSession;
        Aes67MediaSubsession(MediaSession& parent);
        virtual Boolean createSourceObjects(int useSpecialRTPoffset);



        Boolean parseSDPAttribute_Sync();
        Boolean parseSDPAttribute_Domain();
        Boolean parseSDPAttribute_Deviation();

        unsigned long m_nSyncTime;
        unsigned int m_nFirstTimestamp;
        wxString m_sEndpoint;


};
