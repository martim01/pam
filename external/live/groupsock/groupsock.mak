INCLUDES = -Iinclude -I../UsageEnvironment/include
PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib
##### Change the following for your environment:
ODEBUG=1
TARGETOS = WINNT
UI_OPTS =       $(guilflags) $(guilibsdll)
CONSOLE_UI_OPTS =       $(conlflags) $(conlibsdll)
CPU=i386

TOOLS32 =       C:\developer\mingw\bin
COMPILE_OPTS =      $(INCLUDES) $(cdebug) $(cflags) $(cvarsdll) -I. -I"$(TOOLS32)\include"
C =         c
C_COMPILER =        "$(TOOLS32)\gcc.exe"
C_FLAGS =       $(COMPILE_OPTS)
CPP =           cpp
CPLUSPLUS_COMPILER =    "$(TOOLS32)\g++.exe"
CPLUSPLUS_FLAGS =   $(COMPILE_OPTS)
OBJ =           o
LINK =          $(CXX) -o 
LIBRARY_LINK =      $(LD) -o 
LIBRARY_LINK_OPTS = $(LINK_OPTS) -r -Bstatic
LINK_OPTS =     -L.
CONSOLE_LINK_OPTS = $(LINK_OPTS_0) $(CONSOLE_UI_OPTS)
SERVICE_LINK_OPTS =     kernel32.lib advapi32.lib shell32.lib -subsystem:console,$(APPVER)
LIB_SUFFIX =        lib
LIBS_FOR_CONSOLE_APPLICATION =
LIBS_FOR_GUI_APPLICATION =
MULTIMEDIA_LIBS =   winmm.lib
EXE =           .exe
PLATFORM = Windows

rc32 = "$(TOOLS32)\bin\rc"
.rc.res:
    $(rc32) $<##### End of variables to change

NAME = libgroupsock
ALL = $(NAME).$(LIB_SUFFIX)
all:	$(ALL)

.$(C).$(OBJ):
	$(C_COMPILER) -c $(C_FLAGS) $<
.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

GROUPSOCK_LIB_OBJS = GroupsockHelper.$(OBJ) GroupEId.$(OBJ) inet.$(OBJ) Groupsock.$(OBJ) NetInterface.$(OBJ) NetAddress.$(OBJ) IOHandlers.$(OBJ)

GroupsockHelper.$(CPP):	include/GroupsockHelper.hh
include/GroupsockHelper.hh:	include/NetAddress.hh
include/NetAddress.hh:	include/NetCommon.h
GroupEId.$(CPP):	include/GroupEId.hh
include/GroupEId.hh:	include/NetAddress.hh
inet.$(C):		include/NetCommon.h
Groupsock.$(CPP):	include/Groupsock.hh include/GroupsockHelper.hh include/TunnelEncaps.hh
include/Groupsock.hh:	include/groupsock_version.hh include/NetInterface.hh include/GroupEId.hh
include/NetInterface.hh:	include/NetAddress.hh
include/TunnelEncaps.hh:	include/NetAddress.hh
NetInterface.$(CPP):	include/NetInterface.hh include/GroupsockHelper.hh
NetAddress.$(CPP):	include/NetAddress.hh include/GroupsockHelper.hh
IOHandlers.$(CPP):	include/IOHandlers.hh include/TunnelEncaps.hh

libgroupsock.$(LIB_SUFFIX): $(GROUPSOCK_LIB_OBJS) \
    $(PLATFORM_SPECIFIC_LIB_OBJS)
	$(LIBRARY_LINK)$@ $(LIBRARY_LINK_OPTS) \
		$(GROUPSOCK_LIB_OBJS)

clean:
	-rm -rf *.$(OBJ) $(ALL) core *.core *~ include/*~

install: install1 $(INSTALL2)
install1: libgroupsock.$(LIB_SUFFIX)
	  install -d $(DESTDIR)$(PREFIX)/include/groupsock $(DESTDIR)$(LIBDIR)
	  install -m 644 include/*.hh include/*.h $(DESTDIR)$(PREFIX)/include/groupsock
	  install -m 644 libgroupsock.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)
install_shared_libraries: libgroupsock.$(LIB_SUFFIX)
	  ln -fs libgroupsock.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/libgroupsock.$(SHORT_LIB_SUFFIX)
	  ln -fs libgroupsock.$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/libgroupsock.so

##### Any additional, platform-specific rules come here:
