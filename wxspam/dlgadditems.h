#ifndef DLGADDITEMS_H
#define DLGADDITEMS_H

#include <list>
#include "../pambase/wmlist.h"

//(*Headers(dlgAddItems)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/clrpicker.h>
#include "../pambase/wmlist.h"
//*)


struct button_wxs
{
    button_wxs(): nEnabled(wmList::wmENABLED), nSlideAllowed(wmList::SLID_NONE){}
    ~button_wxs(){}

    size_t nIndex;
    wxString sText;
    wxColour clrButton;
    wxColour clrSelected;
    wxColour clrPressed;
    wxColour clrFlashing;

    wxColour clrText;
    wxColour clrTextSelected;
    wxColour clrTextPressed;
    wxColour clrTextFlashing;

    short nEnabled;
    short nSlideAllowed;
    wxString sSlide;

};

typedef std::list<button_wxs> lItems_t;

class dlgAddItems: public wxDialog
{
	public:



		dlgAddItems(wxWindow* parent,const lItems_t& lItems, const wxString& sDefaultColours, wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~dlgAddItems();

		lItems_t& GetItems()
		{
		    return m_lstItems;
		}

		//(*Declarations(dlgAddItems)
		wxColourPickerCtrl* m_pcpcNormal;
		wxButton* m_pbtnAdd;
		wxStaticText* StaticText2;
		wxColourPickerCtrl* m_pcpcSelected;
		wxColourPickerCtrl* m_pcpcPressed;
		wmList* m_pLst;
		wxChoice* m_pchArrow;
		wxButton* m_pbtnDelete;
		wxCheckBox* m_pchbxTextSelected;
		wxCheckBox* m_pchbxTextPressed;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxButton* m_pbtnUpdate;
		wxColourPickerCtrl* m_pcpcTextNormal;
		wxButton* m_pbtnCancel;
		wxTextCtrl* m_ptxtSlide;
		wxCheckBox* m_pchbxButtonPressed;
		wxCheckBox* m_pchbxText;
		wxStaticText* StaticText5;
		wxColourPickerCtrl* m_pcpcTextSelected;
		wxChoice* m_pchState;
		wxButton* m_pbtnSave;
		wxTextCtrl* m_ptxtText;
		wxCheckBox* m_pchbxButton;
		wxColourPickerCtrl* m_pcpcTextPressed;
		wxChoice* m_pchDirection;
		wxCheckBox* m_pchbxButtonSelected;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(dlgAddItems)
		static const long ID_M_PLST1;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_STATICTEXT2;
		static const long ID_CHOICE1;
		static const long ID_CHECKBOX2;
		static const long ID_COLOURPICKERCTRL1;
		static const long ID_CHECKBOX3;
		static const long ID_COLOURPICKERCTRL2;
		static const long ID_CHECKBOX4;
		static const long ID_COLOURPICKERCTRL3;
		static const long ID_CHECKBOX5;
		static const long ID_COLOURPICKERCTRL4;
		static const long ID_CHECKBOX6;
		static const long ID_COLOURPICKERCTRL5;
		static const long ID_CHECKBOX7;
		static const long ID_COLOURPICKERCTRL6;
		static const long ID_STATICTEXT3;
		static const long ID_CHOICE2;
		static const long ID_STATICTEXT4;
		static const long ID_TEXTCTRL2;
		static const long ID_STATICTEXT5;
		static const long ID_CHOICE3;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_BUTTON4;
		static const long ID_BUTTON5;
		//*)

	private:

		//(*Handlers(dlgAddItems)
		void OnLstItemSelected(wxCommandEvent& event);
		void OnbtnAddClick(wxCommandEvent& event);
		void OnbtnUpdateClick(wxCommandEvent& event);
		void OnbtnDeleteClick(wxCommandEvent& event);
		void OnchbxButtonClick(wxCommandEvent& event);
		void OnchbxButtonPressedClick(wxCommandEvent& event);
		void OnchbxButtonSelectedClick(wxCommandEvent& event);
		void OnchbxTextClick(wxCommandEvent& event);
		void OnchbxTextPressedClick(wxCommandEvent& event);
		void OnchbxTextSelectedClick(wxCommandEvent& event);
		void OnChoice2Select(wxCommandEvent& event);
		void OnbtnSaveClick(wxCommandEvent& event);
		void OnbtnCancelClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

        lItems_t m_lstItems;
		long m_nSelected;
};

#endif
