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

static const std::string STR_DTPARAM = "dtparam=audio";
static const std::string STR_DTOVERLAY = "dtoverlay=";
static const std::string STR_LOCAL = "127.0.1.1";

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

int SetOverlay(const std::string& sOverlay, const std::string& sLineNumber, bool bRotate)
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
        else if(sLine == "lcd_rotate=2")
        {
            if(bRotate == false)
            {
                output << "#lcr_rotate=2" << '\n';
            }
            bRotateDone = true;
        }
        else if(sLine == "#lcd_rotate=2")
        {
            if(bRotate)
            {
                output << "lcr_rotate=2" << '\n';
            }
            bRotateDone = true;
        }
        else
        {
            output << sLine << '\n';
        }
        nCount++;
    }
    if(!bAdded)
    {
        pmlLog(pml::LOG_INFO, "dosetup") << "Append overlay";
        output << STR_DTOVERLAY << sOverlay << '\n';
    }
    if(!bRotateDone && bRotate)
    {
        pmlLog(pml::LOG_INFO, "dosetup") << "Append overlay";
        output << "lcr_rotate=2" << '\n';
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

int SetRotate(bool bRotate)
{
    std::ifstream input("/boot/firmware/cmdline.txt");
    std::stringstream output;

    if(!input)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Could not open /boot/firmware/cmdline.txt";
        return -1;
    }

    std::string sLine;
    int nCount = 0;
    bool bAdded(false);
    bool bRotateDone(false);
    while(getline(input, sLine, ' '))
    {
        if(sLine == "video=DSI-1:800x480@60,rotate=180")
        {
            if(bRotate)
            {
                output << sLine << " ";
            }
            bRotateDone = true;
        }        
        else
        {
            output << sLine << " ";
        }

    }
    if(!bRotateDone && bRotate)
    {
        output << "video=DSI-1:800x480@60,rotate=180";
    }

    input.close();

    std::ofstream outputFile("/boot/firmware/cmdline.txt");
    if(!outputFile)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Unable to rotate screen " << strerror(errno);
        return -1;
    }

    outputFile << output.str() << std::endl;
    outputFile.close();

    pmlLog(pml::LOG_INFO, "dosetup") << "/boot/firmware/cmdline.txt overwritten";
    return 0;
}

int main(int argc, char* argv[])
{
    std::string sRoot = "/opt";
    pml::LogStream::AddOutput(std::make_unique<pml::LogToFile>(sRoot, 2, 0));

    pmlLog(pml::LOG_INFO, "dosetup") << "---- Starting dosetup ----";

    if(argc != 6)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Not enough arguments. Need 6 given " << argc;
        pmlLog(pml::LOG_INFO, "dosetup") << "Usage: hostname password overlay line_to_replace";
        pml::LogStream::Stop();
        return -1;
    }


    if(SetHostname(argv[1]) !=0)
    {
        pmlLog(pml::LOG_CRITICAL, "dosetup") << "Failed to set hostname. Exiting";
        pml::LogStream::Stop();

        return -1;
    }

    if(SetPassword(argv[2]) !=0)
    {
        pmlLog(pml::LOG_CRITICAL, "dosetup") << "Failed to set password. Exiting";
        pml::LogStream::Stop();

        return -1;
    }

    if(SetRotate(argv[5] != 0))
    {
        pmlLog(pml::LOG_CRITICAL, "dosetup") << "Failed to set screen rotation. Exiting";
        pml::LogStream::Stop();

        return -1;
    }
    if(SetOverlay(argv[3], argv[4], argv[5]!=0) != 0)
    {
        pml::LogStream::Stop();
        return -1;
    }

    pml::LogStream::Stop();
    return 0;
}
