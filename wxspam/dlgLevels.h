#ifndef DLGLEVELS_H
#define DLGLEVELS_H

//(*Headers(dlgLevels)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)
#include <wx/xml/xml.h>

struct levelDetails
{
    wxString sTitle;
    wxString sUnits;
    wxString sOffset;
    wxString sScaling;
    wxString sOverMod;
    wxString sReference;

    wxArrayString asLevels;
};


class dlgLevels: public wxDialog
{
	public:

		dlgLevels(wxWindow* parent,const levelDetails& details, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgLevels();

		const levelDetails& GetDetails()
		{
		    return m_details;
		}

		//(*Declarations(dlgLevels)
		wxTextCtrl* m_txtOverMod;
		wxStaticText* StaticText2;
		wxButton* btnCancel;
		wxStaticText* StaticText6;
		wxTextCtrl* m_txtOffset;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* btnOK;
		wxTextCtrl* m_txtLevels;
		wxTextCtrl* m_txtScaling;
		wxStaticText* StaticText5;
		wxTextCtrl* m_txtTitle;
		wxTextCtrl* m_txtUnits;
		wxTextCtrl* m_txtReference;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(dlgLevels)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_STATICTEXT5;
		static const long ID_TEXTCTRL5;
		static const long ID_STATICTEXT6;
		static const long ID_TEXTCTRL7;
		static const long ID_TEXTCTRL6;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(dlgLevels)
		void OnbtnCancelClick(wxCommandEvent& event);
		void OnbtnOKClick(wxCommandEvent& event);
		//*)

		levelDetails m_details;

		DECLARE_EVENT_TABLE()
};

#endif
