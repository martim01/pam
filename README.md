# pam
Open Source Audio Monitoring Software designed for (but not limited to) a Raspberry Pi with touchscreen.

## Prerequisites

### Core libraries required

wxWidgets   https://www.wxwidgets.org/
PortAudio
libsndfile   http://www.mega-nerd.com/libsndfile/
Secret Rabit Code  http://www.mega-nerd.com/SRC/


#### For UpdateManager
libcurl  https://curl.haxx.se/download.html


#### For UpdateWebServer
MicroHttp  https://www.gnu.org/software/libmicrohttpd/

## Building PAM

Workspace and project files are supplied for Code::Blocks IDE. There are Debug and Release builds for Windows and Linux

Codeblocks  http://www.codeblocks.org/

### To build in Code::Blocks

#### Build live555 static libraries
Open external/live/live.workspace in Codeblocks
Fill in the necessary global variables
Build the workspace

#### Buld pam2 application and libraries
Open pam2.workspace in Codeblocks
Fill in the necssary global variables
Build the workspace



