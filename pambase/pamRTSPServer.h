#pragma once
#include <RTSPServer.hh>

class PamRTSPServer: public RTSPServer
{
    public:
        static PamRTSPServer* createNew(UsageEnvironment& env, Port ourPort = 554,UserAuthenticationDatabase* authDatabase = NULL, unsigned reclamationSeconds = 65);

    protected:
        PamRTSPServer(UsageEnvironment& env, int ourSocket, Port ourPort, UserAuthenticationDatabase* authDatabase, unsigned reclamationSeconds);
        // called only by createNew();
        virtual ~PamRTSPServer();

    public: // should be protected, but some old compilers complain otherwise
        // The state of a TCP connection used by a RTSP client:

        class PamRTSPClientConnection: public RTSPClientConnection
        {
            protected: // redefined virtual functions:
                virtual void handleRequestBytes(int newBytesRead);

                PamRTSPClientConnection(RTSPServer& ourServer, int clientSocket, struct sockaddr_in clientAddr);
                virtual ~PamRTSPClientConnection();

                friend class RTSPServer;
                friend class PamRTSPServer;
                friend class RTSPClientSession;
                friend class PamRTSPClientSession;

                // Make the handler functions for each command virtual, to allow subclasses to reimplement them, if necessary:
                virtual void handleCmd_OPTIONS();
                virtual void handleCmd_GET_PARAMETER(char const* fullRequestStr); // when operating on the entire server
                virtual void handleCmd_SET_PARAMETER(char const* fullRequestStr); // when operating on the entire server
                virtual void handleCmd_DESCRIBE(char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr);
                virtual void handleCmd_REGISTER(char const* cmd, char const* url, char const* urlSuffix, char const* fullRequestStr, Boolean reuseConnection, Boolean deliverViaTCP, char const* proxyURLSuffix);
                virtual void handleCmd_bad();
                virtual void handleCmd_notSupported();
                virtual void handleCmd_notFound();
                virtual void handleCmd_sessionNotFound();
                virtual void handleCmd_unsupportedTransport();

        };

        // The state of an individual client session (using one or more sequential TCP connections) handled by a RTSP server:
        class PamRTSPClientSession: public RTSPClientSession
        {
            protected:
                PamRTSPClientSession(RTSPServer& ourServer, u_int32_t sessionId);
                virtual ~PamRTSPClientSession();

                friend class RTSPServer;
                friend class PamRTSPServer;
                friend class RTSPClientConnection;
                friend class PamRTSPClientConnection;
                virtual void handleCmd_SETUP(RTSPClientConnection* ourClientConnection, char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr);
                virtual void handleCmd_withinSession(RTSPClientConnection* ourClientConnection, char const* cmdName, char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr);
                virtual void handleCmd_TEARDOWN(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession);
                virtual void handleCmd_PLAY(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession, char const* fullRequestStr);
                virtual void handleCmd_PAUSE(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession);
                virtual void handleCmd_GET_PARAMETER(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession, char const* fullRequestStr);
                virtual void handleCmd_SET_PARAMETER(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession, char const* fullRequestStr);
        };

    protected: // redefined virtual functions
        virtual ClientConnection* createNewClientConnection(int clientSocket, struct sockaddr_in clientAddr);
        virtual ClientSession* createNewClientSession(u_int32_t sessionId);

};


