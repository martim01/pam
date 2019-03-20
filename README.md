# pam
Open Source Audio Monitoring Software designed for (but not limited to) a Raspberry Pi with touchscreen.

## Prerequisites

### Core libraries required

* wxWidgets   https://www.wxwidgets.org/
* PortAudio   http://www.portaudio.com/download.html
* libsndfile   http://www.mega-nerd.com/libsndfile/
* Secret Rabit Code  http://www.mega-nerd.com/SRC/
* Avahi-Client

#### For UpdateManager
* libcurl  https://curl.haxx.se/download.html

#### For UpdateWebServer
* MicroHttp  https://www.gnu.org/software/libmicrohttpd/


##### To install these libraries on Raspbian
```
sudo apt-get update
sudo apt-get install libwxgtk3.0-dev
sudo apt-get install portaudio19-dev
sudo apt-get install libsndfile1-dev
sudo apt-get install libsamplerate0-dev
sudo apt-get install libavahi-client-dev
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libmicrohttpd-dev
```



## Building PAM

Workspace and project files are supplied for Code::Blocks IDE. There are Debug and Release builds for Windows and Linux
There are also CMakeLists.txt files. They have been tested on Linux using GCC

Codeblocks  http://www.codeblocks.org/

### To build in Code::Blocks

#### Build live555 static libraries
* Open external/live/live.workspace in Codeblocks
* Fill in the necessary global variables
* Build the workspace

#### Buld pam2 application and libraries
* Open pam2.workspace in Codeblocks
* Fill in the necssary global variables
* Build the workspace


### To build using CMake
```
cd {pam directory}/build
cmake ..
cmake --build .
```


