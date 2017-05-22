# darkgen2

First-time installation instructions (`-p` for Pythia8.226, `-e` for HepMC, `-d` for Delphes):
```
mkdir scratch
cd scratch
git clone git@github.com:kpedro88/darkgen2
./darkgen2/setup.sh -p -e -d
cd darkgen2
source init.(c)sh
make pythiaTree
```

Environment setup instructions (every time):
```
cd scratch/darkgen2
source init.(c)sh
```

To run `pythiaTree` executable:
```
./pythiaTree
```

To run Delphes on the output:  
```
$DELPHES/DelphesHepMC delphes_card_CMS_imp.tcl haha.root hepmc.out
```
