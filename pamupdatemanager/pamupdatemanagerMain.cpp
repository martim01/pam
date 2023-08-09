#include "pamupdatemanagerMain.h"
#include "pamupdatemanager_version.h"
#include "settings.h"
#include <wx/log.h>
#include <wx/wfstream.h>
#include <wx/tarstrm.h>
#include "wmlistadv.h"
#include <wx/dcclient.h>
#include <wx/txtstrm.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include "releaseelement.h"
#ifdef __WXGNU__
#include <sys/mount.h>
#endif // __WXGNU__
#include <array>
#include <string>
#include <wx/stdpaths.h>

int IsMounted()
{
    wxTextFile tf("/proc/self/mountinfo");
    if(tf.Open())
    {
        for(size_t i = 0; i < tf.GetLineCount(); i++)
        {
            if(tf[i].Find("/mnt/share") != wxNOT_FOUND)
            {
                return 0;
            }
        }
        return -1;
    }
    return -2;
}

int UnmountDevice()
{
    int nResult = umount("/mnt/share");
    if(nResult == -1 && errno != EAGAIN && errno != EINVAL)
    {
        return errno;
    }
    return 0;
}


int MountDevice(const wxString& sDevice)
{
    if(sDevice.empty())
    {
        return EINVAL;
    }

    if(wxDirExists("/mnt/share") == false)
    {
        wxMkdir("/mnt/share");
    }
    int nResult = umount("/mnt/share");
    if(nResult == -1 && errno != EAGAIN && errno != EINVAL)
    {
        return errno;
    }


    auto fs = std::array<std::string, 8>({"ext3", "ext2", "ext4", "vfat", "msdos", "f2fs", "fuseblk", "ntfs"});
    for(size_t i = 0; i < fs.size(); i++)
    {
        nResult = mount(sDevice.ToStdString().c_str(), "/mnt/share", fs[i].c_str(), MS_SILENT, nullptr);
        if(nResult == 0)
        {
            return 0;
        }
    }
    return errno;

}




const long pamupdatemanagerDialog::ID_M_PLBL2 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL3 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL2 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PEDT1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PKBD1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PLBL1 = wxNewId();
const long pamupdatemanagerDialog::ID_PANEL3 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PSWP1 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN4 = wxNewId();
const long pamupdatemanagerDialog::ID_M_PBTN1 = wxNewId();
const long pamupdatemanagerDialog::ID_TIMER2 = wxNewId();

//BEGIN_EVENT_TABLE(pamupdatemanagerDialog,wxDialog)
	//(*EventTable(pamupdatemanagerDialog)
	//*)
//END_EVENT_TABLE()

