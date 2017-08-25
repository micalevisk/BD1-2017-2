#:description: Compila dois códigos independentes e cria um diretório para os objetos gerados. Usando o c++11, -lpthread e inlcude
#:make: make -f 002_3.make
#:make-clean: make clean -f 002_3.make

# MANUAL https://www.gnu.org/software/make/manual/html_node
# IMPLICIT VARIABLES https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

RM := rm -rf
OBJECTS_DIR := objects

CXX := g++ -std=c++11
IDIR := ../include
CXXFLAGS := -I$(IDIR)
LDFLAGS := -lpthread -Wall
LDLIBS  := -lm

SOURCES1 := main1.cc
OBJECTS1 := $(SOURCES1:%.cc=$(OBJECTS_DIR)/%.o)
EXECUTABLE1 := main1.exe

SOURCES2 := main2.cc
OBJECTS2 := $(SOURCES2:%.cc=$(OBJECTS_DIR)/%.o)
EXECUTABLE2 := main2.exe

UNAME_S := $(shell uname -s)

####################
###### Mingw #######
####################
SUB_UNAME_S := $(findstring MINGW, $(UNAME_S))
ifeq ($(SUB_UNAME_S), MINGW)
	CXXFLAGS += -D __MINGW__
endif

#################################
###### Operational System #######
#################################
ifeq ($(OS), Windows_NT)
	# CXXFLAGS += -D __WIN32__
else
	ifeq ($(UNAME_S), Linux)
		# CXXFLAGS += -D __LINUX__
	elif ($(UNAME_S), Darwin)
		# CXXFLAGS += -D __OSX__
	endif
endif



all: $(EXECUTABLE1) $(EXECUTABLE2)

.SECONDEXPANSION:

$(OBJECTS1) $(OBJECTS2) : $$(patsubst $(OBJECTS_DIR)/%.o, %.cc, $$@)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(EXECUTABLE1): $(OBJECTS1)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(EXECUTABLE2): $(OBJECTS2)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)


clean:
	$(RM) $(EXECUTABLE1) $(EXECUTABLE2) $(OBJECTS_DIR)

.PHONY: default
