# Makefile for rpn.
# Should be fairly portable for the various *nix systems. The rules
# themselves shouldn't need to be changed; only the variables may need to
# change.
VERSION = $(shell cat VERSION)

# Programs
MKF = make -f
CC = gcc
RM = rm -f
CP = cp
TAR = tar cjf 
LN = ln -s . 
BZ = bzip2
MKDIR = mkdir -p

# object code
SRCDIR = src/
OBJDIR = obj/
SOURCES = src/arguments.c src/calculator.c src/commands.c src/error.c \
		  src/help.c src/operators.c src/parser.c src/rpn.c src/stack.c \
		  src/tokens.c src/variables.c
OBJECTS = $(subst .c,.o,$(subst $(SRCDIR),$(OBJDIR),$(SOURCES)))

# Files and directories for "make dist"
GIT_ARCHIVE = git archive --format=tar --prefix=rpn-$(VERSION)/ HEAD | \
	bzip2 >rpn-$(VERSION).tar.bz2

# Installation directory
INSTALL_DIR = /usr/bin

# Compilation options
CFLAGS = -c -s -Wall -O2 -DRPN_LONG_DOUBLE -D_GNU_SOURCE -Isrc/include -o
LFLAGS = -s -lm -o
TARGET = rpn

obj/%.o: src/%.c
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

.PHONY: distclean
distclean: clean
	$(RM) $(TARFILE)

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
