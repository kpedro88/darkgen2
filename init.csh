#!/bin/tcsh

set CURDIR=`dirname $PWD`
set LCG=/cvmfs/sft.cern.ch/lcg/views/LCG_87/x86_64-slc6-gcc49-opt
source $LCG/setup.csh
setenv MADGRAPH $CURDIR/MG5_aMC_v2_5_4
setenv PYTHIA8 $MADGRAPH/HEPTools/pythia8
setenv PYTHIA8DATA $PYTHIA8/share/Pythia8/xmldoc/
setenv HEPMC $MADGRAPH/HEPTools/hepmc
setenv DELPHES $CURDIR/Delphes-3.4.1
setenv PATH ${DELPHES}:${MADGRAPH}/bin:${PATH}
setenv LD_LIBRARY_PATH ${PYTHIA8}/lib:${DELPHES}:${LD_LIBRARY_PATH}
setenv ROOT_INCLUDE_PATH ${DELPHES}:${DELPHES}/external:${ROOT_INCLUDE_PATH}
# helpful for debugging
setenv PYTHONHOME $LCG
setenv VALGRIND_LIB $LCG/lib/valgrind
