#ifndef PNLCHANNELMAPPING_H
#define PNLCHANNELMAPPING_H

//(*Headers(pnlChannelMapping)
#include "wmbutton.h"
#include "wmlabel.h"
#include <wx/panel.h>
//*)
#include <map>

class SettingEvent;
class pnlChannelMapping: public wxPanel
{
	public:

		pnlChannelMapping(wxWindow* parent, const wxString& sSection, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize, long nStyle=0, wxString sNotused=wxEmptyString);
		virtual ~pnlChannelMapping();

		wmButton* m_pbtnBack;
		wmButton* m_pbtnCh[8];
		wmLabel* m_pLbl[8];
		wmLabel* m_plblCh[8];
		//*)

	protected:

		//(*Identifiers(pnlChannelMapping)
		static const long ID_M_PLBL14;
		static const long ID_M_PLBL15;
		static const long ID_M_PLBL16;
		static const long ID_M_PLBL17;
		static const long ID_M_PLBL18;
		static const long ID_M_PLBL19;
		static const long ID_M_PLBL20;
		static const long ID_M_PLBL21;
		static const long ID_M_PBTN14;
		static const long ID_M_PBTN15;
		static const long ID_M_PBTN16;
		static const long ID_M_PBTN17;
		static const long ID_M_PBTN18;
		static const long ID_M_PBTN19;
		static const long ID_M_PBTN20;
		static const long ID_M_PBTN21;
		static const long ID_M_PLBL22;
		static const long ID_M_PLBL23;
		static const long ID_M_PLBL24;
		static const long ID_M_PLBL25;
		static const long ID_M_PLBL26;
		static const long ID_M_PLBL27;
		static const long ID_M_PLBL28;
		static const long ID_M_PLBL29;
		static const long ID_M_PBTN22;
		//*)

	private:

		//(*Handlers(pnlChannelMapping)
		//*)

		void OnSettingChanged(SettingEvent& event);

        void OnChannelClicked(wxCommandEvent& event);
        void ShowButton(int nButton);
        void HideButtons(int nButton, int nCount);

        void SaveSettings();

        void ShowMapping(const wxString& sMapping);
        void ShowChannelLabels();


        void ShowChannels();
        wxString m_sSection;
        unsigned int m_nChannels;

        static const std::multimap<unsigned int, wxString> MAPPING;

		DECLARE_EVENT_TABLE()
};

#endif
