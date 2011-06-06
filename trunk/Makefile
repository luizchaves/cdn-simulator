#
# OMNeT++/OMNEST Makefile for cdn-simulator
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I../inet_old/src/transport/contract -I../inet_old/src/transport/udp -I../inet_old/src/networklayer/ipv6 -I../inet_old/src/networklayer/icmpv6 -I../inet_old/src/util -I../inet_old/src/networklayer/ipv4 -I../inet_old/src/base -I../inet_old/src/networklayer/arp -I../inet_old/src/networklayer/autorouting -I../inet_old/src/applications/pingapp -I../inet_old/src/networklayer/contract -I../inet_old/src/networklayer/common -I../inet_old/src/util/headerserializers -I../inet_old/src/networklayer/mpls -I../inet_old/src/networklayer/rsvp_te -I../inet_old/src/linklayer/mfcore -I../inet_old/src/linklayer/contract -I../inet_old/src/transport/sctp -I../inet_old/src/networklayer/ldp -I../inet_old/src/networklayer/ted -I../inet_old/src/transport/tcp -I../inet_old/src/world -L../inet_old/out/$(CONFIGNAME)/src -linet -KINET_OLD_PROJ=../inet_old
#

# Name of target to be created (-o option)
TARGET = cdn-simulator$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../inet_old/src/transport/contract \
    -I../inet_old/src/transport/udp \
    -I../inet_old/src/networklayer/ipv6 \
    -I../inet_old/src/networklayer/icmpv6 \
    -I../inet_old/src/util \
    -I../inet_old/src/networklayer/ipv4 \
    -I../inet_old/src/base \
    -I../inet_old/src/networklayer/arp \
    -I../inet_old/src/networklayer/autorouting \
    -I../inet_old/src/applications/pingapp \
    -I../inet_old/src/networklayer/contract \
    -I../inet_old/src/networklayer/common \
    -I../inet_old/src/util/headerserializers \
    -I../inet_old/src/networklayer/mpls \
    -I../inet_old/src/networklayer/rsvp_te \
    -I../inet_old/src/linklayer/mfcore \
    -I../inet_old/src/linklayer/contract \
    -I../inet_old/src/transport/sctp \
    -I../inet_old/src/networklayer/ldp \
    -I../inet_old/src/networklayer/ted \
    -I../inet_old/src/transport/tcp \
    -I../inet_old/src/world \
    -I. \
    -Isrc \
    -Isrc/cdn \
    -Isrc/cdn/builder \
    -Isrc/cdn/builder/configFile \
    -Isrc/cdn/builder/old \
    -Isrc/cdn/content \
    -Isrc/cdn/execption \
    -Isrc/cdn/message \
    -Isrc/cdn/networks \
    -Isrc/cdn/node \
    -Isrc/cdn/results

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../inet_old/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../inet_old/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/src/cdn/builder/NetConfigurator.o \
    $O/src/cdn/builder/netbuilderCDN.o \
    $O/src/cdn/builder/old/netbuilder.o \
    $O/src/cdn/content/Segment.o \
    $O/src/cdn/content/VideoSet.o \
    $O/src/cdn/content/Cache.o \
    $O/src/cdn/content/LruCache.o \
    $O/src/cdn/content/Video.o \
    $O/src/cdn/node/Storage.o \
    $O/src/cdn/node/Client.o \
    $O/src/cdn/node/Processor.o \
    $O/src/cdn/node/Indexer.o \
    $O/src/cdn/node/Reflector.o \
    $O/src/cdn/message/requestCDN_m.o

# Message files
MSGFILES = \
    src/cdn/message/requestCDN.msg