pamupdatemanagerDialog::pamupdatemanagerDialog(wxWindow* parent,const wxString& sDevice, const wxFileName& fnUpdate, wxWindowID id,const wxPoint& pos,const wxSize& size) : m_sDevice(sDevice), m_fnUpdate(fnUpdate)
{
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxNO_BORDER, _T("id"));
	SetClientSize(wxSize(800,480));
	Move(wxDefaultPosition);
	SetBackgroundColour(wxColour(255,255,255));
	wxFont thisFont(12,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	SetFont(thisFont);
	m_plblTitle = new wmLabel(this, ID_M_PLBL2, _("PAM Update Manager"), wxPoint(0,0), wxSize(800,40), 0, _T("ID_M_PLBL2"));
	m_plblTitle->SetBorderState(uiRect::BORDER_NONE);
	m_plblTitle->GetUiRect().SetGradient(0);
	m_plblTitle->SetForegroundColour(wxColour(255,255,255));
	m_plblTitle->SetBackgroundColour(wxColour(45,36,81));
	wxFont m_plblTitleFont(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Verdana"),wxFONTENCODING_DEFAULT);
	m_plblTitle->SetFont(m_plblTitleFont);
	m_pswpMain = new wmSwitcherPanel(this, ID_M_PSWP1, wxPoint(0,40), wxSize(800,400), wmSwitcherPanel::STYLE_NOSWIPE|wmSwitcherPanel::STYLE_NOANIMATION, _T("ID_M_PSWP1"));
	m_pswpMain->SetPageNameStyle(0);
	m_ppnlProgress = new wxPanel(m_pswpMain, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
	m_ppnlRelease = new wxPanel(m_pswpMain, ID_PANEL2, wxDefaultPosition, wxSize(0,0), wxTAB_TRAVERSAL, _T("ID_PANEL2"));
	m_pLbl2 = new wmLabel(m_ppnlRelease, ID_M_PLBL3, _("Release Notes"), wxPoint(0,5), wxSize(800,35), 0, _T("ID_M_PLBL3"));
	m_pLbl2->SetBorderState(uiRect::BORDER_NONE);
	m_pLbl2->GetUiRect().SetGradient(0);
	m_pLbl2->SetForegroundColour(wxColour(255,255,255));
	m_pLbl2->SetBackgroundColour(wxColour(0,0,0));
	wxFont m_pLbl2Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLbl2->SetFont(m_pLbl2Font);
	m_ppnlPassword = new wxPanel(m_pswpMain, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
	m_ppnlPassword->SetBackgroundColour(wxColour(0,0,0));
	m_pedtPassword = new wmEdit(m_ppnlPassword, ID_M_PEDT1, wxEmptyString, wxPoint(110,40), wxSize(600,45), wxTE_PASSWORD, wxDefaultValidator, _T("ID_M_PEDT1"));
	m_pedtPassword->SetValidation(3);
	m_pedtPassword->SetBackgroundColour(wxColour(255,255,255));
	m_pedtPassword->SetFocusedBackground(wxColour("#FFFFFF"));
	m_pedtPassword->SetFocusedForeground(wxColour("#000000"));
	m_pedtPassword->SetBorderStyle(1,1);
	m_pkeyboard = new wmKeyboard(m_ppnlPassword, ID_M_PKBD1, wxPoint(110,90), wxDefaultSize, 0, 0);
	m_pkeyboard->SetForegroundColour(wxColour(255,255,255));
	m_pLblPassword = new wmLabel(m_ppnlPassword, ID_M_PLBL1, _("Enter Password"), wxPoint(0,5), wxSize(800,35), 0, _T("ID_M_PLBL1"));
	m_pLblPassword->SetBorderState(uiRect::BORDER_NONE);
	m_pLblPassword->GetUiRect().SetGradient(0);
	m_pLblPassword->SetForegroundColour(wxColour(0,255,0));
	wxFont m_pLbl1Font(16,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Arial"),wxFONTENCODING_DEFAULT);
	m_pLblPassword->SetFont(m_pLbl1Font);
	m_pswpMain->AddPage(m_ppnlProgress, _("Progress"), false);
	m_pswpMain->AddPage(m_ppnlRelease, _("Release"), true);
	m_pswpMain->AddPage(m_ppnlPassword, _("Password"), false);
	m_pbtnCancel = new wmButton(this, ID_M_PBTN4, _("Close"), wxPoint(240,443), wxSize(100,35), 0, wxDefaultValidator, _T("ID_M_PBTN4"));
	m_pbtnCancel->SetBackgroundColour(wxColour(128,0,0));
	m_pbtnCancel->SetColourSelected(wxColour("#FF0000"));
	m_pbtnCancel->SetColourDisabled(wxColour("#909090"));
	m_pbtnUpdate = new wmButton(this, ID_M_PBTN1, _("Update"), wxPoint(460,443), wxSize(100,35), 0, wxDefaultValidator, _T("ID_M_PBTN1"));
	m_pbtnUpdate->Disable();
	m_pbtnUpdate->SetBackgroundColour(wxColour(0,128,0));
	m_pbtnUpdate->SetColourSelected(wxColour("#00FF00"));
	m_pbtnUpdate->SetColourDisabled(wxColour("#909090"));

    //UnmountDevice();
	m_timerStart.SetOwner(this, ID_TIMER2);
	m_timerStart.Start(500, true);

	Connect(ID_M_PEDT1,wxEVT_COMMAND_TEXT_ENTER,(wxObjectEventFunction)&pamupdatemanagerDialog::OnedtPasswordTextEnter);
	Connect(ID_M_PBTN4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnCancelClick);
	Connect(ID_M_PBTN1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&pamupdatemanagerDialog::OnbtnUpdateClick);
	Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&pamupdatemanagerDialog::OntimerStartTrigger);

    Settings::Get().Write("Version", "pamupdatemanager", pml::pamupdatemanager::VERSION_STRING);

    m_plstRelease = new wmListAdv(m_ppnlRelease, wxNewId(), wxPoint(0,40), wxSize(800,360), 0, wmListAdv::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(0,0));
	m_plstRelease->SetFont(wxFont(10,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Verdana"),wxFONTENCODING_DEFAULT));
	m_plstRelease->SetBackgroundColour(*wxWHITE);

	m_plstProgress = new wmListAdv(m_ppnlProgress, wxNewId(), wxPoint(0,40), wxSize(800,360), 0, wmListAdv::SCROLL_VERTICAL, wxSize(-1,30), 1, wxSize(0,0));
	m_plstProgress->SetFont(wxFont(10,wxFONTFAMILY_ROMAN,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Verdana"),wxFONTENCODING_DEFAULT));
	m_plstProgress->SetBackgroundColour(*wxWHITE);

	m_plblTitle->SetLabel(wxString::Format("PAM Update Manager [%s] - '%s'", pml::pamupdatemanager::VERSION_STRING, m_fnUpdate.GetName().c_str()));

}

pamupdatemanagerDialog::~pamupdatemanagerDialog()
{
    UnmountDevice();
}

void pamupdatemanagerDialog::OnedtPasswordTextEnter(wxCommandEvent& event)
{
    long nResult = wxExecute(wxString::Format("sh -c \"echo %s | sudo -S -v -k\"", event.GetString().c_str()), wxEXEC_SYNC);
    if(nResult == 0)
    {
        m_pswpMain->ChangeSelection("Progress");
        Update();
    }
    else
    {
        m_pLblPassword->SetLabel("Password Incorrect!");
        m_pedtPassword->SetValue("");
        m_pedtPassword->SetFocus();
    }
}


void pamupdatemanagerDialog::Update()
{
    m_pbtnCancel->Show(false);
    wxClientDC dc(this);
    dc.SetFont(m_plstRelease->GetFont());

    if(ExtractAndUpdate(dc) == false)
    {
        m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, " "));
        m_plstProgress->ShowElement(m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "## Update failed. Reverting changes")), wmListAdv::BOTTOM);
        m_plstProgress->Update();

        RevertAll();
    }
    else
    {
        m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, " "));
        m_plstProgress->ShowElement(m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "## Update Successful")), wmListAdv::BOTTOM);
        m_plstProgress->Update();

        PostUpdate();
        StoreBackupFileNames();
    }
    m_pbtnCancel->SetLabel("Reboot");
    m_pbtnCancel->Show(true);
}

