##
##	[Makefile]
##	Created by Micael Levi on 2017-08-31
##	Copyright (c) 2017 mllc@icomp.ufam.edu.br; All rights reserved.
##
##	Macros definidas nos códigos-fonte por padrão:
##	DEBUG (false)
##	TEST (false)
##	FIELD_MAX_SIZE (2048)
##	QTD_BUCKETS (1549147)
##	QTD_BLOCOS_POR_BUCKET (1)
##

RM := rm -rf
OBJECTS_DIR := objects
ARQUIVOS_GERADOS := docs/generatedfiles/*

CXX := gcc -std=c++11
IDIR := ./include
CXXFLAGS := -I$(IDIR)
LDFLAGS := -Wall
LDLIBS  := -lm -lstdc++

SOURCES1 := $(addprefix src/program_upload/, upload.cpp)
OBJECTS1 := $(SOURCES1:%.cpp=$(OBJECTS_DIR)/%.o)
EXECUTABLE1 := upload

SOURCES2 := $(addprefix src/program_findrec/, findrec.cpp)
OBJECTS2 := $(SOURCES2:%.cpp=$(OBJECTS_DIR)/%.o)
EXECUTABLE2 := findrec



##########################################
ifeq ($(DEBUG), true)
	CXXFLAGS += -D DEBUG
endif
ifeq ($(TEST), true)
	CXXFLAGS += -D TEST
endif
ifneq ($(origin FIELD_MAX_SIZE), undefined)
	CXXFLAGS += -D FIELD_MAX_SIZE=$(FIELD_MAX_SIZE)
endif
ifneq ($(origin QTD_BUCKETS), undefined)
	CXXFLAGS += -D QTD_BUCKETS=$(QTD_BUCKETS)
endif
ifneq ($(origin QTD_BLOCOS_POR_BUCKET), undefined)
	CXXFLAGS += -D QTD_BLOCOS_POR_BUCKET=$(QTD_BLOCOS_POR_BUCKET)
endif
##########################################


all: $(EXECUTABLE1) $(EXECUTABLE2)

.SECONDEXPANSION:

$(OBJECTS1) $(OBJECTS2) : $$(patsubst $(OBJECTS_DIR)/%.o, %.cpp, $$@)
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(EXECUTABLE1): $(OBJECTS1)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(EXECUTABLE2): $(OBJECTS2)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)


clean:
	$(RM) $(EXECUTABLE1) $(EXECUTABLE2) $(OBJECTS_DIR) $(ARQUIVOS_GERADOS)

mostlyclean:
	$(RM) $(OBJECTS_DIR)

.PHONY: default
