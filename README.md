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
On Buster and earlier
```
sudo apt-get update
sudo apt-get install libwxgtk3.0-dev portaudio19-dev libsndfile1-dev libsamplerate0-dev libavahi-client-dev libcurl4-openssl-dev libmicrohttpd-dev
```
On Bullseye
```
sudo apt-get update
sudo apt-get install libwxgtk3.0-gtk3-dev portaudio19-dev libsndfile1-dev libsamplerate0-dev libavahi-client-dev libcurl4-openssl-dev libmicrohttpd-dev
```

### Other libraries required
PAM uses a number of small libraries. All these libraries are hosted on GitHub and are cloned automatically when running cmake.
* [log](https://github.com/martim01/log)   a simple streaming log class
* [dnssd](https://github.com/martim01/dnssd)   a cross platform wrapper around Bonjour and Avahi for service browsing and publishing
* [sapserver]  a library to detect and publish SAP
* [ptpmonkey](https://github.com/martim01/ptpmonkey)  allows PAM to decode and analyse PTP messages and also sync to a PTP grandmaster
* [asio](https://github.com/chriskohlhoff/asio) used by sapserver and ptpmonkey
* [Mongoose](https://GitHub.com/cesanta/mongoose) http and websocket library (only needed for the NMOS build)
* [restgoose](https://GitHub.com/martim01/restgoose) a Restful server built on top of Mongoose (only needed for the NMOS build)
* [nmos](https://github.com/martim01/nmos)   an optional library to allow PAM to advertise itself as an NMOS node and also act as an NMOS client

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
The cmake build will clone all the required GitHub libraries if it can't find them and update them to their latest version if it can.
 By default it looks in the home directory. You can change this by passing in the variable DIR_BASE to cmake

```
cmake -DDIR_BASE=/home/user/pam_external
```
It is also possible to change the expected directory for each individual library
```
cmake -DDIR_LOG=logdir -DDIR_ASIO=asiodir
```

```
cd {pam directory}/build
cmake ..
cmake --build .
sudo cmake --build . --target install
```
This will install the executable __pam2__ in __/usr/local/bin__ and all necessary libraries in __/usr/local/lib/pam2__

It is possible that __/usr/local/lib__ is not in your path. If so then do the following:
* Create a file called __pam2.conf__ in directory __/etc/ld.so.conf.d__
* Add the following line to the file
```
/usr/local/lib/pam2
```
* Run the following command to update the library path
```
sudo ldconfig
```

#### PTPMonkey support
This is included by default.

When subscribing to an AoIP stream whose SDP defines a PTP reference PTPMonkey will listen for a Master Clock on the relevant domain and use the Master Clock's time for timestamping incoming RTP packets. The PTP plugin will also depend on this library being part of the base build

**__Note: If you wish to use PTPMonkey with PAM on Linux then you will need to run the following command to give pam the rights to open the network ports and set the system time__**
```
sudo setcap cap_sys_time,cap_net_bind_service+ep pathToPam/pam2
```

#### NMOS support

[NMOS](https://github.com/AMWA-TV/nmos/wiki) support can be build in to __PAM__ allowing control of AoIP streams in to and out of the software from external devices. It is also possible to select and route NMOS compliant sources to the software for monitoring. 

If you wish to include [nmos](https://github.com/martim01/nmos) in the application then run ``` cmake .. -DNMOS=ON ``` instead of ``` cmake ...```


### Setting Up

PAM expects a few files to live in a specific directory:
* If it doesn't exist create a directory called __pam__ in your home directory.
* Copy the contents of the __document__ directory to the __pam__ directory.


## Prebuilt Images
Links to these can be found [here](https://github.com/martim01/pam/releases) with the associated Releaes


## List of Plugins
Can be found in the wiki section
