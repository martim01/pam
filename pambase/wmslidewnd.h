#pragma once
#include "dlldefine.h"
#include <wx/stattext.h>
#include <wx/dialog.h>
#include "wmlabel.h"


/** @brief Class that does the work of showing sliding buttons in wmList etc. The user should pass it a bmp a pos and a size and it will display them.
**/
class PAMBASE_IMPEXPORT wmSlideWnd: public wxDialog
{
	public:
        /** @brief Constructor
        *   @param bmp the bitmap to display - this would usually be a "screen grab" of the bit of screen you want to slide
        *   @param nPos the position to draw the bitmap
        *   @param nSize the size of the bitmap
        **/
		wmSlideWnd(wxBitmap bmp, wxPoint nPos, wxSize nSize);

		///< @brief Destructor
		virtual ~wmSlideWnd(){}

        /** @brief Sets the bitmap
        **  @param bmp
        **/
        void SetBitmap(wxBitmap bmp);

    protected:
		wmLabel* m_pLabel;

};

