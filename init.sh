#!/bin/bash

CURDIR=$(dirname $PWD)
LCG=/cvmfs/sft.cern.ch/lcg/views/LCG_87/x86_64-slc6-gcc49-opt
source $LCG/setup.sh
export MADGRAPH=$CURDIR/MG5_aMC_v2_5_4
export PYTHIA8=$MADGRAPH/HEPTools/pythia8
export PYTHIA8DATA=$PYTHIA8/share/Pythia8/xmldoc/
export HEPMC=$MADGRAPH/HEPTools/hepmc
export DELPHES=$CURDIR/Delphes-3.4.1
export PATH=${DELPHES}:${MADGRAPH}/bin:${PATH}
export LD_LIBRARY_PATH=${PYTHIA8}/lib:${DELPHES}:${LD_LIBRARY_PATH}
export ROOT_INCLUDE_PATH=${DELPHES}:${DELPHES}/external:${ROOT_INCLUDE_PATH}
# helpful for debugging
export PYTHONHOME=$LCG
export VALGRIND_LIB=$LCG/lib/valgrind

