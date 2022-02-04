#include "pnlTests.h"
#include "testpluginbuilder.h"
#include "testpluginfactory.h"
#include "settings.h"
#include <wx/log.h>

using namespace std;

//(*InternalHeaders(pnlTests)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(pnlTests)
const long pnlTests::ID_M_PSWP1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(pnlTests,wxPanel)
	//(*EventTable(pnlTests)
	//*)
END_EVENT_TABLE()

pnlTests::pnlTests(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(pnlTests)
	Create(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("id"));
	m_pswpTests = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,0), wxSize(600,481), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpTests->SetPageNameStyle(0);

	Connect(ID_M_PSWP1,wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED,(wxObjectEventFunction)&pnlTests::OnswpTestsPageChanged);
	//*)
	m_pSelectedTest = 0;
}

pnlTests::~pnlTests()
{
	//(*Destroy(pnlTests)
	//*)
}


void pnlTests::LoadTestPanels()
{
    TestPluginFactory::Get()->SetHandler(this);
    TestPluginFactory::Get()->SetSwitcherPanels(m_pswpTests);


    map<wxString, wxString>::const_iterator itBegin, itEnd;
    if(Settings::Get().GetSectionDataBegin(wxT("Test Plugins"), itBegin) && Settings::Get().GetSectionDataEnd(wxT("Test Plugins"), itEnd))
    {
        for(; itBegin != itEnd; ++itBegin)
        {
            TestPluginFactory::Get()->LoadTestLibrary(itBegin->second);
        }
    }
    //m_pswpTests->ChangeSelection(Settings
}

void pnlTests::ShowTestPanel(const wxString& sPanel)
{
    m_pswpTests->ChangeSelection(sPanel);


    map<wxString, TestPluginBuilder*>::const_iterator itTest = TestPluginFactory::Get()->FindPlugin(m_pswpTests->GetSelectionName());
    if(itTest != TestPluginFactory::Get()->GetPluginEnd())
    {
        m_pSelectedTest = itTest->second;
    }

}


void pnlTests::SetAudioData(const timedbuffer* pBuffer)
{
    if(m_pSelectedTest)
    {
        m_pSelectedTest->SetAudioData(pBuffer);
    }
}

void pnlTests::OnswpTestsPageChanged(wxNotebookEvent& event)
{

}
