#pragma once

#include "RTSPServer.hh"

class PamRTSPServer : public RTSPServer
{
    public:
        static PamRTSPServer* createNew(UsageEnvironment& env, Port ourPort = 554,
			       UserAuthenticationDatabase* authDatabase = NULL,
			       unsigned reclamationSeconds = 65);

}
