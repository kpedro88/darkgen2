#!/bin/bash

CUR_DIR=`pwd`/..
source /cvmfs/sft.cern.ch/lcg/views/LCG_87/x86_64-slc6-gcc49-opt/setup.sh
export PYTHIA8=$CUR_DIR/pythia8226
export PYTHIA8DATA=$PYTHIA8/share/Pythia8/xmldoc/
export LD_LIBRARY_PATH=${PYTHIA8}/lib:${LD_LIBRARY_PATH}
export DELPHES=$CUR_DIR/Delphes-3.4.1
export PATH=$DELPHES:${PATH}
