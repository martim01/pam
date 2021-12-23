#pragma once
#include <wx/event.h>
#include <thread>
#include <memory>
#include <wx/filename.h>

class UsbChecker
{
    public:
        UsbChecker() : m_pThread(nullptr){}
        ~UsbChecker();

        void SaveToUSB(const wxFileName& fnSource);

        static int MountDevice(const wxString& sDevice);
        static int UnmountDevice();

    private:
        void Abort();
        void MountAndSearch(const wxString& sDevice, const wxString& sFilename);
        wxEvtHandler* m_pHandler;
        std::unique_ptr<std::thread> m_pThread;


};


