######################################################################
# Makefile for building Pythia's ROOT examples.
# Axel Naumann, 2011-03-03
######################################################################

PYTHIA8 := /data/users/eno/pythia8224/

# Need this to get SHAREDSUFFIX (e.g. dylib or so)
-include $(PYTHIA8)/config.mk

# A few variables used in this Makefile:
EX           := hist tree pythiaTree
EXE          := $(addsuffix .exe,$(EX))
PYTHIA8      ?= $(PWD)/..
STATICLIB    := $(PYTHIA8)/lib/libpythia8.a $(PYTHIA8)/libHepMC.a
SHAREDLIB    := $(PYTHIA8)/lib/libpythia8.$(SHAREDSUFFIX)
DICTCXXFLAGS := -I$(PYTHIA8)/include -I$(PYTHIA8)
ROOTCXXFLAGS := $(DICTCXXFLAGS) $(shell root-config --cflags)

# Libraries to include if GZIP support is enabled
ifeq (x$(ENABLEGZIP),xyes)
LIBGZIP=-L$(BOOSTLIBLOCATION) -lboost_iostreams -L$(ZLIBLOCATION) -lz
endif

# LDFLAGS1 for static library, LDFLAGS2 for shared library
LDFLAGS1 := $(shell root-config --ldflags --glibs) \
  -L$(PYTHIA8)/lib/ -lpythia8  -L$(PYTHIA8) -lHepMC $(LIBGZIP)
LDFLAGS2 := $(shell root-config --ldflags --glibs) \
  -L$(PYTHIA8)/lib -lpythia8  -L$(PYTHIA8) -lHepMC $(LIBGZIP)

# Default target; make examples (but not shared dictionary)
all: $(EX)

# Rule to build hist example. Needs static PYTHIA 8 library
hist: $(STATICLIB) hist.cc
	$(CXX) $(ROOTCXXFLAGS) $@.cc -o $@.exe $(LDFLAGS1)


# Rule to build hist example. Needs static PYTHIA 8 library
pythiaTree: $(STATICLIB) pythiaTree.cc
	$(CXX) $(ROOTCXXFLAGS) $@.cc -o $@.exe $(LDFLAGS1)

# Rule to build tree example. Needs dictionary to be built and
# static PYTHIA 8 library
tree: $(STATICLIB) tree.cc
	rootcint -f treeDict.cc -c $(DICTCXXFLAGS) pythiaROOT.h pythiaLinkdef.h
	$(CXX) $(ROOTCXXFLAGS) treeDict.cc $@.cc -o $@.exe $(LDFLAGS1)

# Rule to build full dictionary
dict: $(SHAREDLIB)
	rootcint -f pythiaDict.cc -c $(DICTCXXFLAGS) \
           -DPYTHIA8_COMPLETE_ROOT_DICTIONARY \
           pythiaROOT.h pythiaLinkdef.h
	$(CXX) -shared -fPIC -o pythiaDict.$(SHAREDSUFFIX) pythiaDict.cc \
         -DPYTHIA8_COMPLETE_ROOT_DICTIONARY \
         $(ROOTCXXFLAGS) $(LDFLAGS2)


# Error messages if PYTHIA libraries don't exist
$(STATICLIB):
	@echo "Error: PYTHIA 8 archive library must be built"
	@false
$(SHAREDLIB):
	@echo "Error: PYTHIA 8 shared library must be built"
	@false

# Clean up
clean:
	rm -f $(EXE) hist.root pythiaDict.* \
               treeDict.cc treeDict.h pytree.root

