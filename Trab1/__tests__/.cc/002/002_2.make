#:description: Compila dois códigos independentes e cria um diretório para os objetos gerados
#:make: make -f 002_2.make
#:make-clean: make clean -f 002_2.make

# MANUAL https://www.gnu.org/software/make/manual/html_node

RM := rm -rf
OBJECTS_DIR := objects

CCX := g++
CFLAGS  := -c
LDFLAGS := -Wall
LDLIBS  := -lm

SOURCES1 := main1.cc
OBJECTS1 := $(SOURCES1:%.cc=$(OBJECTS_DIR)/%.o)
EXECUTABLE1 := main1.exe

SOURCES2 := main2.cc
OBJECTS2 := $(SOURCES2:%.cc=$(OBJECTS_DIR)/%.o)
EXECUTABLE2 := main2.exe

all: $(EXECUTABLE1) $(EXECUTABLE2)

.SECONDEXPANSION:

$(OBJECTS1) $(OBJECTS2) : $$(patsubst $(OBJECTS_DIR)/%.o, %.cc, $$@)
	mkdir -p $(@D)
	$(CC) -c -o $@ $(CFLAGS) $<


$(EXECUTABLE1): $(OBJECTS1)
	$(CCX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(EXECUTABLE2): $(OBJECTS2)
	$(CCX) $(LDFLAGS) $^ -o $@ $(LDLIBS)


clean:
	$(RM) $(EXECUTABLE1) $(EXECUTABLE2) $(OBJECTS_DIR)

.PHONY: default
