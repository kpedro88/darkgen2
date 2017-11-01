# darkgen2

## Setup

First-time installation instructions (`-m` for MadGraph5 2.5.4, `-p` for Pythia8.226a\* + LHAPDF6 + HepMC + MG5aMC_PY8_interface, `-d` for Delphes 3.4.1a\*\*):
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

\* patched version of Pythia8.226  
\*\* patched version of Delphes-3.4.1

## Signal generation

Run the `pythiaTree` executable (where `[card]` is one of the `.cmnd` text files, which describes a signal model):
```
./pythiaTree.exe [card]
```

## Background generation

Background generation uses MadGraph + Pythia. An example set of cards can be found in the [mg5cards](./mg5cards) directory.

The options for the script [bkg_generation.sh](./bkg_generation.sh) are as follows:
```
-d              directory of premade cards (default = [your_scratch_dir]/darkgen2/mg5cards)
-p              process name (default = ttbar)
-c              custom card name (default = process name)
-h              display help message and exit
```

The `[name]` given to the `-p` option is the most important parameter. It specifies the MadGraph cards that should be 
used. The [bkg_generation.sh](./bkg_generation.sh) script expects that there is a `[cards]` folder (default [mg5cards](./mg5cards)) in the 
current working directory. In this folder it will look for cards with the following names:

- `[name]_proc_card.dat`
- `[name]_run_card.dat`
- `[name]_param_card.dat`
- `[custom]_customizecards.dat`

The customization card is optional, and can be omitted if the full process you want to generate is specified in 
the other cards. If omitted, the value of `[custom]` will be set to `[name]`. In the customization card, you can
change things like the number of events or the random seed, e.g.:
```
# Contents of an example ttbar_customizecards.dat
set run_card nevents 100
set run_card iseed 12345
```
Alternatively, you can change these values directly in the other cards.

The proc_card defines the process you want to generate. The last line of this card 
specifies the directory name that MadGraph will use to store the diagrams. It is very
important that this name is the same as the `[name]` argument. If not, the script will
not work, and you will get no events. As we will be running on the batch, and there might
be many diagrams, it is a good idea to suppress the production of jpeg files. 
The last line should thus read as follows
```
output [name] -nojpeg
```

The included run_card has reasonable values for SM processes. As an explicit example of how to run the script:
```
echo "set run_card nevents 10" > mg5cards/ttbar_customizecards.dat
./bkg_generation.sh -p ttbar -d $PWD/mg5cards
```

## Detector simulation

Run Delphes on the (unzipped) signal output:
```
DelphesHepMC delphes_card_CMS_imp.tcl signal.root hepmc.out
```

Run Delphes on the (zipped) background output:
```
gunzip -c ttbar/Events/pilotrun/tag_1_pythia8_events.hepmc.gz | DelphesHepMC delphes_card_CMS_imp.tcl ttbar.root
```

## Analysis

To analyze results from Delphes:
```
root -l 'emgD.C("signal.root")'
```

## Plotting

To plot histograms created by the analyzer, use multihist_plotter.C. This takes
five parameters: 
1. ```char* atitle```: The title of the canvas (which will be incorporated into the filename)
2. ```char* sigfile```: The path to the signal output from the analyzer 
3. ```std::vector<char*> sighnames```: A vector of the names of the histograms to be plotted from the signal output 
4. ```char* bkgfile```: The path to the background output from the analyzer
5. ```std::vector<char*> bkghnames```: A vector of the names of the histograms to be plotted from
the background output.

An example command to plot b jet alphamax and dark jet alphamax from signal against
all jet alphamax from background:
```
root -l 'multihist_plotter.C("jet_alphamax","results_signal.root",{"bjet_alphamax","darkjet_alphamax"},"results_ttbar.root",{"jet_alphamax"})'
```
