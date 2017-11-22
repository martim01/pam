#include "wmslidewnd.h"
#include "wmlabel.h"


wmSlideWnd::wmSlideWnd(wxBitmap bmp, wxPoint nPos, wxSize nSize)
{
    Create(NULL, wxNewId(), wxEmptyString, nPos, nSize, wxNO_BORDER, _T("id"));
    //SetBackgroundColour(*wxBLACK);
    m_pLabel = new wmLabel(this, wxNewId(), bmp, wxPoint(0,0), nSize);
    //m_pLabel->SetBackgroundColour(*wxWHITE);
    m_pLabel->SetBorderState(uiRect::BORDER_NONE);
}

void wmSlideWnd::SetBitmap(wxBitmap bmp)
{
    m_pLabel->SetBitmap(bmp);
}
