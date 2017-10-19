#!/bin/bash
cd /home/friccita/Phenom/darkgen2
source init.sh
./bkg_generation.sh -p ttbar -d $PWD/mg5cards
xrdcp -f ttbar/Events/pilotrun/tag_1_pythia8_events.hepmc.gz /store/user/friccita/Phenom

exit 0