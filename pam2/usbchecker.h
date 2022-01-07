#pragma once
#include <wx/event.h>
#include <thread>
#include <memory>
#include <wx/filename.h>

class UsbChecker
{
    public:
        UsbChecker(wxEvtHandler* pHandler) :m_pHandler(pHandler), m_pThread(nullptr){}
        ~UsbChecker();

        void RunCheck(const wxString& sFilename);
        void SaveToUSB(const wxFileName& fnSource);

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

