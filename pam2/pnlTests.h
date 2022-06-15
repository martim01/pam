#ifndef PNLTESTS_H
#define PNLTESTS_H

#include <vector>

//(*Headers(pnlTests)
#include <wx/notebook.h>
#include "wmswitcherpanel.h"
#include <wx/panel.h>
//*)

struct session;
class timedbuffer;
class TestPluginBuilder;

class pnlTests: public wxPanel
{
	public:

		pnlTests(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~pnlTests();

		//(*Declarations(pnlTests)
		wmSwitcherPanel* m_pswpTests;
		//*)

		void LoadTestPanels();
        void ShowTestPanel(const wxString& sPanel);
		void SetAudioData(const timedbuffer* pBuffer);

		TestPluginBuilder* GetSelectedTest() { return m_pSelectedTest;}
	protected:

		//(*Identifiers(pnlTests)
		static const long ID_M_PSWP1;
		//*)

	private:

		//(*Handlers(pnlTests)
		void OnswpTestsPageChanged(wxNotebookEvent& event);
		//*)

		void OnSize(wxSizeEvent& event);

		TestPluginBuilder* m_pSelectedTest;


		DECLARE_EVENT_TABLE()
};

#endif
