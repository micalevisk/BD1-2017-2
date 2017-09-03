##
##	[Makefile]
##	Created by Micael Levi on 2017-08-31
##	Copyright (c) 2017 mllc@icomp.ufam.edu.br; All rights reserved.
##

RM := rm -rf
OBJECTS_DIR := objects

CXX := gcc -std=c++11
IDIR := ./include
CXXFLAGS := -I$(IDIR)
LDFLAGS := -Wall
LDLIBS  := -lm -lstdc++

SOURCES1 := $(addprefix src/program_upload/, upload.cpp)
OBJECTS1 := $(SOURCES1:%.cpp=$(OBJECTS_DIR)/%.o)
EXECUTABLE1 := upload



##########################################
ifeq ($(DEBUG), true)
	CXXFLAGS += -D DEBUG
endif
ifeq ($(TEST), true)
	CXXFLAGS += -D TEST
endif
##########################################


all: $(EXECUTABLE1)

.SECONDEXPANSION:

$(OBJECTS1) : $$(patsubst $(OBJECTS_DIR)/%.o, %.cpp, $$@)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(EXECUTABLE1) : $(OBJECTS1)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)


clean:
	$(RM) $(EXECUTABLE1) $(OBJECTS_DIR)

mostlyclean:
	$(RM) $(OBJECTS_DIR)

.PHONY: default
