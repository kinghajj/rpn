# Makefile for rpn.
# Should be fairly portable for the various *nix systems. The rules
# themselves shouldn't need to be changed; only the variables may need to
# change.

# Automatically default to a release build.
ifndef DEBUG
RELEASE = 1
endif

# Version
VERSION = $(shell cat VERSION)
GIT_BUILD = $(shell git describe)

# Git command to make a distribution tarball.
GIT_ARCHIVE = git archive --format=tar --prefix=rpn-$(VERSION)/ HEAD | \
	bzip2 >rpn-$(VERSION).tar.bz2

ifdef RELEASE
CXXFLAGS = -Wall -Weffc++ -pedantic -O2 -DRPN_CONSOLE -DRPN_LONG_DOUBLE \
		   -DGIT_BUILD="\"$(GIT_BUILD)\""
LFLAGS = -s -lm -o
endif
ifdef DEBUG
CXXFLAGS = -Wall -Weffc++ -pedantic -g -DRPN_CONSOLE -DRPN_LONG_DOUBLE \
		   -DGIT_BUILD=$(GIT_BUILD)
LFLAGS = -lm -o
endif

OBJDIR = obj/console/
SRCDIR = src/
TARGET = bin/console/rpn
OBJECTS = \
	$(OBJDIR)Calculator.o $(OBJDIR)Commands.o $(OBJDIR)Help.o \
	$(OBJDIR)History.o $(OBJDIR)Main.o $(OBJDIR)Operators.o \
	$(OBJDIR)Variables.o $(OBJDIR)console/Arguments.o

# make the program by default
.PHONY: all
all: $(TARGET)

# rule to clean-up the objects and the target, if they exist
.PHONY: clean
clean:
	@echo Cleaning objects and executables...
	@$(RM) $(OBJECTS) $(TARGET)

# General rule for compiling.
$(OBJDIR)%.o: $(SRCDIR)%.cpp $(SRCDIR)rpn.h
	@echo Compiling $(notdir $<)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# rule to make the program
$(TARGET): $(OBJECTS)
	@echo Linking $(TARGET)...
	@$(CXX) $(OBJECTS) $(LFLAGS) $@

# rule to make tarball for distribution.
.PHONY: dist
dist:
	$(GIT_ARCHIVE)

# rule to make documentation
.PHONY: doc
doc:
	doxygen Doxyfile

# PSP compilation.
PSP_TARGET   = EBOOT.PBP
PSP_MAKEFILE = Makefile.psp

# Call the PSP makefile to do the work.
$(PSP_TARGET): $(PSP_MAKEFILE)
	@$(MAKE) -f $<

# Make the executable.
.PHONY: psp
psp: $(PSP_TARGET)

# Cleanup.
.PHONY: psp-clean
psp-clean:
	@$(MAKE) -f $(PSP_MAKEFILE) clean

.PHONY: psp-install
psp-install:
	@$(MAKE) -f $(PSP_MAKEFILE) install

# Wii compilation.
WII_MAKEFILE = Makefile.wii

.PHONY: wii
wii: $(WII_MAKEFILE)
	@$(MAKE) -f $(WII_MAKEFILE)

.PHONY: wii-clean
wii-clean: $(WII_MAKEFILE)
	@$(MAKE) -f $(WII_MAKEFILE) clean

.PHONY: wii-run
wii-run: $(WII_MAKEFILE)
	@$(MAKE) -f $(WII_MAKEFILE) run

# An easter egg, just for the hell of it.
.PHONY: love
love:
	@echo Not war?
