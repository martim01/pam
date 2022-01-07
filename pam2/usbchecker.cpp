#include "usbchecker.h"
#include <thread>
#include <wx/dir.h>
#include <wx/log.h>
#ifdef __WXGNU__
#include <sys/mount.h>
#endif // __WXGNU__
#include <wx/msgdlg.h>
#include <fstream>
#include <iostream>
#include "log.h"

wxDEFINE_EVENT(wxEVT_USB_FOUND, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_USB_FILE_FOUND, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_USB_FINISHED, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_USB_ERROR, wxCommandEvent);

UsbChecker::~UsbChecker()
{
    Abort();
}

void UsbChecker::Abort()
{
    if(m_pThread)
    {
        m_pThread->join();
        m_pThread = nullptr;
    }
}

void UsbChecker::SaveToUSB(const wxFileName& fnSource)
{
    pmlLog(pml::LOG_DEBUG) << "USB\tSaveToUsb: " << fnSource.GetFullPath();
    if(m_pThread != nullptr)
    {
        Abort();
    }

    m_pThread = std::make_unique<std::thread>([this, fnSource]()
    {
        wxArrayString asFiles;
        #ifdef __WXGNU__
        //look through /dev/disk/by-id for drives starting usb-
        wxDir::GetAllFiles("/dev/disk/by-id", &asFiles, "usb*");
        #endif // __WXGNU__
        if(asFiles.GetCount() == 0)
        {
            pmlLog(pml::LOG_WARN) << "USB\tNot able to save " << fnSource.GetName() << " no USB drives detected.";
        }
        else
        {
            for(size_t i = 0; i < asFiles.GetCount(); i++)
            {
                if(asFiles[i].Find("part") != wxNOT_FOUND)
                {
                    if(MountDevice(asFiles[i]) == 0)
                    {
                        std::ifstream source(fnSource.GetFullPath().ToStdString(), std::ios::binary);
                        std::ofstream dest(wxString("/mnt/share/"+fnSource.GetFullName()).ToStdString(), std::ios::binary);
                        if(source.is_open() == false)
                        {
                            pmlLog(pml::LOG_WARN) << "USB\tCould not open source file";
                        }
                        else if(dest.is_open() == false)
                        {
                            pmlLog(pml::LOG_WARN) << "USB\tCould not open destination file";
                        }
                        else
                        {
                            dest << source.rdbuf();
                            pmlLog() << "USB\t" << fnSource.GetName().ToStdString() << " saved to USB drive";
                            source.close();
                            remove(fnSource.GetFullPath().ToStdString().c_str());
                        }
                        UnmountDevice();
                        break;
                    }
                }
            }
        }
    });
}

int UsbChecker::UnmountDevice()
{
    int nResult = umount("/mnt/share");
    if(nResult == -1 && errno != EAGAIN && errno != EINVAL)
    {
        return errno;
    }
    return 0;
}


int UsbChecker::MountDevice(const wxString& sDevice)
{
    if(sDevice.empty())
    {
        pmlLog(pml::LOG_WARN) << "USB\tTrying to mount an emtpy device";
        return EINVAL;
    }

    if(wxDirExists("/mnt/share") == false)
    {
        wxMkdir("/mnt/share");
    }
    int nResult = umount("/mnt/share");
    if(nResult == -1 && errno != EAGAIN && errno != EINVAL)
    {
        pmlLog(pml::LOG_WARN) << "USB\tFailed to umount /mnt/share: " << strerror(errno);
        return errno;
    }


    std::array<std::string, 8> fs({"ext3", "ext2", "ext4", "vfat", "msdos", "f2fs", "fuseblk", "ntfs"});
    for(size_t i = 0; i < fs.size(); i++)
    {
        nResult = mount(sDevice.ToStdString().c_str(), "/mnt/share", fs[i].c_str(), MS_SILENT, nullptr);
        if(nResult == 0)
        {
            return 0;
        }
    }
    pmlLog(pml::LOG_WARN) << "USB\tFailed to mount " << sDevice.ToStdString() << ": " << strerror(errno);
    return errno;

}


void UsbChecker::RunCheck(const wxString& sFilename)
{
    if(m_pThread != nullptr)
    {
        Abort();
    }

    m_pThread = std::make_unique<std::thread>([this, sFilename]()
    {
        wxArrayString asFiles;
        //look through /dev/disk/by-id for drives starting usb-
        wxDir::GetAllFiles("/dev/disk/by-id", &asFiles, "usb*");
        //if have same name only try to mount -part1

        for(size_t i = 0; i < asFiles.GetCount(); i++)
        {
            if(asFiles[i].Find("part") != wxNOT_FOUND)
            {
                wxString sDebug(asFiles[i]);
                wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_USB_FOUND);
                pEvent->SetString(asFiles[i]);
                wxQueueEvent(m_pHandler, pEvent);
                MountAndSearch(sDebug, sFilename);
            }
        }
        wxCommandEvent* pEventFinished = new wxCommandEvent(wxEVT_USB_FINISHED);
        wxQueueEvent(m_pHandler, pEventFinished);
    });
}

void UsbChecker::MountAndSearch(const wxString& sDevice, const wxString& sFilename)
{
    int nError = MountDevice(sDevice);
    if(nError != 0)
    {
        wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_USB_ERROR);
        pEvent->SetInt(nError);
        pEvent->SetString(wxString::Format("Mount: %s", wxString::FromUTF8(strerror(errno)).c_str()));

        wxQueueEvent(m_pHandler, pEvent);
        return;
    }

    wxString sPath = "/mnt/share";

    wxArrayString asFiles;
    wxDir::GetAllFiles("/mnt/share", &asFiles, sFilename);
    wxCommandEvent* pEvent = new wxCommandEvent(wxEVT_USB_FILE_FOUND);
    wxString sEvent = sDevice+"=";
    for(size_t i = 0; i < asFiles.size(); i++)
    {
        if(i != 0)
        {
            sEvent += ",";
        }
        sEvent += asFiles[i];

    }
    wxQueueEvent(m_pHandler, pEvent);
    umount("/mnt/share");
}
