# emg_delphes

first time, do 
ln -s ../delphes/* .

source /cvmfs/sft.cern.ch/lcg/views/LCG_87/x86_64-slc6-gcc49-opt/setup.csh
root -l
   .X emgD.C("../delphes/haha.root");



(see darkgen2 to see how to make an input file )




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