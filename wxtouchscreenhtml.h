#pragma once
#include <wx/html/htmlwin.h>

class wxTouchScreenHtml : public wxHtmlWindow
{
    public:
        wxTouchScreenHtml();

        wxTouchScreenHtml(wxWindow *parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxHW_DEFAULT_STYLE, const wxString& name = wxT("touchScreenHtml"));

        void SetPageTouch(const wxString& sCode);
        void LoadFileTouch(const wxString& sFile);
        void SetScrollLock(bool bLock);

        void Home();
		void End();
		void PageUp();
		void PageDown();

    private:
        void OnMouseDown(wxMouseEvent& event);
        void OnMouseUp(wxMouseEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnPaint(wxPaintEvent& event);

        bool m_bScrollLock;
        bool m_bDown;
        wxPoint m_pntDown;
};
