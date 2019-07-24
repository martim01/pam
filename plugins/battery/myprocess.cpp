#include "myprocess.h"
#include <wx/txtstrm.h>
#include <wx/log.h>
#include <wx/tokenzr.h>


using namespace std;

DEFINE_EVENT_TYPE(wxEVT_PROCESS_FINISHED)

void MyProcess::OnTerminate(int pid, int status)
{
    GetInput();

    if(m_pHandler)
    {
        wxCommandEvent event(wxEVT_PROCESS_FINISHED);
        wxPostEvent(m_pHandler, event);
    }

}


bool MyProcess::GetInput()
{

    if ( IsInputAvailable() )
    {
        wxTextInputStream tis(*GetInputStream());

        Json::Reader jsRead;
        jsRead.parse(std::string(tis.ReadLine().mb_str()), m_jsStatus);
        jsRead.parse(std::string(tis.ReadLine().mb_str()), m_jsCharge);
        m_bErrors = false;
    }

    if ( IsErrorAvailable() )
    {
        wxTextInputStream tis(*GetErrorStream());

        // this assumes that the output is always line buffered
        wxString msg;
        msg << _T(" (stderr): ") << tis.ReadLine();

        wxLogDebug(msg);

    }
    m_bErrors = true;

    return true;
}


wxString MyProcess::GetStatus()
{
    if(m_jsStatus["data"]["battery"].isString())
    {
        return wxString::FromAscii(m_jsStatus["data"]["battery"].asString().c_str());
    }
    return wxT("Unknown");
}

wxString MyProcess::GetPower()
{
    if(m_jsStatus["data"]["powerInput"].isString())
    {
        return wxString::FromAscii(m_jsStatus["data"]["powerInput"].asString().c_str());
    }
    return wxT("Unknown");
}

wxString MyProcess::GetError()
{
    if(m_jsStatus["data"]["error"].isString())
    {
        return wxString::FromAscii(m_jsStatus["data"]["error"].asString().c_str());
    }
    return wxT("Unknown");
}

double MyProcess::GetCharge()
{
    if(m_jsCharge["data"].isInt())
    {
        return m_jsCharge["data"].asInt();
    }
    return 0.0;
}

bool MyProcess::IsProcessOk()
{
    return (m_bErrors == false);
}


