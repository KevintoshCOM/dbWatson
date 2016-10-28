#dbWatson Makefile
.PHONY: all clean

CC=g++
CFLAGS=-c -Wall
TARGET=dbWatson

SRCDIR=src
INCDIR=include
OBJDIR=obj
BINDIR=bin

SOURCES=$(wildcard $(SRCDIR)/*.cpp)
OBJFILES=$(subst $(SRCDIR),$(OBJDIR),$(SOURCES:.cpp=.o))

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJFILES)
	mkdir -p $(BINDIR)
	$(CC) $(OBJFILES) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR)  $< -o $@

clean:
	rm -r -f $(OBJDIR)
	rm -r -f $(BINDIR)
