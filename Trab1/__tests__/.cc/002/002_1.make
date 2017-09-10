#:description: Compila dois códigos independentes
#:make: make -f 002_1.make
#:make-clean: make clean -f 002_1.make

# MANUAL https://www.gnu.org/software/make/manual/html_node

RM := rm -rf

CCX := g++
CFLAGS  := -c
LDFLAGS := -Wall
LDLIBS  := -lm

SOURCES1 := main1.cc
OBJECTS1 := $(SOURCES1:.cc=.o)
EXECUTABLE1 := main1.exe

SOURCES2 := main2.cc
OBJECTS2 := $(SOURCES2:.cc=.o)
EXECUTABLE2 := main2.exe

all: program1 program2


program1: $(SOURCES1)
	$(CCX) $(LDFLAGS) $^ -o $(EXECUTABLE1) $(LDLIBS)

program2: $(SOURCES2)
	$(CCX) $(LDFLAGS) $^ -o $(EXECUTABLE2) $(LDLIBS)


.cc.o:
	$(CCX) $(CFLAGS) $< -o $@

clean:
	$(RM) $(EXECUTABLE1) $(EXECUTABLE2)

.PHONY: default