# Other makefile variables (-K)
INET_OLD_PROJ=../inet_old

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
	-rm -f cdn-simulator cdn-simulator.exe libcdn-simulator.so libcdn-simulator.a libcdn-simulator.dll libcdn-simulator.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f src/*_m.cc src/*_m.h
	-rm -f src/cdn/*_m.cc src/cdn/*_m.h
	-rm -f src/cdn/builder/*_m.cc src/cdn/builder/*_m.h
	-rm -f src/cdn/builder/configFile/*_m.cc src/cdn/builder/configFile/*_m.h
	-rm -f src/cdn/builder/old/*_m.cc src/cdn/builder/old/*_m.h
	-rm -f src/cdn/content/*_m.cc src/cdn/content/*_m.h
	-rm -f src/cdn/execption/*_m.cc src/cdn/execption/*_m.h
	-rm -f src/cdn/message/*_m.cc src/cdn/message/*_m.h
	-rm -f src/cdn/networks/*_m.cc src/cdn/networks/*_m.h
	-rm -f src/cdn/node/*_m.cc src/cdn/node/*_m.h
	-rm -f src/cdn/results/*_m.cc src/cdn/results/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc src/*.cc src/cdn/*.cc src/cdn/builder/*.cc src/cdn/builder/configFile/*.cc src/cdn/builder/old/*.cc src/cdn/content/*.cc src/cdn/execption/*.cc src/cdn/message/*.cc src/cdn/networks/*.cc src/cdn/node/*.cc src/cdn/results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/cdn/builder/netbuilderCDN.o: src/cdn/builder/netbuilderCDN.cc \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_OLD_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_OLD_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_OLD_PROJ)/src/base/ModuleAccess.h \
	src/cdn/builder/NetConfigurator.h \
	$(INET_OLD_PROJ)/src/networklayer/autorouting/FlatNetworkConfigurator.h \
	$(INET_OLD_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_OLD_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_OLD_PROJ)/src/base/NotifierConsts.h \
	$(INET_OLD_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_OLD_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_OLD_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_OLD_PROJ)/src/base/INotifiable.h \
	$(INET_OLD_PROJ)/src/base/NotificationBoard.h \
	$(INET_OLD_PROJ)/src/base/INETDefs.h
$O/src/cdn/builder/NetConfigurator.o: src/cdn/builder/NetConfigurator.cc \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_OLD_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_OLD_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_OLD_PROJ)/src/base/ModuleAccess.h \
	src/cdn/builder/NetConfigurator.h \
	$(INET_OLD_PROJ)/src/networklayer/autorouting/FlatNetworkConfigurator.h \
	$(INET_OLD_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_OLD_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_OLD_PROJ)/src/base/NotifierConsts.h \
	$(INET_OLD_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_OLD_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_OLD_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_OLD_PROJ)/src/base/INotifiable.h \
	$(INET_OLD_PROJ)/src/base/NotificationBoard.h \
	$(INET_OLD_PROJ)/src/base/INETDefs.h
$O/src/cdn/builder/old/netbuilder.o: src/cdn/builder/old/netbuilder.cc
$O/src/cdn/content/Video.o: src/cdn/content/Video.cc \
	src/cdn/content/Video.h \
	src/cdn/content/Segment.h
$O/src/cdn/content/Cache.o: src/cdn/content/Cache.cc \
	src/cdn/content/Cache.h \
	src/cdn/content/Video.h \
	src/cdn/content/Segment.h
$O/src/cdn/content/LruCache.o: src/cdn/content/LruCache.cc \
	src/cdn/content/Cache.h \
	src/cdn/content/LruCache.h \
	src/cdn/execption/Exceptions.h \
	src/cdn/content/Video.h \
	src/cdn/content/Segment.h
$O/src/cdn/content/Segment.o: src/cdn/content/Segment.cc \
	src/cdn/content/Segment.h
$O/src/cdn/content/VideoSet.o: src/cdn/content/VideoSet.cc \
	src/cdn/content/VideoSet.h \
	src/cdn/content/Video.h \
	src/cdn/content/Segment.h
$O/src/cdn/message/requestCDN_m.o: src/cdn/message/requestCDN_m.cc \
	src/cdn/message/requestCDN_m.h
$O/src/cdn/node/Indexer.o: src/cdn/node/Indexer.cc \
	src/cdn/content/Cache.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPv6Address.h \
	src/cdn/node/Storage.h \
	src/cdn/node/Indexer.h \
	src/cdn/node/Reflector.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPvXAddress.h \
	src/cdn/content/LruCache.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPAddress.h \
	src/cdn/content/VideoSet.h \
	src/cdn/content/Video.h \
	$(INET_OLD_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_OLD_PROJ)/src/transport/contract/UDPSocket.h \
	src/cdn/content/Segment.h \
	$(INET_OLD_PROJ)/src/base/INETDefs.h
$O/src/cdn/node/Storage.o: src/cdn/node/Storage.cc \
	src/cdn/content/VideoSet.h \
	src/cdn/content/Video.h \
	src/cdn/node/Storage.h \
	src/cdn/content/Segment.h
$O/src/cdn/node/Reflector.o: src/cdn/node/Reflector.cc \
	src/cdn/node/Reflector.h \
	src/cdn/content/Cache.h \
	src/cdn/content/LruCache.h \
	src/cdn/content/Video.h \
	src/cdn/content/Segment.h
$O/src/cdn/node/Client.o: src/cdn/node/Client.cc \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_OLD_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_OLD_PROJ)/src/networklayer/common/InterfaceTable.h \
	$(INET_OLD_PROJ)/src/base/ModuleAccess.h \
	$(INET_OLD_PROJ)/src/networklayer/autorouting/FlatNetworkConfigurator.h \
	src/cdn/builder/NetConfigurator.h \
	$(INET_OLD_PROJ)/src/networklayer/ipv4/IPRoute.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPvXAddress.h \
	src/cdn/message/requestCDN_m.h \
	$(INET_OLD_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_OLD_PROJ)/src/networklayer/ipv4/IRoutingTable.h \
	$(INET_OLD_PROJ)/src/base/NotifierConsts.h \
	$(INET_OLD_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_OLD_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_OLD_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_OLD_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_OLD_PROJ)/src/base/INotifiable.h \
	$(INET_OLD_PROJ)/src/base/NotificationBoard.h \
	src/cdn/node/Client.h \
	$(INET_OLD_PROJ)/src/transport/contract/UDPSocket.h \
	$(INET_OLD_PROJ)/src/base/INETDefs.h
$O/src/cdn/node/Processor.o: src/cdn/node/Processor.cc \
	src/cdn/node/Processor.h

