#include "pamRTSPServer.h"
#include "log.h"

PamRTSPServer* PamRTSPServer::createNew(UsageEnvironment& env, Port ourPort,UserAuthenticationDatabase* authDatabase, unsigned reclamationSeconds)
{
    int ourSocketIPv4 = setUpOurSocket(env, ourPort, AF_INET);
    int ourSocketIPv6 = setUpOurSocket(env, ourPort, AF_INET6);
    if (ourSocketIPv4 < 0 && ourSocketIPv6 < 0)
    {
        return NULL;
    }
    return new PamRTSPServer(env, ourSocketIPv4, ourSocketIPv6, ourPort, authDatabase, reclamationSeconds);
}

PamRTSPServer::PamRTSPServer(UsageEnvironment& env,  int ourSocketIPv4, int ourSocketIPv6,  Port ourPort, UserAuthenticationDatabase* authDatabase, unsigned reclamationSeconds) :
RTSPServer(env, ourSocketIPv4, ourSocketIPv6, ourPort, authDatabase, reclamationSeconds)
{
}

PamRTSPServer::~PamRTSPServer()
{
}

void PamRTSPServer::PamRTSPClientConnection::handleRequestBytes(int newBytesRead)
{
    RTSPClientConnection::handleRequestBytes(newBytesRead);
}

PamRTSPServer::PamRTSPClientConnection::PamRTSPClientConnection(RTSPServer& ourServer, int clientSocket, struct sockaddr_storage clientAddr) :
RTSPServer::RTSPClientConnection(ourServer, clientSocket, clientAddr)
{
    //@todo put in client address and port number

//    pmlLog() << "RTSPServer\t" << "New Connection: " << inet_ntoa(clientAddr.sin_addr); //@todo fix ip address
}
PamRTSPServer::PamRTSPClientConnection::~PamRTSPClientConnection()
{}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_OPTIONS()
{
    pmlLog() << "RTSPServer\t" << "OPTIONS";
    RTSPClientConnection::handleCmd_OPTIONS();
}
void PamRTSPServer::PamRTSPClientConnection::handleCmd_GET_PARAMETER(char const* fullRequestStr)
{
    pmlLog() << "RTSPServer\t" << "GET_PARAMETER: " << fullRequestStr;
    RTSPClientConnection::handleCmd_GET_PARAMETER(fullRequestStr);
}


void PamRTSPServer::PamRTSPClientConnection::handleCmd_SET_PARAMETER(char const* fullRequestStr)
{
    pmlLog() << "RTSPServer\t" << "SET_PARAMETER: " << fullRequestStr;
    RTSPClientConnection::handleCmd_SET_PARAMETER(fullRequestStr); // when operating on the entire server
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_DESCRIBE(char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr)
{
    pmlLog() << "RTSPServer\t" << "DESCRIBE: " << fullRequestStr;
    RTSPClientConnection::handleCmd_DESCRIBE(urlPreSuffix, urlSuffix, fullRequestStr);
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_REGISTER(char const* cmd, char const* url, char const* urlSuffix, char const* fullRequestStr, Boolean reuseConnection, Boolean deliverViaTCP, char const* proxyURLSuffix)
{
    pmlLog() << "RTSPServer\t" << "REGISTER: " << fullRequestStr;
    RTSPClientConnection::handleCmd_REGISTER(cmd, url, urlSuffix, fullRequestStr, reuseConnection, deliverViaTCP, proxyURLSuffix);
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_bad()
{
    pmlLog(pml::LOG_WARN) << "RTSPServer\t" << "Bad command";
    RTSPClientConnection::handleCmd_bad();
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_notSupported()
{
    pmlLog(pml::LOG_WARN) << "RTSPServer\t" << "Command not supported ";
    RTSPClientConnection::handleCmd_notSupported();
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_notFound()
{
    pmlLog(pml::LOG_WARN) << "RTSPServer\t" << "Stream not found ";
    RTSPClientConnection::handleCmd_notFound();
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_sessionNotFound()
{
    pmlLog(pml::LOG_WARN) << "RTSPServer\t" << "Session not found ";
    RTSPClientConnection::handleCmd_sessionNotFound();
}

void PamRTSPServer::PamRTSPClientConnection::handleCmd_unsupportedTransport()
{
    pmlLog(pml::LOG_WARN) << "RTSPServer\t" << "Unsupported transport ";
    RTSPClientConnection::handleCmd_unsupportedTransport();
}

PamRTSPServer::PamRTSPClientSession::PamRTSPClientSession(RTSPServer& ourServer, u_int32_t sessionId) :
 RTSPClientSession(ourServer, sessionId)
{
    pmlLog() << "RTSPServer\t" << "New Session";
}

PamRTSPServer::PamRTSPClientSession::~PamRTSPClientSession()
{
}

void PamRTSPServer::PamRTSPClientSession::handleCmd_SETUP(RTSPClientConnection* ourClientConnection, char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr)
{
    pmlLog() << "RTSPServer\t" << "Session\n" << fullRequestStr;
    RTSPClientSession::handleCmd_SETUP(ourClientConnection, urlPreSuffix, urlSuffix, fullRequestStr);
}

void PamRTSPServer::PamRTSPClientSession::handleCmd_withinSession(RTSPClientConnection* ourClientConnection, char const* cmdName, char const* urlPreSuffix, char const* urlSuffix, char const* fullRequestStr)
{
    pmlLog() << "RTSPServer\t" << "Session\n" << fullRequestStr;
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


GenericMediaServer::ClientConnection* PamRTSPServer::createNewClientConnection(int clientSocket, struct sockaddr_storage clientAddr)
{

    return new PamRTSPClientConnection(*this, clientSocket, clientAddr);
}

GenericMediaServer::ClientSession* PamRTSPServer::createNewClientSession(u_int32_t sessionId)
{
    return new PamRTSPClientSession(*this, sessionId);
}
