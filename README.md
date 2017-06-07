# darkgen2

First-time installation instructions (`-m` for MadGraph5 2.5.4, `-p` for Pythia8.226 + LHAPDF6 + HepMC + MG5aMC_PY8_interface, `-d` for Delphes 3.4.1):
```
mkdir scratch
cd scratch
git clone git@github.com:kpedro88/darkgen2
darkgen2/setup.sh -m -p -d
cd darkgen2
source init.(c)sh
make pythiaTree
```

Environment setup instructions (every time):
```
cd scratch/darkgen2
source init.(c)sh
```

To run `pythiaTree` executable (where `[card]` is one of the `.cmnd` text files):
```
./pythiaTree.exe [card]
```

To run Delphes on the output:  
```
DelphesHepMC delphes_card_CMS_imp.tcl haha.root hepmc.out
```

To analyze results from Delphes:
```
root -l 'emgD.C("haha.root")'
```
