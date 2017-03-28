# darkgen2

source /cvmfs/sft.cern.ch/lcg/views/LCG_87/x86_64-slc6-gcc49-opt/setup.csh
setenv PYTHIA8DATA /data/users/eno/pythia8224/share/Pythia8/xmldoc/
setenv PYTHIA8 /data/users/eno/pythia8224/
setenv  LD_LIBRARY_PATH ${PYTHIA8}/lib:${LD_LIBRARY_PATH}


make pythiaTree
./pythiaTree



( to link pythia8.224 with HepMC, download it from somewhere, do:
go into the new area and then

ln -s /cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/hepmc/2.06.07-cms/lib/libHepMC.a libHepMC.a
mkdir HepMC
cd HepMC  
ln -s /cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/hepmc/2.06.07-cms/include/HepMC/* .
cd ..
./configure --with-hepmc2


to run delphes on the output
download delphes from https://github.com/delphes/delphes
make the exes following the README there
./DelphesHepMC cards/delphes_card_CMS.tcl haha.root ../darkgen2/hepmc.out


 )

