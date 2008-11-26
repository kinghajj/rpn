CXXFLAGS = -Wall -pedantic -O2
SRCDIR = src/
TARGET = bin/rpn
OBJECTS = \
	$(SRCDIR)Calculator.o $(SRCDIR)Commands.o $(SRCDIR)History.o \
	$(SRCDIR)Main.o $(SRCDIR)Operators.o $(SRCDIR)Variables.o

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

$(SRCDIR)Calculator.o: $(SRCDIR)Calculator.cpp $(SRCDIR)rpn.h
$(SRCDIR)Commands.o:   $(SRCDIR)Commands.cpp   $(SRCDIR)rpn.h
$(SRCDIR)History.o:    $(SRCDIR)History.cpp    $(SRCDIR)rpn.h
$(SRCDIR)Main.o:       $(SRCDIR)Main.cpp       $(SRCDIR)rpn.h
$(SRCDIR)Operators.o:  $(SRCDIR)Operators.cpp  $(SRCDIR)rpn.h
$(SRCDIR)Variables.o:  $(SRCDIR)Variables.cpp  $(SRCDIR)rpn.h