void pamupdatemanagerDialog::OnbtnCancelClick(wxCommandEvent& event)
{
    if(m_pbtnCancel->GetLabel() == "Reboot")
    {
        wxShutdown(wxSHUTDOWN_REBOOT);
    }
    else
    {
        EndModal(wxID_OK);
    }
}

void pamupdatemanagerDialog::OnbtnUpdateClick(wxCommandEvent& event)
{
    m_pbtnUpdate->Show(false);
    m_pswpMain->ChangeSelection("Password");
    m_pedtPassword->SetFocus();
}

void pamupdatemanagerDialog::OntimerStartTrigger(wxTimerEvent& event)
{
    wxClientDC dc(this);
    dc.SetFont(m_plstRelease->GetFont());

    int nMounted = IsMounted();
    if(nMounted == -2)
    {
        m_plstProgress->ShowElement(m_plstRelease->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Could not check whether drive mounted")), wmListAdv::BOTTOM);
        UnmountDevice();
        nMounted = MountDevice(m_sDevice);
    }
    else if(nMounted == -1)
    {
        m_plstProgress->ShowElement(m_plstRelease->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Drive not mounted")), wmListAdv::BOTTOM);
        nMounted = MountDevice(m_sDevice);
    }
    else
    {
        nMounted = 0;
    }

    if(nMounted != 0)
    {
        m_plstRelease->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "## Could not mount device"));
        m_plstProgress->ShowElement(m_plstRelease->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, strerror(errno))), wmListAdv::BOTTOM);
    }
    else
    {
        wxFileInputStream in(m_fnUpdate.GetFullPath());
        wxTarInputStream tar(in);
        wxTarEntry* pEntry = nullptr;
        bool bRelease(false);
        do
        {
            pEntry = tar.GetNextEntry();
            if(pEntry && pEntry->GetName().CmpNoCase("release.md") == 0)
            {
                bRelease = true;
                wxTextInputStream text(tar);
                while(tar.IsOk() && !tar.Eof())
                {
                    m_plstRelease->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, text.ReadLine()));
                }
                break;
            }
        }while(pEntry);

        if(bRelease)
        {
            m_pbtnUpdate->Enable();
        }
        else
        {
            m_plstRelease->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "## Not a valid update file"));
        }
    }
}


