// test
// for ce change

// main91.cc is a part of the PYTHIA event generator.
// Copyright (C) 2015 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It studies the charged multiplicity distribution at the LHC.
// Modified by Rene Brun, Axel Naumann and Bernhard Meirose
// to use ROOT for histogramming.

// Stdlib header file for input and output.
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>

using namespace std;

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"

// ROOT, for histogramming.
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"

// ROOT, for saving file.
#include "TFile.h"



using namespace Pythia8;


int idsp=0;
int idbg=0;
int ihepMCout=1; 

int nCharged, nNeutral, nTot;

int main(int argc, char* argv[]) {

  // Create the ROOT application environment.
  TApplication theApp("hist", &argc, argv);

  // Create Pythia instance and set it up to generate hard QCD processes
  // above pTHat = 20 GeV for pp collisions at 14 TeV.
  Pythia pythia;
  pythia.readFile("junk.cmnd");
  pythia.init();
  int nEvent = pythia.mode("Main:numberOfEvents");



  // create a file for hepMC output if needed                                   
  //ihepMCout                                                                   
  HepMC::Pythia8ToHepMC ToHepMC;
  HepMC::IO_GenEvent ascii_io("hepmc.out", std::ios::out);

  // Begin event loop. Generate event; skip if generation aborted.



  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {

    if (!pythia.next()) continue;

    if(idbg>0) {
      std::cout<<endl;
      std::cout<<endl;
      std::cout<<endl;
      std::cout<<"***********************************************************"<<endl;
      std::cout<<"Will Robinson New Event "<<iEvent<<std::endl;
    }


    if(ihepMCout>0) {  // write hepMCoutput file                                
      HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
      ToHepMC.fill_next_event( pythia, hepmcevt );

      // Write the HepMC event to file. Done with it.                          \
      //                                                         
      ascii_io << hepmcevt;
      delete hepmcevt;

    }



  }  // end loop over events



  // Statistics on event generation.
  pythia.stat();



  // Done.
  return 0;
}
