# pam
Open Source Audio Monitoring Software designed for (but not limited to) a Raspberry Pi with touchscreen. 
The application provides:

* A number of useful audio monitoring tools, including:
  * Digital meters of various flavours (BBC PPM, EBU, Nordic, VU etc)
  * "Moving Coil" meters of various flavours
  * Lissajou
  * Spectrum Analyser
  * R128 Loudness
  * Scope
* A number of audio test tools, including:
  * Channel delay measurement
  * Distortion measurements
  * Level monitoring
  * Peak Sample Count
  * Recording of incoming audio
  * LTC detection
* An audio generator that can output audio from a number of sources, including:
  * The input source
  * A file
  * A simple tone
  * A tone sequence
  * LTC
  
PAM can take in audio from a soundcard or Pi HAT or from an AES67 stream. There is both DNS-SD and SAP discovery of AES67 streams and also an NMOS version which complies to IS04/IS05.

For more information please look at the help files contained in the __documents__ directory
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


### To build using CMake (currently on Linux only)
```
cd {pam directory}/build
cmake ..
cmake --build .
cmake --build . --target install
```
This will install the executable __pam2__ in __/usr/local/bin__ and all necessary libraries in __/usr/local/lib/pam2__

### Setting Up

PAM expects a few files to live in a specific directory:
* If it doesn't exist create a directory called __pam__ in your home directory.
* Copy the contents of the __document__ directory to the __pam__ directory.

