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
        return 0;
    }
    return -1;
}

int SetOverlay(const std::string& sOverlay, const std::string& sLineNumber)
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

    std::ifstream input("/boot/config.txt");
    std::stringstream output;

    if(!input)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Could not open /boot/config.txt";
        return -1;
    }

    std::string sLine;
    int nCount = 0;
    bool bAdded(false);
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
    input.close();

    std::ofstream outputFile("/boot/config.txt");
    if(!outputFile)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Unable to set overlay: " << strerror(errno);
        return -1;
    }

    outputFile << output.str() << std::endl;
    outputFile.close();

    pmlLog(pml::LOG_INFO, "dosetup") << "/boot/config.txt overwritten";
    return 0;
}

int main(int argc, char* argv[])
{
    struct passwd* pw = getpwuid(getuid());
    std::string sRoot(pw->pw_dir);
    sRoot += "/pam/logs/setup";
    pml::LogStream::AddOutput(std::make_unique<pml::LogToFile>(sRoot, 2, 0));

    pmlLog(pml::LOG_INFO, "dosetup") << "---- Starting dosetup ----";

    if(argc != 5)
    {
        pmlLog(pml::LOG_ERROR, "dosetup") << "Not enough arguments. Need 5 given " << argc;
        pmlLog(pml::LOG_INFO, "dosetup") << "Usage: hostname password overlay line_to_replace";
        return -1;
    }


    if(SetHostname(argv[1]) !=0)
    {
        return -1;
    }

    if(SetPassword(argv[2]) !=0)
    {
        return -1;
    }

    return SetOverlay(argv[3], argv[4]);

}
