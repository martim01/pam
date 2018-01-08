INCLUDES = -Iinclude -I../groupsock/include
PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib
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

NAME = libUsageEnvironment
USAGE_ENVIRONMENT_LIB = $(NAME).$(LIB_SUFFIX)
ALL = $(USAGE_ENVIRONMENT_LIB)
all:	$(ALL)

OBJS = UsageEnvironment.$(OBJ) HashTable.$(OBJ) strDup.$(OBJ)

$(USAGE_ENVIRONMENT_LIB): $(OBJS)
	$(LIBRARY_LINK)$@ $(LIBRARY_LINK_OPTS) $(OBJS)

.$(C).$(OBJ):
	$(C_COMPILER) -c $(C_FLAGS) $<       

.$(CPP).$(OBJ):
	$(CPLUSPLUS_COMPILER) -c $(CPLUSPLUS_FLAGS) $<

UsageEnvironment.$(CPP):	include/UsageEnvironment.hh
include/UsageEnvironment.hh:	include/UsageEnvironment_version.hh include/Boolean.hh include/strDup.hh
HashTable.$(CPP):		include/HashTable.hh
include/HashTable.hh:		include/Boolean.hh
strDup.$(CPP):			include/strDup.hh

clean:
	-rm -rf *.$(OBJ) $(ALL) core *.core *~ include/*~

install: install1 $(INSTALL2)
install1: $(USAGE_ENVIRONMENT_LIB)
	  install -d $(DESTDIR)$(PREFIX)/include/UsageEnvironment $(DESTDIR)$(LIBDIR)
	  install -m 644 include/*.hh $(DESTDIR)$(PREFIX)/include/UsageEnvironment
	  install -m 644 $(USAGE_ENVIRONMENT_LIB) $(DESTDIR)$(LIBDIR)
install_shared_libraries: $(USAGE_ENVIRONMENT_LIB)
	  ln -fs $(NAME).$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/$(NAME).$(SHORT_LIB_SUFFIX)
	  ln -fs $(NAME).$(LIB_SUFFIX) $(DESTDIR)$(LIBDIR)/$(NAME).so

##### Any additional, platform-specific rules come here:
