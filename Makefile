#############################################################################
# Makefile for building: libutils.so.1.0.0
# Project:  libutils.pro
# Template: lib
#############################################################################

MAKEFILE      = Makefile

####### Compiler, tools and options

CFLAGS        = -m64 -pipe -g -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -m64 -pipe -g -std=c++0x -Wall -W -fPIC -fvisibility=hidden $(DEFINES)
INCPATH       = -I. -I.. -I/usr/include -I/usr/local/include
LINK          = g++
CC            = gcc
CXX           = g++
DEFINES       = -DLIBRARY_BUILD
DEL_FILE      = rm -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
RMDIR         = rm -rf
COPY          = cp -f
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
INSTALL_FILE  = install -m 644 -p
INSTALL_PROGRAM = install -m 755 -p
INSTALL_DIR   = cp -f -R
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
TAR           = tar -cf
COMPRESS      = gzip -9f
DISTNAME      = utils1.0.0
DISTDIR       = /integra/usr/lib/utils1.0.0
LFLAGS        = -m64 -shared -Wl,-soname,libutils.so.
LIBS          = $(SUBLIBS) -L$(DESTDIR) -L/usr/lib -L/usr/lib/x86_64-linux-gnu/ -ljson -lmosquittopp
AR            = ar cqs
RANLIB        =
SED           = sed
CAT           = cat
STRIP         = strip
VER_FILE      = VERSION
VERSION       = $(shell cat ${VER_FILE})
VERSION_MINOR = $(shell cat ${VER_FILE} | sed 's/\.[0-9]*$$//')
VERSION_MAJOR = $(shell cat ${VER_FILE} | sed 's/\.[0-9]*\.[0-9]*//')

####### Output directory

OBJECTS_DIR   = obj/
DEPLOYDIR     = package/usr/lib/
DESTDIR       = ../lib/
TESTDIR       = ../bin/

####### Files

SOURCES = jsonreport.cpp \
		logger.cpp \
		mossie.cpp \
		stringutils.cpp
OBJECTS = $(OBJECTS_DIR)jsonreport.o \
		$(OBJECTS_DIR)logger.o \
		$(OBJECTS_DIR)mossie.o \
		$(OBJECTS_DIR)stringutils.o \
		$(OBJECTS_DIR)datetimeutils.o

TESTOBJECTS   = $(OBJECTS_DIR)jsonreport_ut.o \
		$(OBJECTS_DIR)stringutils_ut.o \
		$(OBJECTS_DIR)mossie_ut.o \
		$(OBJECTS_DIR)logger_ut.o \
		$(OBJECTS_DIR)datetimeutils_ut.o \
		$(OBJECTS_DIR)main.o

DIST          = jsonreport.h \
		stringutils.h \
		def.h \
		jsonreport.cpp \
		logger.cpp \
		mossie.cpp \
		stringutils.cpp
TARGET        = libutils.so.$(VERSION)
TARGETA       = $(DESTDIR)libutils.a
TARGET0       = libutils.so
TARGETD       = libutils.so.$(VERSION)
TARGET1       = libutils.so.$(VERSION_MAJOR)
TARGET2       = libutils.so.$(VERSION_MINOR)
TESTTARGET    = utilstest

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(DEPLOYDIR)$(TARGET):  $(OBJECTS) $(SUBLIBS) $(OBJCOMP)
	@test -d $(DEPLOYDIR) || $(MKDIR) -p $(DEPLOYDIR)
	@test -d $(DESTDIR) || $(MKDIR) -p $(DESTDIR)
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS)$(VERSION_MINOR) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)
	-ln -s $(TARGET) $(TARGET0)
	-ln -s $(TARGET) $(TARGET1)
	-ln -s $(TARGET) $(TARGET2)
	-$(DEL_FILE) $(DESTDIR)$(TARGET)
	-$(DEL_FILE) $(DEPLOYDIR)$(TARGET)
	-$(DEL_FILE) $(DEPLOYDIR)$(TARGET0)
	-$(DEL_FILE) $(DEPLOYDIR)$(TARGET1)
	-$(DEL_FILE) $(DEPLOYDIR)$(TARGET2)
	-$(MOVE) $(TARGET)  $(DEPLOYDIR)
	-$(MOVE) $(TARGET0) $(DEPLOYDIR)
	-$(MOVE) $(TARGET1) $(DEPLOYDIR)
	-$(MOVE) $(TARGET2) $(DEPLOYDIR)
	-ln -s -f $(CURDIR)/$(DEPLOYDIR)$(TARGET) $(DESTDIR)$(TARGET)
	-ln -s -f $(CURDIR)/$(DEPLOYDIR)$(TARGET) $(DESTDIR)$(TARGET0)
	-ln -s -f $(CURDIR)/$(DEPLOYDIR)$(TARGET) $(DESTDIR)$(TARGET1)
	-ln -s -f $(CURDIR)/$(DEPLOYDIR)$(TARGET) $(DESTDIR)$(TARGET2)

$(TESTTARGET):  $(TESTOBJECTS)
	$(LINK) $(LFLAGS) -o $(TESTTARGET) $(TESTOBJECTS) $(OBJCOMP) $(LIBS)
	@test -d $(TESTDIR) || $(MKDIR) $(TESTDIR)
	-$(DEL_FILE) $(TESTDIR)$(TESTTARGET)
	-$(MOVE) $(TESTTARGET)  $(TESTDIR)

staticlib: $(TARGETA)

$(TARGETA):  $(OBJECTS) $(OBJCOMP)
	-$(DEL_FILE) $(TARGETA)
	$(AR) $(TARGETA) $(OBJECTS)

$(OBJECTS_DIR): FORCE
	@test -d $(OBJECTS_DIR) || $(MKDIR) $(OBJECTS_DIR)

$(TESTDIR): FORCE
	@test -d $(TESTDIR) || $(MKDIR) $(TESTDIR)

$(DESTDIR): FORCE
	@test -d $(DESTDIR) || $(MKDIR) $(DESTDIR)

all: $(DESTDIR) $(OBJECTS_DIR) $(DEPLOYDIR)$(TARGET) $(DESTDIR)$(TARGET)

test: LFLAGS      = -m64
test: DEFINES     = -DUNITTEST
test: CFLAGS      = -m64 -pipe -g -Wall -W -fPIC  $(DEFINES)
test: CXXFLAGS    = -m64 -pipe -g -std=c++0x -Wall -W -fPIC -fvisibility=default $(DEFINES)
test: LIBS       += -lutils
test: $(TESTDIR) $(OBJECTS_DIR) $(TESTTARGET)
	-$(TESTDIR)$(TESTTARGET)

dist: distdir FORCE
	(cd `dirname $(DISTDIR)` && $(TAR) $(DISTNAME).tar $(DISTNAME) && $(COMPRESS) $(DISTNAME).tar) && $(MOVE) `dirname $(DISTDIR)`/$(DISTNAME).tar.gz . && $(DEL_FILE) -r $(DISTDIR)

distdir: FORCE
	@test -d $(DISTDIR) || $(MKDIR) -p $(DISTDIR)
	$(COPY_FILE) --parents $(DIST) $(DISTDIR)/

clean: compiler_clean
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core
	-$(RMDIR) $(OBJECTS_DIR)

distclean: clean
	-$(DEL_FILE) $(DESTDIR)$(TARGET)
	-$(DEL_FILE) $(DESTDIR)$(TARGET0) $(DESTDIR)$(TARGET1) $(DESTDIR)$(TARGET2) $(TARGETA)
	-$(DEL_FILE) Makefile

#arm: SDK_PATH        = "/opt/buildroot-toolchain"
arm: SDK_PATH        = "/opt/bin/"
arm: SDK_PATH_TARGET = $(SDK_PATH)/usr/arm-buildroot-linux-gnueabi/sysroot
arm: SDK_PATH_NATIVE = $(SDK_PATH)
#arm: TOOLCHAIN_PREFIX=arm-buildroot-linux-gnueabi-
arm: TOOLCHAIN_PREFIX=
arm: CFLAGS          = -pipe -g -Wall -W -O3 -fPIC  -march=armv7-a -marm -mthumb-interwork -mfloat-abi=soft -mfpu=neon -mtune=cortex-a9 #--sysroot=$(SDK_PATH_TARGET) $(DEFINES)
arm: CXXFLAGS        = -pipe -g -std=c++0x -Wall -W -O3 -fPIC -fvisibility=hidden -march=armv7-a -marm -mthumb-interwork -mfloat-abi=soft -mfpu=neon -mtune=cortex-a9 --sysroot=$(SDK_PATH_TARGET) $(DEFINES)
arm: LFLAGS          = -shared -Wl,-soname,libutils.so.
#arm: INCPATH         = -I. -I.. -I$(SDK_PATH_TARGET)/usr/include
arm: INCPATH         = -I. -I.. -I/opt/include
arm: LINK            = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)g++
arm: CC              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)gcc
arm: CXX             = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)g++
arm: NM              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)nm
arm: AS              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)as
arm: AR              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)ar
arm: RANLIB          = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)ranlib
arm: STRIP           = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)strip
#arm: LIBS            = $(SUBLIBS) -L$(SDK_PATH_TARGET)/lib/ -ljson-c  -lmosquittopp
arm: LIBS            = $(SUBLIBS) -L/lib/ -ljson-c  -lmosquittopp
arm: DEPLOYDIR       = package.arm/integra/lib/
arm: $(DESTDIR) $(OBJECTS_DIR) $(DEPLOYDIR)$(TARGET) $(DESTDIR)$(TARGET)
	$(STRIP) -s $(DEPLOYDIR)$(TARGET)

#arm-debug: SDK_PATH        = "/opt/buildroot-toolchain"
arm-debug: SDK_PATH        = "/opt/bin/"
arm-debug: SDK_PATH_TARGET = $(SDK_PATH)/usr/arm-buildroot-linux-gnueabi/sysroot
arm-debug: SDK_PATH_NATIVE = $(SDK_PATH)
#arm-debug: TOOLCHAIN_PREFIX=arm-buildroot-linux-gnueabi-
arm-debug: TOOLCHAIN_PREFIX=
arm-debug: CFLAGS          = -pipe -g -Wall -W -fPIC  -march=armv7-a -marm -mthumb-interwork -mfloat-abi=soft -mfpu=neon -mtune=cortex-a9 #--sysroot=$(SDK_PATH_TARGET) $(DEFINES)
arm-debug: CXXFLAGS        = -pipe -g -std=c++0x -Wall -W -fPIC -fvisibility=hidden -march=armv7-a -marm -mthumb-interwork -mfloat-abi=soft -mfpu=neon -mtune=cortex-a9 --sysroot=$(SDK_PATH_TARGET) $(DEFINES)
arm-debug: LFLAGS          = -shared -Wl,-soname,libutils.so.
#arm-debug: INCPATH         = -I. -I.. -I$(SDK_PATH_TARGET)/usr/include
arm-debug: INCPATH         = -I. -I.. -I/opt/include
arm-debug: LINK            = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)g++
arm-debug: CC              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)gcc
arm-debug: CXX             = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)g++
arm-debug: NM              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)nm
arm-debug: AS              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)as
arm-debug: AR              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)ar
arm-debug: RANLIB          = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)ranlib
arm-debug: STRIP           = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)strip
#arm-debug: LIBS            = $(SUBLIBS) -L$(SDK_PATH_TARGET)/lib/ -ljson-c  -lmosquittopp
arm-debug: LIBS            = $(SUBLIBS) -L/lib/ -ljson-c  -lmosquittopp
arm-debug: DEPLOYDIR       = package.arm/integra/lib/
arm-debug: $(DESTDIR) $(OBJECTS_DIR) $(DEPLOYDIR)$(TARGET) $(DESTDIR)$(TARGET)

#armtest: SDK_PATH        = "/opt/buildroot-toolchain"
armtest: SDK_PATH        = "/opt/bin/"
armtest: SDK_PATH_TARGET = $(SDK_PATH)/usr/arm-buildroot-linux-gnueabi/sysroot
armtest: SDK_PATH_NATIVE = $(SDK_PATH)
#armtest: TOOLCHAIN_PREFIX=arm-buildroot-linux-gnueabi-
armtest: TOOLCHAIN_PREFIX=
#armtest: INCPATH        += -I$(SDK_PATH_TARGET)/usr/include
armtest: INCPATH        += -I/opt/include
armtest: LINK            = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)g++
armtest: CC              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)gcc
armtest: CXX             = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)g++
armtest: NM              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)nm
armtest: AS              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)as
armtest: AR              = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)ar
armtest: RANLIB          = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)ranlib
armtest: STRIP           = $(SDK_PATH_NATIVE)$(TOOLCHAIN_PREFIX)strip
#armtest: LIBS            = $(SUBLIBS) -L$(DESTDIR) -L$(SDK_PATH_TARGET)/lib/ -lutils -ljson-c -lmosquittopp
armtest: LIBS            = $(SUBLIBS) -L$(DESTDIR) -L/lib/ -lutils -ljson-c -lmosquittopp
armtest: LFLAGS          =
armtest: DEFINES        += -DUNITTEST
armtest: CFLAGS          = -pipe -g -Wall -W -fPIC  -march=armv7-a -marm -mthumb-interwork -mfloat-abi=soft -mfpu=neon -mtune=cortex-a9 --sysroot=$(SDK_PATH_TARGET) $(DEFINES)
armtest: CXXFLAGS        = -pipe -g -std=c++0x -Wall -W -fPIC -fvisibility=hidden -march=armv7-a -marm -mthumb-interwork -mfloat-abi=soft -mfpu=neon -mtune=cortex-a9 --sysroot=$(SDK_PATH_TARGET) $(DEFINES)
armtest: $(TESTDIR) $(OBJECTS_DIR) $(TESTTARGET)
	-$(TESTDIR)$(TESTTARGET)


####### Sub-libraries

check: first

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean:

CATCH_FILES = ../Catch/include/internal/catch_suppress_warnings.h \
		../Catch/include/internal/catch_notimplemented_exception.h \
		../Catch/include/internal/catch_common.h \
		../Catch/include/internal/catch_compiler_capabilities.h \
		../Catch/include/internal/catch_context.h \
		../Catch/include/internal/catch_interfaces_generators.h \
		../Catch/include/internal/catch_ptr.hpp \
		../Catch/include/internal/catch_test_registry.hpp \
		../Catch/include/internal/catch_interfaces_testcase.h \
		../Catch/include/internal/catch_capture.hpp \
		../Catch/include/internal/catch_result_builder.h \
		../Catch/include/internal/catch_result_type.h \
		../Catch/include/internal/catch_assertionresult.h \
		../Catch/include/internal/catch_matchers.hpp \
		../Catch/include/internal/catch_expression_lhs.hpp \
		../Catch/include/internal/catch_evaluate.hpp \
		../Catch/include/internal/catch_tostring.h \
		../Catch/include/internal/catch_objc_arc.hpp \
		../Catch/include/internal/catch_message.h \
		../Catch/include/internal/catch_interfaces_capture.h \
		../Catch/include/internal/catch_debugger.h \
		../Catch/include/internal/catch_platform.h \
		../Catch/include/internal/catch_interfaces_runner.h \
		../Catch/include/internal/catch_section.h \
		../Catch/include/internal/catch_section_info.h \
		../Catch/include/internal/catch_totals.hpp \
		../Catch/include/internal/catch_timer.h \
		../Catch/include/internal/catch_generators.hpp \
		../Catch/include/internal/catch_interfaces_exception.h \
		../Catch/include/internal/catch_interfaces_registry_hub.h \
		../Catch/include/internal/catch_approx.hpp \
		../Catch/include/internal/catch_interfaces_tag_alias_registry.h \
		../Catch/include/internal/catch_tag_alias.h \
		../Catch/include/internal/catch_option.hpp \
		../Catch/include/internal/catch_test_case_info.h \
		../Catch/include/internal/catch_objc.hpp \
		../Catch/include/internal/catch_impl.hpp \
		../Catch/include/catch_session.hpp \
		../Catch/include/internal/catch_commandline.hpp \
		../Catch/include/internal/catch_config.hpp \
		../Catch/include/internal/catch_test_spec_parser.hpp \
		../Catch/include/internal/catch_test_spec.hpp \
		../Catch/include/internal/catch_wildcard_pattern.hpp \
		../Catch/include/internal/catch_interfaces_config.h \
		../Catch/include/internal/catch_stream.h \
		../Catch/include/internal/catch_streambuf.h \
		../Catch/include/internal/catch_clara.h \
		../Catch/include/external/clara.h \
		../Catch/include/internal/catch_list.hpp \
		../Catch/include/internal/catch_text.h \
		../Catch/include/external/tbc_text_format.h \
		../Catch/include/internal/catch_console_colour.hpp \
		../Catch/include/internal/catch_interfaces_reporter.h \
		../Catch/include/internal/catch_run_context.hpp \
		../Catch/include/internal/catch_test_case_tracker.hpp \
		../Catch/include/internal/catch_fatal_condition.hpp \
		../Catch/include/internal/catch_version.h \
		../Catch/include/internal/catch_registry_hub.hpp \
		../Catch/include/internal/catch_test_case_registry_impl.hpp \
		../Catch/include/internal/catch_reporter_registry.hpp \
		../Catch/include/internal/catch_exception_translator_registry.hpp \
		../Catch/include/internal/catch_notimplemented_exception.hpp \
		../Catch/include/internal/catch_context_impl.hpp \
		../Catch/include/internal/catch_stream.hpp \
		../Catch/include/internal/catch_console_colour_impl.hpp \
		../Catch/include/internal/catch_generators_impl.hpp \
		../Catch/include/internal/catch_assertionresult.hpp \
		../Catch/include/internal/catch_test_case_info.hpp \
		../Catch/include/internal/catch_version.hpp \
		../Catch/include/internal/catch_message.hpp \
		../Catch/include/internal/catch_legacy_reporter_adapter.hpp \
		../Catch/include/internal/catch_legacy_reporter_adapter.h \
		../Catch/include/internal/catch_timer.hpp \
		../Catch/include/internal/catch_common.hpp \
		../Catch/include/internal/catch_section.hpp \
		../Catch/include/internal/catch_debugger.hpp \
		../Catch/include/internal/catch_tostring.hpp \
		../Catch/include/internal/catch_result_builder.hpp \
		../Catch/include/internal/catch_tag_alias_registry.hpp \
		../Catch/include/internal/catch_tag_alias_registry.h \
		../Catch/include/reporters/catch_reporter_multi.hpp \
		../Catch/include/reporters/catch_reporter_xml.hpp \
		../Catch/include/reporters/catch_reporter_bases.hpp \
		../Catch/include/internal/catch_reporter_registrars.hpp \
		../Catch/include/internal/catch_xmlwriter.hpp \
		../Catch/include/internal/catch_reenable_warnings.h \
		../Catch/include/reporters/catch_reporter_junit.hpp \
		../Catch/include/reporters/catch_reporter_console.hpp \
		../Catch/include/reporters/catch_reporter_compact.hpp \
		../Catch/include/internal/catch_default_main.hpp

