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

# Programs
MKF = make -f
CC = gcc
RM = rm -f
CP = cp

# object code
SRCDIR = src/
OBJDIR = obj/generic/
SOURCES = src/console/arguments.c src/calculator.c src/commands.c \
          src/console/error.c src/console/help.c src/operators.c src/parser.c \
          src/console/main.c src/stack.c src/tokens.c src/variables.c
OBJECTS = $(subst .c,.o,$(subst $(SRCDIR),$(OBJDIR),$(SOURCES)))

# Git command to make a distribution tarball.
GIT_ARCHIVE = git archive --format=tar --prefix=rpn-$(VERSION)/ HEAD | \
	bzip2 >rpn-$(VERSION).tar.bz2

# Installation directory
INSTALL_DIR = /usr/bin

# Executable generation directory
BIN_DIR = bin/generic

# Compilation options
ifdef RELEASE
CFLAGS = -c -s -Wall -O2 -DRPN_CONSOLE -DRPN_LONG_DOUBLE -D_GNU_SOURCE \
         -Isrc/include -o
endif
ifdef DEBUG
CFLAGS = -c -Wall -g -DRPN_CONSOLE -DRPN_LONG_DOUBLE -D_GNU_SOURCE -DRPN_DEBUG \
         -Isrc/include -o
endif
LFLAGS = -s -lm -o
TARGET = $(BIN_DIR)/rpn

# General rule for compiling.
$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $< $(CFLAGS) $@

# rule to make the program
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) $@

# make the program by default
.PHONY: all
all: $(TARGET)

# rule to clean-up the objects and the target, if they exist
.PHONY: clean
clean:
	$(RM) $(OBJECTS)
	$(RM) $(TARGET)

# rule to install program
.PHONY: install
install: $(TARGET)
	$(CP) $< $(INSTALL_DIR)

# rule to make tarball for distribution.
.PHONY: dist
dist:
	$(GIT_ARCHIVE)

# rule to make documentation
.PHONY: doc
doc:
	doxygen Doxyfile

# PSP compilation.
PSP_TARGET  = EBOOT.PBP
PSP_MAKEFILE = Makefile.psp

# Call the PSP makefile to do the work.
$(PSP_TARGET): $(PSP_MAKEFILE)
	$(MKF) $<

# Make the executable.
.PHONY: psp
psp: $(PSP_TARGET)

# Cleanup.
.PHONY: psp-clean
psp-clean:
	$(MKF) $(PSP_MAKEFILE) clean

.PHONY: psp-install
psp-install:
	$(MKF) $(PSP_MAKEFILE) install

# GUI compilation.
GUI_TARGET = $(BIN_DIR)/rpn-gui
GUI_MAKEFILE = Makefile.gui

$(GUI_TARGET): $(GUI_MAKEFILE)
	$(MKF) $<

.PHONY: gui
gui: $(GUI_TARGET)

