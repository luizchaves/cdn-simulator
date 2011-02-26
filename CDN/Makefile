#
# OMNeT++/OMNEST Makefile for CDN
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -L../inet/out/$(CONFIGNAME)/src -linet -KINET_PROJ=../inet
#

# Name of target to be created (-o option)
TARGET = CDN$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -Isrc \
    -Isrc/cdn \
    -Isrc/cdn/builder \
    -Isrc/cdn/netBuilder \
    -Isrc/cdn/networks \
    -Isrc/cdn/node \
    -Isrc/cdn/results

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../inet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/src/cdn/builder/netbuilder.o \
    $O/src/cdn/node/Storage.o \
    $O/src/cdn/node/Processador.o \
    $O/src/cdn/node/Refletor.o \
    $O/src/cdn/node/Indexador.o \
    $O/src/cdn/node/Client.o

# Message files
MSGFILES =

# Other makefile variables (-K)
INET_PROJ=../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f CDN CDN.exe libCDN.so libCDN.a libCDN.dll libCDN.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f src/*_m.cc src/*_m.h
	-rm -f src/cdn/*_m.cc src/cdn/*_m.h
	-rm -f src/cdn/builder/*_m.cc src/cdn/builder/*_m.h
	-rm -f src/cdn/netBuilder/*_m.cc src/cdn/netBuilder/*_m.h
	-rm -f src/cdn/networks/*_m.cc src/cdn/networks/*_m.h
	-rm -f src/cdn/node/*_m.cc src/cdn/node/*_m.h
	-rm -f src/cdn/results/*_m.cc src/cdn/results/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc src/*.cc src/cdn/*.cc src/cdn/builder/*.cc src/cdn/netBuilder/*.cc src/cdn/networks/*.cc src/cdn/node/*.cc src/cdn/results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/cdn/builder/netbuilder.o: src/cdn/builder/netbuilder.cc
$O/src/cdn/node/Processador.o: src/cdn/node/Processador.cc \
	src/cdn/node/Processador.h
$O/src/cdn/node/Indexador.o: src/cdn/node/Indexador.cc \
	src/cdn/node/Indexador.h
$O/src/cdn/node/Storage.o: src/cdn/node/Storage.cc \
	src/cdn/node/Storage.h
$O/src/cdn/node/Refletor.o: src/cdn/node/Refletor.cc \
	src/cdn/node/Refletor.h
$O/src/cdn/node/Client.o: src/cdn/node/Client.cc \
	src/cdn/node/Client.h

