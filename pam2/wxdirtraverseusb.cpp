#include "wxdirtraverseusb.h"
#include "wmlist.h"
#include "wmlabel.h"
#include <wx/log.h>

wxDirTraverseUsb::wxDirTraverseUsb(wmLabel* plblProgress, wmList* plstResults) :
    m_plblProgress(plblProgress),
    m_plstResults(plstResults)
{

}

wxDirTraverseResult wxDirTraverseUsb::OnFile(const wxString& sFilename)
{
    m_plstResults->AddButton(sFilename);
    return wxDIR_CONTINUE;
}

wxDirTraverseResult wxDirTraverseUsb::OnDir(const wxString& sDirname)
{
    wxLogDebug(sDirname);
    m_plblProgress->SetLabel(wxString::Format("Scanning '%s'", sDirname.c_str()));
    m_plblProgress->Update();
    return wxDIR_CONTINUE;
}
