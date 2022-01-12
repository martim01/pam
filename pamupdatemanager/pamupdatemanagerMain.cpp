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
#include "releaseelement.h"
#ifdef __WXGNU__
#include <sys/mount.h>
#endif // __WXGNU__

#include <wx/stdpaths.h>


int UnmountDevice()
{
    int nResult = umount("/mnt/share");
    if(nResult == -1 && errno != EAGAIN && errno != EINVAL)
    {
        std::cout << "Could not umount device: " << strerror(errno);
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


    std::array<std::string, 8> fs({"ext3", "ext2", "ext4", "vfat", "msdos", "f2fs", "fuseblk", "ntfs"});
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

    UnmountDevice();
	m_timerStart.SetOwner(this, ID_TIMER2);
	m_timerStart.Start(1000, true);

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

	wxClientDC dc(this);
    dc.SetFont(m_plstProgress->GetFont());

    m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, Settings::Get().GetConfigDirectory()));
    m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, Settings::Get().GetDocumentDirectory()));
    m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, wxStandardPaths::Get().GetUserDataDir()));
    m_plstProgress->Update();



}

pamupdatemanagerDialog::~pamupdatemanagerDialog()
{
    UnmountDevice();
}

void pamupdatemanagerDialog::OnedtPasswordTextEnter(wxCommandEvent& event)
{
    long nResult = wxExecute(wxString::Format("echo %s | sudo -S -k cat /etc/shadow", event.GetString().c_str()), wxEXEC_SYNC);
    if(nResult == 0)
    {
        m_pswpMain->ChangeSelection("Progress");
        Update();
    }
    else
    {
        m_pLblPassword->SetLabel("Password Incorrect!");
    }
}


void pamupdatemanagerDialog::Update()
{
    wxClientDC dc(this);
    dc.SetFont(m_plstRelease->GetFont());

    if(ExtractAndUpdate(dc) == false)
    {
        m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, " "));
        m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "## Update failed. Reverting changes"));
        m_plstProgress->Update();

        RevertAll();
    }
    else
    {
        m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, " "));
        m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "## Update Successful"));
        m_plstProgress->Update();

        PostUpdate();
        StoreBackupFileNames();
    }
}

void pamupdatemanagerDialog::OnbtnCancelClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
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

    if(MountDevice(m_sDevice) != 0)
    {
        m_plstRelease->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "## Could not mount device"));
        m_plstRelease->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, strerror(errno)));
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

    m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "# Update"));
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

                wxFileName fnExisiting;
                fnExisiting.SetFullName(fnFile.GetFullName());

                m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "### "+fnFile.GetFullName()));
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
                                fnExisiting.SetPath(GetRealPath("/usr/local/lib/pam2/monitor"));
                            }
                            else if(asDir[1] == "test")
                            {
                                fnExisiting.SetPath(GetRealPath("/usr/local/lib/pam2/test"));
                            }
                            else if(asDir[1] == "generator")
                            {
                                fnExisiting.SetPath(GetRealPath("/usr/local/lib/pam2/generator"));
                            }
                        }
                        else
                        {
                            fnExisiting.SetPath(GetRealPath("/usr/local/lib/pam2"));
                        }
                    }
                    else if(asDir[0] == "bin")
                    {
                        fnExisiting.SetPath(GetRealPath("/usr/local/bin/pam2"));
                    }
                    else if(asDir[0] == "documents")
                    {
                        if(asDir.Count() > 1)
                        {
                            if(asDir[1] == "help")
                            {
                                if(asDir.Count() > 2 && asDir[2] == "images")
                                {
                                    fnExisiting.SetPath(Settings::Get().GetDocumentDirectory()+"/help/images");
                                }
                                else
                                {
                                    fnExisiting.SetPath(Settings::Get().GetDocumentDirectory()+"/help");
                                }
                            }
                            else if(asDir[1] == "generator")
                            {
                                fnExisiting.SetPath(Settings::Get().GetDocumentDirectory()+"/generator");
                            }
                        }
                        else
                        {
                            fnExisiting.SetPath(Settings::Get().GetConfigDirectory());
                        }
                    }

                    if(ReplaceFile(dc, tar, fnExisiting) == false)
                    {
                        return false;
                    }
                }
            }
        }
    }while(pEntry);

    return true;
}


wxString pamupdatemanagerDialog::GetRealPath(const wxString& sPath)
{
    auto ins = m_mRealPaths.insert({sPath,""});
    if(ins.second)
    {

        wxClientDC dc(this);
        dc.SetFont(m_plstProgress->GetFont());

        wxArrayString asOutput;
        wxArrayString asError;
        long nResult = wxExecute("realpath "+sPath, asOutput, asError);

        if(nResult == 0 && asOutput.Count() != 0)
        {
            ins.first->second = asOutput[0];
        }
    }
    return ins.first->second;
}

bool pamupdatemanagerDialog::ReplaceFile(wxDC& dc, wxTarInputStream& input, const wxFileName& fnOutput)
{
    //rename the file
    wxString sBackup = wxString::Format("%s.bak", fnOutput.GetFullPath().c_str());
    if(!wxFileExists(fnOutput.GetFullPath()) || wxRenameFile(fnOutput.GetFullPath(), sBackup))
    {
        wxFileOutputStream out(fnOutput.GetPath());
        if(out.IsOk())
        {
            //read in and output
            while(input.IsOk() && !input.Eof())
            {
                input.Read(out);
            }

            m_setUpdated.insert(sBackup);
            m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Updated"));
            m_plstProgress->Update();
            return true;
        }
        else
        {
            m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Failed to update"));
            m_plstProgress->Update();
            return false;
        }
    }
    else
    {
        m_plstProgress->AddElement(std::make_shared<ReleaseElement>(dc, GetClientSize().x, "Failed to create backup"));
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
    wxExecute(wxString::Format("echo %s | sudo -S -k setcap cap_sys_time,cap_sys_admin,cap_net_bind_service+ep %s", m_pedtPassword->GetValue().c_str(), GetRealPath("/usr/local/bin/pam2").c_str()), wxEXEC_SYNC);
    wxExecute(wxString::Format("echo %s | sudo -S -k ldconfig", m_pedtPassword->GetValue().c_str()), wxEXEC_SYNC);
}
