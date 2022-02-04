#ifndef DLGMASK_H
#define DLGMASK_H

//(*Headers(dlgMask)
#include <wx/dialog.h>
#include "wmlist.h"
//*)
#include <vector>

class dlgMask: public wxDialog
{
	public:

		dlgMask(wxWindow* parent, const wxArrayString& asButtons, const wxString& sSelected, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		dlgMask(wxWindow* parent, const std::vector<wxString>& vButtons, const wxString& sSelected, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgMask();

		//(*Declarations(dlgMask)
		wmList* m_plstSubnet;
		//*)

		wxString m_sSelected;

	protected:

		//(*Identifiers(dlgMask)
		static const long ID_M_PLST1;
		//*)

		void OnActivate(wxActivateEvent& event);
	private:

		//(*Handlers(dlgMask)
		void OnlstSubnetSelected(wxCommandEvent& event);
		void OnLeftDown(wxMouseEvent& event);
		//*)

		void OnCaptureLost(wxMouseCaptureLostEvent& event);
		wxPoint m_pntWindow;


		DECLARE_EVENT_TABLE()
};

#endif
