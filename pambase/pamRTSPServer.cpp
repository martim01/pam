#include "pamRTSPServer.h"
#include "log.h"

PamRTSPServer* PamRTSPServer::createNew(UsageEnvironment& env, Port ourPort,UserAuthenticationDatabase* authDatabase, unsigned reclamationSeconds)
{
    int ourSocket = setUpOurSocket(env, ourPort);
    if (ourSocket == -1)
    {
        return NULL;
    }
    return new PamRTSPServer(env, ourSocket, ourPort, authDatabase, reclamationSeconds);
}

PamRTSPServer::PamRTSPServer(UsageEnvironment& env, int ourSocket, Port ourPort, UserAuthenticationDatabase* authDatabase, unsigned reclamationSeconds) :
RTSPServer(env, ourSocket, ourPort, authDatabase, reclamationSeconds)
{
}

PamRTSPServer::~PamRTSPServer()
{
}

void PamRTSPServer::PamRTSPClientConnection::handleRequestBytes(int newBytesRead)
{
    RTSPClientConnection::handleRequestBytes(newBytesRead);
}

PamRTSPServer::PamRTSPClientConnection::PamRTSPClientConnection(RTSPServer& ourServer, int clientSocket, struct sockaddr_in clientAddr) :
RTSPServer::RTSPClientConnection(ourServer, clientSocket, clientAddr)
{
    //@todo put in client address and port number

    pml::Log::Get() << "RTSPServer\t" << "New Connection: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
}
PamRTSPServer::PamRTSPClientConnection::~PamRTSPClientConnection()
{}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_OPTIONS()
{
    pml::Log::Get() << "RTSPServer\t" << "OPTIONS" << std::endl;
    RTSPClientConnection::handleCmd_OPTIONS();
}
void PamRTSPServer::PamRTSPClientConnection::handleCmd_GET_PARAMETER(char const* fullRequestStr)
{
    pml::Log::Get() << "RTSPServer\t" << "GET_PARAMETER: " << fullRequestStr << std::endl;
    RTSPClientConnection::handleCmd_GET_PARAMETER(fullRequestStr);
}


void PamRTSPServer::PamRTSPClientConnection::handleCmd_SET_PARAMETER(char const* fullRequestStr)
{
    pml::Log::Get() << "RTSPServer\t" << "SET_PARAMETER: " << fullRequestStr << std::endl;
    RTSPClientConnection::handleCmd_SET_PARAMETER(fullRequestStr); // when operating on the entire server
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_DESCRIBE(char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr)
{
    pml::Log::Get() << "RTSPServer\t" << "DESCRIBE: " << fullRequestStr << std::endl;
    RTSPClientConnection::handleCmd_DESCRIBE(urlPreSuffix, urlSuffix, fullRequestStr);
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_REGISTER(char const* cmd, char const* url, char const* urlSuffix, char const* fullRequestStr, Boolean reuseConnection, Boolean deliverViaTCP, char const* proxyURLSuffix)
{
    pml::Log::Get() << "RTSPServer\t" << "REGISTER: " << fullRequestStr << std::endl;
    RTSPClientConnection::handleCmd_REGISTER(cmd, url, urlSuffix, fullRequestStr, reuseConnection, deliverViaTCP, proxyURLSuffix);
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_bad()
{
    pml::Log::Get(pml::Log::LOG_WARN) << "RTSPServer\t" << "Bad command" << std::endl;
    RTSPClientConnection::handleCmd_bad();
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_notSupported()
{
    pml::Log::Get(pml::Log::LOG_WARN) << "RTSPServer\t" << "Command not supported " << std::endl;
    RTSPClientConnection::handleCmd_notSupported();
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_notFound()
{
    pml::Log::Get(pml::Log::LOG_WARN) << "RTSPServer\t" << "Stream not found " << std::endl;
    RTSPClientConnection::handleCmd_notFound();
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_sessionNotFound()
{
    pml::Log::Get(pml::Log::LOG_WARN) << "RTSPServer\t" << "Session not found " << std::endl;
    RTSPClientConnection::handleCmd_sessionNotFound();
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_unsupportedTransport()
{
    pml::Log::Get(pml::Log::LOG_WARN) << "RTSPServer\t" << "Unsupported transport " << std::endl;
    RTSPClientConnection::handleCmd_unsupportedTransport();
}

PamRTSPServer::PamRTSPClientSession::PamRTSPClientSession(RTSPServer& ourServer, u_int32_t sessionId) :
 RTSPClientSession(ourServer, sessionId)
{
    pml::Log::Get() << "RTSPServer\t" << "New Session" << std::endl;
}

PamRTSPServer::PamRTSPClientSession::~PamRTSPClientSession()
{
}

void PamRTSPServer::PamRTSPClientSession::handleCmd_SETUP(RTSPClientConnection* ourClientConnection, char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr)
{
    pml::Log::Get() << "RTSPServer\t" << "Session\n" << fullRequestStr << std::endl;
    RTSPClientSession::handleCmd_SETUP(ourClientConnection, urlPreSuffix, urlSuffix, fullRequestStr);
}

void PamRTSPServer::PamRTSPClientSession::handleCmd_withinSession(RTSPClientConnection* ourClientConnection, char const* cmdName, char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr)
{
    pml::Log::Get() << "RTSPServer\t" << "Session\n" << fullRequestStr << std::endl;
    RTSPClientSession::handleCmd_withinSession(ourClientConnection, cmdName, urlPreSuffix, urlSuffix, fullRequestStr);
}

void PamRTSPServer::PamRTSPClientSession::handleCmd_TEARDOWN(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession)
{
    RTSPClientSession::handleCmd_TEARDOWN(ourClientConnection, subsession);
}

void PamRTSPServer::PamRTSPClientSession::handleCmd_PLAY(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession, char const* fullRequestStr)
{
    RTSPClientSession::handleCmd_PLAY(ourClientConnection, subsession, fullRequestStr);
}

void PamRTSPServer::PamRTSPClientSession::handleCmd_PAUSE(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession)
{
    RTSPClientSession::handleCmd_PAUSE(ourClientConnection, subsession);
}

void PamRTSPServer::PamRTSPClientSession::handleCmd_GET_PARAMETER(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession, char const* fullRequestStr)
{
    RTSPClientSession::handleCmd_GET_PARAMETER(ourClientConnection, subsession, fullRequestStr);
}

void PamRTSPServer::PamRTSPClientSession::handleCmd_SET_PARAMETER(RTSPClientConnection* ourClientConnection, ServerMediaSubsession* subsession, char const* fullRequestStr)
{
    RTSPClientSession::handleCmd_SET_PARAMETER(ourClientConnection, subsession, fullRequestStr);
}


GenericMediaServer::ClientConnection* PamRTSPServer::createNewClientConnection(int clientSocket, struct sockaddr_in clientAddr)
{

    return new PamRTSPClientConnection(*this, clientSocket, clientAddr);
}

GenericMediaServer::ClientSession* PamRTSPServer::createNewClientSession(u_int32_t sessionId)
{
    return new PamRTSPClientSession(*this, sessionId);
}
