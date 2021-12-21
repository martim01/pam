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