####### Compile

$(OBJECTS_DIR)jsonreport.o: jsonreport.cpp jsonreport.h \
		stringutils.h \
		def.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)jsonreport.o jsonreport.cpp

$(OBJECTS_DIR)logger.o: logger.cpp logger.h \
		def.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)logger.o logger.cpp

$(OBJECTS_DIR)mossie.o: mossie.cpp mossie.h \
		def.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)mossie.o mossie.cpp

$(OBJECTS_DIR)jsonreport_ut.o: jsonreport_ut.cpp jsonreport.h \
		$(CATCHFILES)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)jsonreport_ut.o jsonreport_ut.cpp

$(OBJECTS_DIR)stringutils_ut.o: stringutils_ut.cpp stringutils.h $(CATCHFILES)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)stringutils_ut.o stringutils_ut.cpp

$(OBJECTS_DIR)mossie_ut.o: mossie_ut.cpp $(CATCHFILES)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)mossie_ut.o mossie_ut.cpp

$(OBJECTS_DIR)logger_ut.o: logger_ut.cpp $(CATCHFILES)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)logger_ut.o logger_ut.cpp

$(OBJECTS_DIR)datetimeutils_ut.o: datetimeutils_ut.cpp datetimeutils.h $(CATCHFILES)
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)datetimeutils_ut.o datetimeutils_ut.cpp

$(OBJECTS_DIR)main.o: main.cpp ../Catch/single_include/catch.hpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)main.o main.cpp

$(OBJECTS_DIR)stringutils.o: stringutils.cpp stringutils.h def.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)stringutils.o stringutils.cpp

$(OBJECTS_DIR)datetimeutils.o: datetimeutils.cpp datetimeutils.h def.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)datetimeutils.o datetimeutils.cpp

####### Install

install_target: first FORCE
	@test -d $(INSTALL_ROOT)/usr/lib || $(MKDIR) $(INSTALL_ROOT)/usr/lib
	-$(INSTALL_PROGRAM) $(DESTDIR)$(TARGET) $(INSTALL_ROOT)/usr/lib/$(TARGET)
	-$(SYMLINK) $(TARGET) $(INSTALL_ROOT)/usr/lib/$(TARGET0)
	-$(SYMLINK) $(TARGET) $(INSTALL_ROOT)/usr/lib/$(TARGET1)
	-$(SYMLINK) $(TARGET) $(INSTALL_ROOT)/usr/lib/$(TARGET2)

uninstall_target: FORCE
	-$(DEL_FILE) $(INSTALL_ROOT)/usr/lib/$(TARGET)
	-$(DEL_FILE) $(INSTALL_ROOT)/usr/lib/$(TARGET0)
	-$(DEL_FILE) $(INSTALL_ROOT)/usr/lib/$(TARGET1)
	-$(DEL_FILE) $(INSTALL_ROOT)/usr/lib/$(TARGET2)
	-$(DEL_DIR) $(INSTALL_ROOT)/usr/lib/


install: install_target  FORCE

uninstall: uninstall_target  FORCE

FORCE:

