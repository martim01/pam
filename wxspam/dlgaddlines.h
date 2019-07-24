#ifndef DLGADDLINES_H
#define DLGADDLINES_H

#include "../pambase/wmline.h"

//(*Headers(dlgAddLines)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/listbox.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/clrpicker.h>
//*)

#include "../pambase/wmline.h"

class dlgAddLines: public wxDialog
{
	public:

		dlgAddLines(wxWindow* parent, mLines_t& mLines, wxWindowID id=wxID_ANY);
		virtual ~dlgAddLines();

		mLines_t& GetLines()
		{
		    return m_mLines;
		}

		//(*Declarations(dlgAddLines)
		wxStaticText* StaticText10;
		wxCheckBox* m_pcbDraw;
		wxStaticText* StaticText9;
		wxSpinCtrl* m_pspcArrowSize;
		wxChoice* m_pchEndArrow;
		wxButton* btnSave;
		wxStaticText* StaticText2;
		wxButton* btnCancel;
		wxStaticText* StaticText6;
		wxButton* btnUpdate;
		wxButton* btnDelete;
		wxStaticText* StaticText8;
		wxSpinCtrl* m_pspcEndY;
		wxSpinCtrl* m_pspcEndX;
		wxStaticText* StaticText1;
		wmLineWnd* m_pLns;
		wxChoice* m_pchStartArrow;
		wxStaticText* StaticText3;
		wxListBox* m_plbxLines;
		wxButton* btnAdd;
		wxStaticText* StaticText5;
		wxSpinCtrl* m_pspcStartX;
		wxChoice* m_pchPen;
		wxSpinCtrl* m_pspcStartY;
		wxTextCtrl* m_ptxtName;
		wxColourPickerCtrl* m_pcpcColour;
		wxStaticText* StaticText4;
		wxSpinCtrl* m_pspcWidth;
		//*)

	protected:

		//(*Identifiers(dlgAddLines)
		static const long ID_LISTBOX1;
		static const long ID_STATICTEXT3;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL3;
		static const long ID_SPINCTRL4;
		static const long ID_STATICTEXT2;
		static const long ID_SPINCTRL5;
		static const long ID_SPINCTRL6;
		static const long ID_STATICTEXT4;
		static const long ID_COLOURPICKERCTRL1;
		static const long ID_STATICTEXT5;
		static const long ID_SPINCTRL1;
		static const long ID_STATICTEXT6;
		static const long ID_CHOICE1;
		static const long ID_CHECKBOX1;
		static const long ID_STATICTEXT8;
		static const long ID_CHOICE2;
		static const long ID_STATICTEXT9;
		static const long ID_CHOICE3;
		static const long ID_STATICTEXT10;
		static const long ID_SPINCTRL2;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_M_PLNS1;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		//*)

	private:

		//(*Handlers(dlgAddLines)
		void OnbtnAddClick(wxCommandEvent& event);
		void OnbtnUpdateClick(wxCommandEvent& event);
		void OnbtnDeleteClick(wxCommandEvent& event);
		void OnlbxLinesSelect(wxCommandEvent& event);
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		void OntxtNameText(wxCommandEvent& event);
		//*)

		unsigned long DecodeArrow(const wxString& sArrow);

		void SelectArrow(wxChoice* pChoice, int nArrow);

        mLines_t m_mLines;

		DECLARE_EVENT_TABLE()
};

#endif
