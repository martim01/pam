#include "updater.h"
#include <wx/wfstream.h>
#include <wx/tarstrm.h>
#include <wx/string.h>
#include "log.h"
#include "usbchecker.h"


#ifdef __WXGNU__
#include <sys/capability.h>
#include <sys/prctl.h>
#include <sys/stat.h>
bool InheritCapabilities()
{
    cap_t caps = cap_get_proc();
    if(caps == nullptr)
    {
        pmlLog(pml::LOG_ERROR) << "Failed to load cap" << std::endl;
        return false;
    }
    pmlLog(pml::LOG_TRACE) << "Loaded caps = " << cap_to_text(caps, nullptr);

    cap_value_t cap_list[1];
    cap_list[0] = CAP_SYS_ADMIN;
    if(cap_set_flag(caps, CAP_INHERITABLE, 1, cap_list, CAP_SET) == -1)
    {
        pmlLog(pml::LOG_ERROR) << "Failed to set inheritable " << strerror(errno) << std::endl;
        return false;
    }

    pmlLog(pml::LOG_TRACE) << "Loaded caps2 = " << cap_to_text(caps, nullptr) << std::endl;

    if(cap_set_proc(caps) == -1)
    {
        pmlLog(pml::LOG_ERROR) << "Failed to set proc " << strerror(errno) << std::endl;
        return false;
    }

    caps = cap_get_proc();
    if(caps == nullptr)
    {
        pmlLog(pml::LOG_ERROR) << "Failed to load cap" << std::endl;
        return false;
    }

    pmlLog(pml::LOG_TRACE) << "Loaded caps3 = " << cap_to_text(caps, nullptr) << std::endl;

    if(prctl(PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, CAP_SYS_ADMIN,0,0) == -1)
    {
        pmlLog(pml::LOG_ERROR) << "Failed to rasie cap" << std::endl;
        return false;
    }

    return true;
}


void ExtractAndRunUpdater(const wxString& sDevice, const wxString& sFile)
{
    pmlLog(pml::LOG_DEBUG) << "ExtractAndRunUpdater: " << sFile;

    wxRemoveFile("/tmp/pamupdatemanager");

    wxFileInputStream in("/mnt/share/"+sFile);
    wxTarInputStream tar(in);
    wxTarEntry* pEntry = nullptr;
    bool bExtracted(false);
    do
    {
        pEntry = tar.GetNextEntry();
        if(pEntry)
        {
            pmlLog(pml::LOG_TRACE) << pEntry->GetName();
            if(pEntry->GetName().CmpNoCase("pamupdatemanager") == 0)
            {
                pmlLog(pml::LOG_DEBUG) << "Extract pamupdatemanager";
                bExtracted = true;
                wxFileOutputStream out("/tmp/pamupdatemanager");
                while(tar.IsOk() && !tar.Eof())
                {
                    tar.Read(out);
                }
                break;
            }
        }
    }while(pEntry);

    if(bExtracted)
    {
        chmod("/tmp/pamupdatemanager", S_IXUSR | S_IRUSR | S_IWUSR);
    
        InheritCapabilities();
        pmlLog(pml::LOG_DEBUG) << "Run Updater";
        wxExecute("/tmp/pamupdatemanager "+sDevice+" "+sFile);    
    }
    else
    {
        pmlLog(pml::LOG_ERROR) << "Could not find pamupdatemanager in 'put' file";
    }
}

#endif