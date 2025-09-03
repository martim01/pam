#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <sstream>
#include <pwd.h>
#include <sys/types.h>
#include "log.h"
#include "logtofile.h"
#include <filesystem>


static const std::string STR_DTPARAM = "dtparam=audio";
static const std::string STR_DTOVERLAY = "dtoverlay=";
static const std::string STR_LOCAL = "127.0.1.1";
static const std::string STR_VC4 = "dtoverlay=vc4-kms-v3d";

int SetHostname(const std::string& sHostname)
{
    pmlLog(pml::LOG_INFO, "dosetup") << "Set hostname to " << sHostname;

    char c[255];
    if(gethostname(&c[0], 255) != 0)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Unable to get current hostname " << strerror(errno);
        return -1;
    }
    std::string sCurrentHost(c);

    pmlLog(pml::LOG_INFO, "dosetup") << "Current hostname = " << sCurrentHost;

    std::ifstream input("/etc/hosts");
    if(!input)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Unable to read /etc/hosts";
        return -1;
    }


    std::stringstream ssOut;
    std::string sLine;
    while(getline(input, sLine))
    {
        if(sLine.substr(0, STR_LOCAL.length()) == STR_LOCAL && sLine.find(sCurrentHost) != std::string::npos)
        {
            ssOut << STR_LOCAL << "\t" << sHostname << '\n';

            pmlLog(pml::LOG_INFO, "dosetup") << "/etc/hosts\tChange '" << sLine << "' to '" << STR_LOCAL << "\t" << sHostname << "'";
        }
        else
        {
            ssOut << sLine  << '\n';
        }
    }
    input.close();


    std::ofstream over("/etc/hosts");
    std::ofstream out("/etc/hostname");
    if(!over)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Unable to write to /etc/hosts";
        return -1;
    }

    if(!out)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Unable to write to /etc/hostname";
        return -1;
    }

    out << sHostname << std::endl;
    out.close();

    over << ssOut.str() << std::endl;
    over.close();

    pmlLog(pml::LOG_INFO, "dosetup") << "/etc/hosts and /etc/hostname overwritten";


    // @todo do we need this here?
    //system("sudo raspi-config nonint do_expand_rootfs");

    return 0;
}

int SetPassword(const std::string& sPassword)
{
    auto user = getlogin();
    if(user)
    {
        //set the password
        std::string sCommand = "echo '";
        sCommand += user;
        sCommand += ":"+sPassword+"' | sudo chpasswd";

        system(sCommand.c_str());

        pmlLog(pml::LOG_INFO, "dosetup") << "Password changed";
        return 0;
    }
    pmlLog(pml::LOG_ERROR, "dosetup") << "Failed to change password";
    return 0;
}

int SetOverlay(const std::string& sOverlay, const std::string& sLineNumber, const std::string& sRotate)
{
    if(sOverlay == "_") //this means no HAT
    {
        return 0;
    }

    pmlLog(pml::LOG_INFO, "dosetup") << "Set overlay to " << sOverlay;

    int nLine = -1;
    try
    {
        nLine = std::stoi(sLineNumber);
    }
    catch(const std::invalid_argument& e)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Line number incorrect: " << sLineNumber;
    }

    std::ifstream input("/boot/firmware/config.txt");
    std::stringstream output;

    if(!input)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Could not open /boot/firmware/config.txt";
        return -1;
    }

    std::string sLine;
    int nCount = 0;
    bool bAdded(false);
    bool bRotateDone(false);
    while(getline(input, sLine))
    {
        if(sLine.substr(0, STR_DTPARAM.length()) == STR_DTPARAM)
        {
            output << "#" << sLine << '\n';
            pmlLog(pml::LOG_INFO, "dosetup") << "Commented out " << STR_DTPARAM;

            if(nLine == -1)
            {
                output << STR_DTOVERLAY << sOverlay << '\n';
                pmlLog(pml::LOG_INFO, "dosetup") << "Insert overlay";
                bAdded = true;
            }
        }
        else if(nCount == nLine)
        {
            pmlLog(pml::LOG_INFO, "dosetup") << "Overwrite overlay";
            output << STR_DTOVERLAY << sOverlay << '\n';
            bAdded = true;
        }
        else if(sLine == "display_rotate=2")
        {
            if(sRotate == "0")
            {
                output << "#display_rotate=2" << '\n';
            }
            bRotateDone = true;
        }
        else if(sLine == "#display_rotate=2")
        {
            if(sRotate == "1")
            {
                output << "display_rotate=2" << '\n';
            }
            bRotateDone = true;
        }
        else if(sLine.substr(0,STR_VC4.length()) != STR_VC4)
        {
            output << sLine << '\n';
        }
        else
        {
         std::cout << "removed" << std::endl;
        }
        nCount++;
    }
    if(!bAdded)
    {
        pmlLog(pml::LOG_INFO, "dosetup") << "Append overlay";
        output << STR_DTOVERLAY << sOverlay << '\n';
    }
    if(!bRotateDone && sRotate == "1")
    {
        pmlLog(pml::LOG_INFO, "dosetup") << "Append overlay";
        output << "display_rotate=2" << '\n';
    }

    input.close();

    std::ofstream outputFile("/boot/firmware/config.txt");
    if(!outputFile)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Unable to set overlay: " << strerror(errno);
        return -1;
    }

    outputFile << output.str() << std::endl;
    outputFile.close();

    pmlLog(pml::LOG_INFO, "dosetup") << "/boot/firmware/config.txt overwritten";
    return 0;
}

void SetRotate(const std::string& sRotate)
{
    if(sRotate == "1")
    {
        if(std::filesystem::exists("/usr/local/etc/pam2/rotate") == false)
        {
            std::ofstream rotate("/usr/local/etc/pam2/rotate");
        }
    }
    else
    {
        if(std::filesystem::exists("/usr/local/etc/pam2/rotate") == true)
        {
            std::filesystem::remove("/usr/local/etc/pam2/remove");
        }
    }
}


int main(int argc, char* argv[])
{
    std::string sRoot = "/opt";
    pml::log::Stream::AddOutput(std::make_unique<pml::log::File>(sRoot, pml::log::Output::kTsTime, pml::log::Output::TS::kSecond));

    pmlLog(pml::LOG_INFO, "dosetup") << "---- Starting dosetup ----";

    if(argc != 6)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Not enough arguments. Need 6 given " << argc;
        pmlLog(pml::LOG_INFO, "dosetup") << "Usage: hostname password overlay line_to_replace";
        pml::log::Stream::Stop();
        std::cout << "STOPPED" << std::endl;
        return -1;
    }


    if(SetHostname(argv[1]) !=0)
    {
        pmlLog(pml::LOG_CRITICAL, "dosetup") << "Failed to set hostname. Exiting";
        pml::log::Stream::Stop();
        std::cout << "STOPPED" << std::endl;

        return -1;
    }

    if(SetPassword(argv[2]) !=0)
    {
        pmlLog(pml::LOG_CRITICAL, "dosetup") << "Failed to set password. Exiting";
        pml::log::Stream::Stop();
        std::cout << "STOPPED" << std::endl;

        return -1;
    }


    if(SetOverlay(argv[3], argv[4], argv[5]) != 0)
    {
        pml::log::Stream::Stop();
        std::cout << "STOPPED" << std::endl;
        return -1;
    }
    
    SetRotate(argv[5]);

    pmlLog(pml::LOG_INFO, "dosetup") << "------ FINISHED ------";
    pml::log::Stream::Stop();
    std::cout << "STOPPED" << std::endl;

    return 0;
}
