#ifndef DLGWIZARD_H
#define DLGWIZARD_H

//(*Headers(dlgWizard)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checklst.h>
#include <wx/filepicker.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class dlgWizard: public wxDialog
{
	public:

		dlgWizard(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgWizard();

		//(*Declarations(dlgWizard)
		wxTextCtrl* m_ptxtEmail;
		wxTextCtrl* m_ptxtDescription;
		wxStaticText* StaticText2;
		wxDirPickerCtrl* m_pdpcPath;
		wxCheckListBox* m_pchlbxLibs;
		wxTextCtrl* m_ptxtAuthor;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* m_pbtnCancel;
		wxChoice* m_pchType;
		wxButton* m_pbtnCreate;
		wxTextCtrl* m_ptxtName;
		wxChoice* m_pchWindow;
		wxStaticText* StaticText4;
		//*)

		wxString GetNewProjectPath()
		{
		    return m_sProjectPath;
		}
	protected:

		//(*Identifiers(dlgWizard)
		static const long ID_CHOICE1;
		static const long ID_CHOICE2;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL3;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL4;
		static const long ID_CHECKLISTBOX1;
		static const long ID_DIRPICKERCTRL1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(dlgWizard)
		void OnbtnCreateClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OntxtNameText(wxCommandEvent& event);
		//*)

		void CreateFiles(const wxArrayString& asFiles, const wxString& sSub=wxEmptyString);

        wxString m_sProjectPath;

        static const wxString STR_TYPE[2];

		DECLARE_EVENT_TABLE()
};

#endif
