#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <string.h>
#include <sstream>

static const std::string STR_DTPARAM = "dtparam=audio";
static const std::string STR_DTOVERLAY = "dtoverlay=";

int SetHostname(const std::string& sHostname)
{
    int nResult = sethostname(sHostname.c_str(), sHostname.length());
    if(nResult != 0)
    {
        std::cout << "Unable to set hostname: " << strerror(errno);
        return -1;
    }
    return 0;
}

int SetPassword(const std::string& sPassword)
{
    //set the password
    std::string sCommand("echo 'pi:"+sPassword+"' | sudo chpasswd");
    system(sCommand.c_str());
    return 0;
}

int SetOverlay(const std::string& sOverlay, const std::string& sLineNumber)
{
    int nLine = -1;
    try
    {
        nLine = std::stoi(sLineNumber);
    }
    catch(const std::invalid_argument& e)
    {
        std::cout << "Line number incorrect: " << sLineNumber << std::endl;
    }

    std::ifstream input("/boot/config.txt");
    std::stringstream output;

    if(!input)
    {
        std::cout << "Could not open config.txt" << std::endl;
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
            if(nLine == -1)
            {
                output << STR_DTOVERLAY << sOverlay << '\n';
                bAdded = true;
            }
        }
        else if(nCount == nLine)
        {
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
        output << STR_DTOVERLAY << sOverlay << '\n';
    }
    input.close();

    std::ofstream outputFile("/boot/config.txt");
    if(!outputFile)
    {
        std::cout << "Unable to set overlay: " << strerror(errno);
        return -1;
    }

    outputFile << output.str();
    outputFile.close();
    return 0;
}

int main(int argc, char* argv[])
{
    if(argc != 5)
    {
        std::cout << "Usage: hostname password overlay line_to_replace" << std::endl;
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
