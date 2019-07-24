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
    m_bErrors = false;
}


bool MyProcess::GetInput()
{

    if ( IsInputAvailable() )
    {
        wxTextInputStream tis(*GetInputStream());

        Json::Reader jsRead;

        wxString sLine(tis.ReadLine());
        sLine.Replace("'","\"");
        sLine.MakeLower();
        m_sRaw << sLine;


        if(jsRead.parse(std::string(sLine.mb_str()), m_jsStatus) == false)
        {
            m_sRaw << "\n" << wxString::FromAscii(jsRead.getFormatedErrorMessages().c_str());
        }

        sLine = tis.ReadLine();
        sLine.Replace("'","\"");
        sLine.MakeLower();
        m_sRaw << sLine;


        if(jsRead.parse(std::string(sLine.mb_str()), m_jsCharge) == false)
        {
            m_sRaw << "\n" << wxString::FromAscii(jsRead.getFormatedErrorMessages().c_str());
        }
        m_bErrors = false;
    }

    else if ( IsErrorAvailable() )
    {
        wxTextInputStream tis(*GetErrorStream());

        // this assumes that the output is always line buffered
        wxString msg;

        m_jsStatus["error"] = std::string(tis.ReadLine().mb_str());

        wxLogDebug(msg);
        m_bErrors = true;
    }


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
    if(m_jsStatus["data"]["powerinput"].isString())
    {
        return wxString::FromAscii(m_jsStatus["data"]["powerinput"].asString().c_str());
    }
    return wxT("Unknown");
}

wxString MyProcess::GetError()
{
    if(m_jsStatus["error"].isString())
    {
        return wxString::FromAscii(m_jsStatus["error"].asString().c_str());
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


