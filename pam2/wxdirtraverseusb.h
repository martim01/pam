#include <wx/dir.h>

class wmLabel;
class wmList;

class wxDirTraverseUsb : public wxDirTraverser
{
    public:
        wxDirTraverseUsb(wmLabel* plblProgress, wmList* plstResults);
        wxDirTraverseResult OnFile(const wxString& sFilename) override;
        wxDirTraverseResult OnDir(const wxString& sDirname) override;
    private:
        wmLabel* m_plblProgress;
        wmList* m_plstResults;

};