bool pamupdatemanagerDialog::ExtractAndUpdate(wxDC& dc)
{

    m_plstProgress->ShowElement(m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "# Update")), wmListAdv::BOTTOM);
    m_plstProgress->Update();

    wxFileInputStream in(m_fnUpdate.GetFullPath());
    wxTarInputStream tar(in);
    wxTarEntry* pEntry = nullptr;

    do
    {
        pEntry = tar.GetNextEntry();
        if(pEntry)
        {
            if(pEntry->IsDir() == false)
            {
                wxFileName fnFile(pEntry->GetName());

                wxFileName fnExisting;
                fnExisting.SetFullName(fnFile.GetFullName());

                m_plstProgress->ShowElement(m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "## "+fnFile.GetFullName())), wmListAdv::TOP);
                m_plstProgress->Update();

                auto asDir = fnFile.GetDirs();
                if(asDir.Count() > 0)
                {
                    if(asDir[0] == "lib")
                    {
                        if(asDir.Count() > 1)
                        {
                            if(asDir[1] == "monitor")
                            {
                                fnExisting.SetPath("/usr/local/lib/pam2/monitor");
                            }
                            else if(asDir[1] == "test")
                            {
                                fnExisting.SetPath("/usr/local/lib/pam2/test");
                            }
                            else if(asDir[1] == "generator")
                            {
                                fnExisting.SetPath("/usr/local/lib/pam2/generator");
                            }
                        }
                        else
                        {
                            fnExisting.SetPath("/usr/local/lib");
                        }
                        if(ReplaceFileUsr(dc, tar, fnExisting) == false)
                        {
                            return false;
                        }
                    }
                    else if(asDir[0] == "bin")
                    {
                        fnExisting.SetPath("/usr/local/bin");
                        if(ReplaceFileUsr(dc, tar, fnExisting) == false)
                        {
                            return false;
                        }
                    }
                    else if(asDir[0] == "documents")
                    {
                        fnExisting.SetPath(Settings::Get().GetDocumentDirectory()+"/"+fnFile.GetPath().AfterFirst('/'));
                        if(ReplaceFile(dc, tar, fnExisting) == false)
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }while(pEntry);

    return true;
}


bool pamupdatemanagerDialog::ReplaceFile(wxDC& dc, wxTarInputStream& input, const wxFileName& fnOutput)
{
    //rename the file
    if(wxFileExists(fnOutput.GetFullPath()))
    {
        wxRenameFile(fnOutput.GetFullPath(), fnOutput.GetFullPath()+".bak");
        m_setUpdated.insert(fnOutput.GetFullPath()+".bak");
    }

    wxFileOutputStream out(fnOutput.GetFullPath());
    if(out.IsOk())
    {
        //read in and output
        while(input.IsOk() && !input.Eof())
        {
            input.Read(out);
        }
        m_plstProgress->ShowElement(m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Updated")), wmListAdv::TOP);
        m_plstProgress->Update();
        return true;
    }
    else
    {
        m_plstProgress->ShowElement(m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Failed to update")), wmListAdv::BOTTOM);
        m_plstProgress->Update();
        return false;
    }
}

bool pamupdatemanagerDialog::ReplaceFileUsr(wxDC& dc, wxTarInputStream& input, const wxFileName& fnOutput)
{
    //rename the file
    if(wxFileExists(fnOutput.GetFullPath()))
    {
        auto nResult = wxExecute(wxString::Format("sh -c \"echo %s | sudo -S -k mv %s %s.bak\"", m_pedtPassword->GetValue().c_str(), fnOutput.GetFullPath().c_str(), fnOutput.GetFullPath().c_str()), wxEXEC_SYNC);
        if(nResult != 0)
        {
            m_plstProgress->ShowElement(m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Failed to create backup")), wmListAdv::BOTTOM);
            m_plstProgress->Update();
            return false;
        }
    }


    wxString sTemp = "/tmp/"+fnOutput.GetFullName();
    wxFileOutputStream out(sTemp);
    if(out.IsOk())
    {
        //read in and output
        while(input.IsOk() && !input.Eof())
        {
            input.Read(out);
        }
        auto nResult = wxExecute(wxString::Format("sh -c \"echo %s | sudo -S -k mv %s %s\"", m_pedtPassword->GetValue().c_str(), sTemp.c_str(), fnOutput.GetFullPath().c_str()), wxEXEC_SYNC);
        nResult += wxExecute(wxString::Format("sh -c \"echo %s | sudo -S -k chmod +x %s\"", m_pedtPassword->GetValue().c_str(), fnOutput.GetFullPath().c_str()), wxEXEC_SYNC);
        if(nResult == 0)
        {
            m_setUpdated.insert(fnOutput.GetFullPath()+".bak");
            m_plstProgress->ShowElement(m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Updated")), wmListAdv::BOTTOM);
            m_plstProgress->Update();
            return true;
        }
    }
    else
    {
        m_plstProgress->ShowElement(m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Failed to update")), wmListAdv::BOTTOM);
        m_plstProgress->Update();
        return false;
    }
}

void pamupdatemanagerDialog::StoreBackupFileNames()
{
    wxString sRemove;
    for(auto sBackup : m_setUpdated)
    {
        if(sRemove.empty() == false)
        {
            sRemove << ",";
        }
        sRemove << sBackup;
    }
    Settings::Get().Write("Startup", "Remove", sRemove);
}


void pamupdatemanagerDialog::RevertAll()
{
    for(auto sBackup : m_setUpdated)
    {
        wxString sFile = sBackup.BeforeLast('.');
        wxRemoveFile(sFile);
        wxRenameFile(sBackup, sFile);
    }
}

void pamupdatemanagerDialog::PostUpdate()
{
    wxExecute(wxString::Format("sh -c \"echo %s | sudo -S -k setcap cap_sys_time,cap_sys_admin,cap_net_bind_service+ep /usr/local/bin/pam2 \"", m_pedtPassword->GetValue().c_str()), wxEXEC_SYNC);
//    wxExecute(wxString::Format("sh -c \"echo %s | sudo -S -k ldconfig \"", m_pedtPassword->GetValue().c_str()), wxEXEC_SYNC);
}
