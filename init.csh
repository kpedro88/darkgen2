#!/bin/tcsh

set CUR_DIR=`pwd`/..
source /cvmfs/sft.cern.ch/lcg/views/LCG_87/x86_64-slc6-gcc49-opt/setup.csh
setenv PYTHIA8 $CUR_DIR/pythia8226
setenv PYTHIA8DATA $PYTHIA8/share/Pythia8/xmldoc/
setenv LD_LIBRARY_PATH ${PYTHIA8}/lib:${LD_LIBRARY_PATH}
setenv DELPHES $CUR_DIR/Delphes-3.4.1

