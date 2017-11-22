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
    bool hasInput = false;

    if ( IsInputAvailable() )
    {
        wxTextInputStream tis(*GetInputStream());

        wxString sInput = tis.ReadLine();

        wxArrayString asLines = wxStringTokenize(sInput, wxT("|"));
        for(int i = 0; i < asLines.GetCount(); i++)
        {
            m_mResult.insert(make_pair(asLines[i].BeforeFirst(wxT('=')), asLines[i].AfterFirst(wxT('='))));
        }


        hasInput = true;
    }

    if ( IsErrorAvailable() )
    {
        wxTextInputStream tis(*GetErrorStream());

        // this assumes that the output is always line buffered
        wxString msg;
        msg << m_sCmd << _T(" (stderr): ") << tis.ReadLine();

        wxLogMessage(msg);

        hasInput = true;
    }

    return hasInput;
}


wxString MyProcess::GetResultAttribute(const wxString& sAttribute)
{
    map<wxString, wxString>::iterator itResult = m_mResult.find(sAttribute);
    if(itResult != m_mResult.end())
    {
        return itResult->second;
    }
    return wxEmptyString;
}
