#pragma once
#include <wx/event.h>
#include <thread>
#include <memory>
#include <wx/filename.h>
#include <wx/dir.h>
#include <list>
class UsbChecker
{
    public:
        UsbChecker(wxEvtHandler* pHandler) :m_pHandler(pHandler), m_pThread(nullptr){}
        ~UsbChecker();

        void RunCheck(const wxString& sFilename);
        void SaveToUSB(const std::list<wxFileName>& lstFn, bool bRemove);

        static int MountDevice(const wxString& sDevice);
        static int UnmountDevice();

    private:
        void Abort();
        void MountAndSearch(const wxString& sDevice, const wxString& sFilename);
        wxEvtHandler* m_pHandler;
        std::unique_ptr<std::thread> m_pThread;


};

wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_USB_FOUND, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_USB_FILE_FOUND, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_USB_FINISHED, wxCommandEvent);
wxDECLARE_EXPORTED_EVENT(WXEXPORT, wxEVT_USB_ERROR, wxCommandEvent);

class wxDirTraverserSimple : public wxDirTraverser
{
    public:
        wxDirTraverserSimple(wxEvtHandler* pHandler, const wxString& sDevice) : m_pHandler(pHandler), m_sDevice(sDevice){}
        wxDirTraverseResult OnFile(const wxString& sFilename) override
        {
            wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_USB_FILE_FOUND);
            pEvent->SetString(m_sDevice+"="+sFilename.Mid(11));
            wxQueueEvent(m_pHandler, pEvent);
            return wxDIR_CONTINUE;
        }

        wxDirTraverseResult OnDir(const wxString& sDirname) override
        {
            return wxDIR_CONTINUE;
        }
    private:
        wxEvtHandler* m_pHandler;
        wxString m_sDevice;

};



