INCLUDES = -I../UsageEnvironment/include -I../groupsock/include -I../liveMedia/include -I../BasicUsageEnvironment/include
# Default library filename suffixes for each library that we link with.  The "config.*" file might redefine these later.
libliveMedia_LIB_SUFFIX = $(LIB_SUFFIX)
libBasicUsageEnvironment_LIB_SUFFIX = $(LIB_SUFFIX)
libUsageEnvironment_LIB_SUFFIX = $(LIB_SUFFIX)
libgroupsock_LIB_SUFFIX = $(LIB_SUFFIX)
##### Change the following for your environment:
COMPILE_OPTS =         $(INCLUDES) -I. -O -DSOCKLEN_T=int -DLOCALE_NOT_USED
C =                    c
C_COMPILER =           $(GCC)
C_FLAGS =              $(COMPILE_OPTS) -DUSE_OUR_BZERO=1 -D__MINGW32__
CPP =                  cpp
CPLUSPLUS_COMPILER =   $(CXX)
CPLUSPLUS_FLAGS =      $(COMPILE_OPTS) -D__MINGW32__ -Wall -Wno-deprecated
OBJ =                  o
LINK =                 $(CXX) -o 
LINK_OPTS =            -L.
CONSOLE_LINK_OPTS =    $(LINK_OPTS)
LIBRARY_LINK =         $(LD) 
LIBRARY_LINK_OPTS =    $(LINK_OPTS) -r -Bstatic
LIB_SUFFIX =                   a
LIBS_FOR_CONSOLE_APPLICATION = -lws2_32
LIBS_FOR_GUI_APPLICATION = -lws2_32
EXE =
##### End of variables to change

PROXY_SERVER = live555ProxyServer$(EXE)

PREFIX = /usr/local
ALL = $(PROXY_SERVER)
all: $(ALL)

.$(C).$(OBJ):
	$(C_COMPILER) -c $(C_FLAGS) $<
.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

PROXY_SERVER_OBJS = live555ProxyServer.$(OBJ)

USAGE_ENVIRONMENT_DIR = ../UsageEnvironment
USAGE_ENVIRONMENT_LIB = $(USAGE_ENVIRONMENT_DIR)/libUsageEnvironment.$(libUsageEnvironment_LIB_SUFFIX)
BASIC_USAGE_ENVIRONMENT_DIR = ../BasicUsageEnvironment
BASIC_USAGE_ENVIRONMENT_LIB = $(BASIC_USAGE_ENVIRONMENT_DIR)/libBasicUsageEnvironment.$(libBasicUsageEnvironment_LIB_SUFFIX)
LIVEMEDIA_DIR = ../liveMedia
LIVEMEDIA_LIB = $(LIVEMEDIA_DIR)/libliveMedia.$(libliveMedia_LIB_SUFFIX)
GROUPSOCK_DIR = ../groupsock
GROUPSOCK_LIB = $(GROUPSOCK_DIR)/libgroupsock.$(libgroupsock_LIB_SUFFIX)
LOCAL_LIBS =	$(LIVEMEDIA_LIB) $(GROUPSOCK_LIB) \
		$(BASIC_USAGE_ENVIRONMENT_LIB) $(USAGE_ENVIRONMENT_LIB)
LIBS =			$(LOCAL_LIBS) $(LIBS_FOR_CONSOLE_APPLICATION)

live555ProxyServer$(EXE):	$(PROXY_SERVER_OBJS) $(LOCAL_LIBS)
	$(LINK)$@ $(CONSOLE_LINK_OPTS) $(PROXY_SERVER_OBJS) $(LIBS)

clean:
	-rm -rf *.$(OBJ) $(ALL) core *.core *~ include/*~

install: $(PROXY_SERVER)
	  install -d $(DESTDIR)$(PREFIX)/bin
	  install -m 755 $(PROXY_SERVER) $(DESTDIR)$(PREFIX)/bin

##### Any additional, platform-specific rules come here:
