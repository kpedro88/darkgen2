# honrpythia

source /cvmfs/sft.cern.ch/lcg/views/LCG_87/x86_64-slc6-gcc49-opt/setup.csh
setenv PYTHIA8DATA /data/users/eno/pythia8224
/share/Pythia8/xmldoc/
setenv PYTHIA8 /data/users/eno/pythia8224/
setenv  LD_LIBRARY_PATH ${PYTHIA8}/lib:${LD_LIBRARY_PATH}


make pythiaTree
./pythiaTree
