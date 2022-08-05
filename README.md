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
* OpenSSL
* libCurl


##### To install these libraries on Raspbian
On Buster and earlier
```
sudo apt-get update
sudo apt-get install libwxgtk3.0-dev portaudio19-dev libsndfile1-dev libsamplerate0-dev libavahi-client-dev libcap-dev libssl-dev libcurl4-openssl-dev
```
On Bullseye
```
sudo apt-get update
sudo apt-get install libwxgtk3.0-gtk3-dev portaudio19-dev libsndfile1-dev libsamplerate0-dev libavahi-client-dev libcap-dev libssl-dev libcurl4-openssl-dev
```

### Other libraries required
PAM uses a number of small libraries. All these libraries are hosted on GitHub and are cloned automatically when running cmake.
* [log](https://github.com/martim01/log)   a simple streaming log class
* [dnssd](https://github.com/martim01/dnssd)   a cross platform wrapper around Bonjour and Avahi for service browsing and publishing
* [sapserver](https://github.com/martim01/sapserver) a library to detect and publish SAP
* [ptpmonkey](https://github.com/martim01/ptpmonkey)  allows PAM to decode and analyse PTP messages and also sync to a PTP grandmaster
* [asio](https://github.com/chriskohlhoff/asio) used by sapserver and ptpmonkey
* [Mongoose](https://GitHub.com/cesanta/mongoose) http and websocket library (only needed for the NMOS build)
* [restgoose](https://GitHub.com/martim01/restgoose) a Restful server built on top of Mongoose (only needed for the NMOS build)
* [nmos](https://github.com/martim01/nmos)   an optional library to allow PAM to advertise itself as an NMOS node and also act as an NMOS client

## Building PAM

Workspace and project files are supplied for [Code::Blocks IDE](http://www.codeblocks.org/). There are Debug and Release builds for Windows and Linux
There are also CMakeLists.txt files. They have been tested on Linux using GCC

### To build using CMake (currently on Linux only)
The easiest way to obtain the required GitHub libraries and build and install a PAM on Linux is by using CMake.
The CMake build will clone all the required GitHub libraries if it can't find them and update them to their latest version if it can.
By default it looks in your home directory. You can change this by passing in the variable __DIR_BASE__ to cmake

```
cmake -DDIR_BASE=/home/user/pam_external
```

It is also possible to change the expected directory for each individual library
```
cmake -DDIR_LOG=logdir
```

The following directories can be defined
- __DIR_NMOS__ the location of your nmos directory (or where you want it cloned to).
- __DIR_PTPMONKEY__ the location of your ptpmonkey directory (or where you want it cloned to).
- __DIR_SAPSERVER__ the location of your sapserver directory (or where you want it cloned to).
- __DIR_DNSSD__ the location of your dnssd directory directory (or where you want it cloned to).
- __DIR_LOG__ the location of your log directory (or where you want it cloned to).
- __DIR_MONGOOSE__ the location of your Mongoose directory (or where you want it cloned to).
- __DIR_ASIO__ the location of your asio directory (or where you want it cloned to).

To build and install all applications and libraries
```
cd {pam directory}/build
cmake ..
cmake --build .
sudo cmake --build . --target install
```
This will 
- clone all releveant source code if necessary
- configure and build all applications and libraries
- install the executable __pam2__ in __/usr/local/bin__ 
- install all necessary libraries in __/usr/local/lib/pam2__
- set the relevant capabilities on the application
- create the required __pam2.conf__ file in __/etc.ld.so.conf.d__ and run __ldconfig__ so Linux knows about the new libraries
- copy the required files from the documents directory

### To build in Code::Blocks

#### Build live555 static libraries
* Open external/live/live.workspace in Codeblocks
* Fill in the necessary global variables
* Build the workspace

#### Buld pam2 application and libraries
* Open pam2.workspace in Codeblocks
* Fill in the necssary global variables
* Build the workspace

* Create a file called __pam2.conf__ in directory __/etc/ld.so.conf.d__
* Add the following line to the file
```
/usr/local/lib/pam2
```
* Run the following command to update the library path
```
sudo ldconfig
```

In order for PAM to have the correct capabilities you need to run the following
```
setcap cap_sys_time,cap_sys_admin,cap_net_bind_service+ep absolutepathtopam/pam2
```

#### PTPMonkey support
This is included by default.

When subscribing to an AoIP stream whose SDP defines a PTP reference PTPMonkey will listen for a Master Clock on the relevant domain and use the Master Clock's time for timestamping incoming RTP packets. The PTP plugin will also depend on this library being part of the base build

#### NMOS support

[NMOS](https://github.com/AMWA-TV/nmos/wiki) support can be build in to __PAM__ allowing control of AoIP streams in to and out of the software from external devices. It is also possible to select and route NMOS compliant sources to the software for monitoring. 

If you wish to include [nmos](https://github.com/martim01/nmos) in the application then run ``` cmake .. -DNMOS=ON ``` instead of ``` cmake ...```

**__Note: NMOS support is a work in progress. The Node library passes all the NMOS tests and the Client library can be used to make simple connections. Amongst other things__**
* **__Work is needed on the client side for systems with many nodes in order to display and filter them properly.__**

### Setting Up

PAM expects a few files to live in a specific directory:
* If it doesn't exist create a directory called __pam__ in your home directory.
* Copy the contents of the __document__ directory to the __pam__ directory.
* If you used CMake to build and install PAM then this will happen automatically

## Prebuilt Images
Links to these can be found [here](https://github.com/martim01/pam/releases) with the associated Releaes


## List of Plugins
Can be found in the wiki section
