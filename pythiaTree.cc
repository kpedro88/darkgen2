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


Int_t idsp=0;
Int_t idbg=0;

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

  // Create file on which histogram(s) can be saved.
  TFile* outFile = new TFile("PythiaOutput.root", "RECREATE");


  // create a file for the event display
  ofstream outPut;
  if(idsp>0) outPut.open("forDisplay.txt");
  if(idsp>0) outPut<<" pid x0 y0 z0 px py pz"<<endl;


unordered_map<int,std::string> pdgName;
unordered_map<int,std::string> darkName;
unordered_map<int,int> pdgNum;


const int npart=26;
const char *partNames[npart] = {
  "pi0",
  "rho0",
  "pi+-",
  "rho+-",
  "eta",
  "omega",
  "K0",
  "K*",
  "K+-",
  "K*+-",
  "etaprime",
  "phi",
  "Delta-",
  "n",
  "Delta0",
  "p",
  "Delta+",
  "Delta++",
  "Sigma-",
  "Sigma*-",
  "Lambda",
  "Sigma0",
  "Sigma*0",
  "Sigma+",
  "Omega*0",
    "unknown"
};



pdgName.emplace(111,"pi0");
pdgName.emplace(113,"rho0");
pdgName.emplace(211,"pi+-");
pdgName.emplace(213,"rho+-");
pdgName.emplace(221,"eta");
pdgName.emplace(223,"omega");
pdgName.emplace(311,"K0");
pdgName.emplace(313,"K*");
pdgName.emplace(321,"K+-");
pdgName.emplace(323,"K*+-");
pdgName.emplace(331,"etaprime");
pdgName.emplace(333,"phi");
pdgName.emplace(1114,"Delta-");
pdgName.emplace(2112,"n");
pdgName.emplace(2114,"Delta0");
pdgName.emplace(2212,"p");
pdgName.emplace(2214,"Delta+");
pdgName.emplace(2224,"Delta++");
pdgName.emplace(3112,"Sigma-");
pdgName.emplace(3114,"Sigma*-");
pdgName.emplace(3122,"Lambda");
pdgName.emplace(3212,"Sigma0");
pdgName.emplace(3214,"Sigma*0");
pdgName.emplace(3222,"Sigma+");
pdgName.emplace(3324,"Omega*0");

pdgNum.emplace(111,0);
pdgNum.emplace(113,1);
pdgNum.emplace(211,2);
pdgNum.emplace(213,3);
pdgNum.emplace(221,4);
pdgNum.emplace(223,5);
pdgNum.emplace(311,6);
pdgNum.emplace(313,7);
pdgNum.emplace(321,8);
pdgNum.emplace(323,9);
pdgNum.emplace(331,10);
pdgNum.emplace(333,11);
pdgNum.emplace(1114,12);
pdgNum.emplace(2112,13);
pdgNum.emplace(2114,14);
pdgNum.emplace(2212,15);
pdgNum.emplace(2214,16);
pdgNum.emplace(2224,17);
pdgNum.emplace(3112,18);
pdgNum.emplace(3114,19);
pdgNum.emplace(3122,20);
pdgNum.emplace(3212,21);
pdgNum.emplace(3214,22);
pdgNum.emplace(3222,23);
pdgNum.emplace(3324,24);

std::unordered_map<int,std::string>::iterator got;
std::unordered_map<int,int>::iterator got2;
for(int hh=0;hh<10000000;hh++) {
  got = pdgName.find(hh);
  if(got == pdgName.end()) pdgName.emplace(hh,"unknown");
  got2 = pdgNum.find(hh);
  if(got2 == pdgNum.end()) pdgNum.emplace(hh,25);
 }


  // Book histogram.
  TH1F *hmultch = new TH1F("hmultch","charged multiplicity", 100, -0.5, 799.5);
  TH1F *hmultneu = new TH1F("hmultneu","neutral multiplicity", 100, -0.5, 799.5);
  TH1F *hppid = new TH1F("hppid","particle identification number", 1000, -500, 500);
  // dark scalar mediator 4900001
  // dark gluon 4900021
  // dark scalar quark 4900101
  // dark scalar pion 4900111
  // dark scalar rho 4900113
  TH1F *hppidHV = new TH1F("hppidHV","particle identification number-490000", 400, -200., 200.);
  TH1F *hppid2HV = new TH1F("hppid2HV","particle identification number-490000 if has stable daughter", 400, -200., 200.);
  TH1F *hppid2ddg = new TH1F("hppid2ddg","particle identification number-490000 dark gluon daughters", 400, -200., 200.);
  TH2F *hmassHV = new TH2F("hmassHV","mass versus id-4900000",300,-150.,150.,1000,-20.,1500.);
  TH2F *hqHV = new TH2F("hqHV","charge versus id-4900000",300,-150.,150.,40,-2.,2.);
  TH1F *hmHV = new TH1F("hmHV","particle mass HV", 5000, 0., 5000.);
  TH1F *hm2HV = new TH1F("hm2HV","particle mass HV", 200, 0., 50.);
  TH1F *hd0HV = new TH1F("hd0HV","r decay HV",200,0.,0.1);
  TH1F *hd0gHV = new TH1F("hd0gHV","decay length over gamma HV stable daughter",200,0.,600);
  TH1F *ht0HV = new TH1F("ht0HV","t decay HV over gamma",200,0.,0.1);
  TH1F *hd0dHV = new TH1F("hd0dHV","r decay HV stable daughter",200,0.,1000.);
  TH2F *hd0d2HV = new TH2F("hd0d2HV","r decay HV stable daughter versus mom id",200,0.,200.,200,0.,1500.);
  TH1F *hstatus = new TH1F("hstatus","particle status HV",200,-100.,100.);
  TH1F *hstatus2 = new TH1F("hstatus2","particle status HV ndau<2",200,-100.,100.);
  TH1F *hndau = new TH1F("hndau","number daughters HV",100,0.,50.);
  TH1F *hnsdau = new TH1F("hnsdau","number stable daughters HV",100,0.,50.);
  TH1F *hd0HVs1 = new TH1F("hd0HVs1","r decay first stable daughter of HV",100,0.,1000.);
  TH2F *hndau2 = new TH2F("hndau2","number of daughters veruss parent ID-4900000",200,0.,200.,50,0.,50.);
  TH1F *hndpis = new TH1F("hndpis","number HV with a  stable daughter in event",100,0.,100.);
  TH1F *hnjet = new TH1F("hnjet"," number jets",50,0.,50.);
  TH1F *hjetpT = new TH1F("hjetpT","jet pT",100,0.,1000.);
  TH1F *hjet1pT = new TH1F("hjet1pT","jet pT",100,0.,1000.);
  TH1F *hjet2pT = new TH1F("hjet2pT","jet pT",100,0.,1000.);
  TH1F *hjet3pT = new TH1F("hjet3pT","jet pT",100,0.,1000.);
  TH1F *hjet4pT = new TH1F("hjet4pT","jet pT",100,0.,1000.);
  TH1F *hjety = new TH1F("hjety","jet y",50,-5.,5.);
  TH1F *hjetphi = new TH1F("hjetphi","jet phi",50,-4.,7.);
  TH1F *hndqs = new TH1F("hndqs","number dark quarks",50,0.,50.);
  TH1F *hndq71 = new TH1F("hndq71","number dark quarks code 71",50,0.,50.);
  TH1F *hndqnm = new TH1F("hndqnm","number dark quarks without dark quark mother",50,0.,50.);
  TH1F *hdRdqj = new TH1F("hdRdqj","delta R between dark quark and matching jet ",100,0.,5.);
  TH1F *hdRdj = new TH1F("hdRdj","delta R between down quark and matching jet ",100,0.,5.);
  TH1F *htright = new TH1F("htright","trigger ht",500,0.,5000.);
  TH1F *hcutflow = new TH1F("hcutflow","cut flow",20,0.,20.);
  TH1F *hdRdpisdjet = new TH1F("hdRdpisdjet","delta R between dark quark and dark pions ",100,0.,5.);
  TH1F *hnjetdpi = new TH1F("hnjetdpi","number of jets containing a dark pi",50,0.,50.);
  TH1F *hndpipj = new TH1F("hndpipj","number of dark pis per jet",50,0.,50.);
  TH1F *hptjetdp = new TH1F("hptjetdp","pt of jets with a dark pi",500,0.,1000.);
  TH1F *hndpipjndq = new TH1F("hndpipjndq","number of dark pis per jet jet not matched dq",50,0.,50.);
  TH1F *hndpipjdq = new TH1F("hndpipjdq","number of dark pis per jet jet matched dq",50,0.,50.);
  TH1F *hndpipjd = new TH1F("hndpipjd","number of dark pis per jet jet matched d",50,0.,50.);
  TH1F *hdppt = new TH1F("hdppt","pt spectra of dark pions",50,0.,50.);
  TH1F *hptjetdpndq = new TH1F("hptjetdpndq","pt of jets with a dark pi not matched dq",500,0.,1000.);
  TH1F *hptjetdpdq = new TH1F("hptjetdpdq","pt of jets with a dark pi matched dq",500,0.,1000.);
  TH2F *hdqvjet = new TH2F("hdqvjet"," pt of dark quark versus matched jet",500,0.,1000.,500,0.,1000.);
  TH1F *hdRdqdq71 = new TH1F("hdRdqdq71","delta R between dark quark and dark quark 71 ",100,0.,5.);
  TH2F *hpTdqdq71 = new TH2F("hpTdqdq71"," pt of dark quark versus dark quark 71",500,0.,1000.,500,0.,1000.);

  TH1F *hdaupt = new TH1F("hdaupt"," pT of stable daughters",100,0.,50.);
  TH1F *hmapt = new TH1F("hmapt"," pT of mother",100,0.,50.);

  TH1F *hdecays = new TH1F("hdecays"," decays ",3,0,3);
  hdecays->SetStats(0);
  hdecays->SetCanExtend(TH1::kAllAxes);
  for(int ij=0;ij<npart;ij++) {
    hdecays->Fill(partNames[ij],1);
  }


  TH1F *hdecays2 = new TH1F("hdecays2"," decays2 ",3,0,3);
  hdecays2->SetStats(0);
  hdecays2->SetCanExtend(TH1::kAllAxes);
  for(int ij=0;ij<npart;ij++) {
    hdecays2->Fill(partNames[ij],1);
  }




  // initialize slowjet
  SlowJet aSlowJet(-1,0.4,35.,2.5,2,1); // power, R, ptjetmin, etamax, which particle, mass
  // initialize trigger slow jet
  SlowJet trigSlowJet(-1,0.4,40.,3.0,2,1); // power, R, ptjetmin, etamax, which particle, mass

  //delete jets outside of acceptance
  


  // Begin event loop. Generate event; skip if generation aborted.


  Int_t ndpis,ndqs,ndq71,ndqnm,m1,m2,ipid,dq1,dq2,d1,d2;
  Int_t dq711,dq712;


  Float_t dq1pT,dq1y,dq1phi;
  Float_t dq2pT,dq2y,dq2phi;
  Float_t d1pT,d1y,d1phi;
  Float_t d2pT,d2y,d2phi;
  Float_t dq1dR,dq2dR,d1dR,d2dR,aaatmp,aaatmp2;
  Int_t dq1sj,dq2sj,d1sj,d2sj;

  Int_t ndpismax=100;
  Int_t ptdpis[100];

  cout<<"test test"<<endl;

  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    if(idsp>0) outPut<<"New Event "<<iEvent<<endl;

    if (!pythia.next()) continue;

    if(idbg>0) {
      std::cout<<endl;
      std::cout<<endl;
      std::cout<<endl;
      std::cout<<"***********************************************************"<<endl;
      std::cout<<"Will Robinson New Event "<<iEvent<<std::endl;
    }

    // do jet finding for this event
    aSlowJet.analyze(pythia.event );
    trigSlowJet.analyze(pythia.event );

    // Find number of all final charged particles.
    nCharged = 0;  // for counting the number of stable charged particles in the event
    nNeutral = 0;  // ditto neutral
    nTot=0;
    ndpis=0;  // number of dark pions with at least one stable daughter
    ndqs=0;  // number of dark quarks 
    ndq71=0; // number of dark quarks with 71 code
    ndqnm=0; // number of dark quarks with no dark quark mother
    dq1=0;
    dq2=0;
    d1=0;
    d2=0;
    dq711=0;
    dq712=0;


    for (int i = 0; i < pythia.event.size(); ++i) {  // loop over all particles in the event
      //      std::cout<<pythia.event[i].isCharged()<<endl;

      //  look at dark quarks
      if(abs(pythia.event[i].id())==4900101) {
	ndqs++;  // count number of dark quarks in event
	// look for dark quarks that do not have a dark quark daughter
        m1=pythia.event[i].mother1();
	m2=pythia.event[i].mother2();
	if( (abs(pythia.event[m1].id())!=4900101) && (abs(pythia.event[m2].id())!=4900101) ) {
	  ndqnm++;
	  if(dq1==0) {
	    dq1=i;
	    if(abs(pythia.event[pythia.event[m1].daughter1()].id())==1) {
	      d1=pythia.event[m1].daughter1();
	    } else{
	      d1=pythia.event[m1].daughter2();
	    }
	  } else {
	    dq2=i;
	    if(abs(pythia.event[pythia.event[m1].daughter1()].id())==1) {
	      d2=pythia.event[m1].daughter1();
	    } else{
	      d2=pythia.event[m1].daughter2();
	    }
	  }
	}
	// look for dark quarks with code 71
	if(abs(pythia.event[i].status())==71) {
	  ndq71++;
	  if(idbg>0) cout<<" dark quark with code 71 is "<<i<<" "<<pythia.event[i].pT()<<" "<<pythia.event[i].y()<<" "<<pythia.event[i].phi()<<endl;
	  if(dq711==0) {
	    dq711=i;
	  } else {
	    dq712=i;
	  }
	}
      }
      // look at all HV particles and make list of dark pions with stable daughters and put in ptdpise
      if(abs(pythia.event[i].id())>4900000) {
	int idHV = pythia.event[i].id();
	Float_t mHV = pythia.event[i].m();
	Float_t qHV = pythia.event[i].charge();
        Float_t d0HV = sqrt(pow(pythia.event[i].xProd(),2)+pow(pythia.event[i].yProd(),2));
	Float_t decayLHV = sqrt(pow(pythia.event[i].xProd(),2)+pow(pythia.event[i].yProd(),2)+pow(pythia.event[i].zProd(),2));
	Float_t massHV = sqrt( pow(pythia.event[i].e(),2) - pow(pythia.event[i].pAbs(),2));
	Float_t gammaHV = pythia.event[i].e()/ massHV;
	Float_t betaHV = pythia.event[i].pAbs()/pythia.event[i].e();
	Float_t decaypT = decayLHV/betaHV/3e10/gammaHV;
	Int_t ndauHV=0; 
	if(pythia.event[i].daughter1()!=0) ndauHV=pythia.event[i].daughter2()-pythia.event[i].daughter1()+1;
	if(idbg>8) std::cout<<" for particle "<<i<<" number of daughters is "<<ndauHV<<std::endl;
	Int_t HV = (idHV/abs(idHV))*(abs(idHV)-4900000);
	hppidHV->Fill( HV);  // get the type of the particle
        hmassHV->Fill( HV,mHV );
        hqHV->Fill( HV,qHV );
	hmHV->Fill(mHV);
	hm2HV->Fill(mHV);
	hd0HV->Fill(d0HV);

	ht0HV->Fill(decaypT);
	hstatus->Fill(pythia.event[i].status());
	if(ndauHV<2) hstatus2->Fill(pythia.event[i].status());
        hndau->Fill(ndauHV);
        hndau2->Fill(abs(HV),ndauHV);
	//what are the dark gluon daughters?
	if(HV==21) {
	  if(pythia.event[i].daughter1()!=0) hppid2ddg->Fill(pythia.event[pythia.event[i].daughter1()].id()-4900000);
	  if(pythia.event[i].daughter2()!=0) hppid2ddg->Fill(pythia.event[pythia.event[i].daughter2()].id()-4900000);
	}
	if(ndauHV>0) { // if it is not a stable HV particle
	  if(idbg>8) std::cout<<"entering studies of unstable HVs"<<std::endl;

	  //          if( abs(idHV)==4900113) {  // dark rho
	  //	    cout<<"danger danger will robinson dark rho number daughters is "<<ndauHV<<endl;
	  //  	    for(int ij=0; ij<ndauHV; ++ij) {
	  //	      Int_t iii = pythia.event[i].daughter1()+ij;
	  //	      cout<<"daughter "<<ij<<" has id "<<pythia.event[iii].id()<<endl;
	  //	      cout<<"mother momentum is "<<pythia.event[i].px()<<","<<pythia.event[i].py()<<","<<pythia.event[i].pz()<<endl;
	  //	      cout<<"daught momentum is "<<pythia.event[iii].px()<<","<<pythia.event[iii].py()<<","<<pythia.event[iii].pz()<<endl;
	  //	    }
	  //          }

	  Int_t nstable=0;
	  Int_t nHVdau=0;
	  for(int ij=0; ij<ndauHV; ++ij) {  // loop over all the HV particle's daughters
	    Int_t iii = pythia.event[i].daughter1()+ij;
	    Int_t idauid = abs(pythia.event[iii].id());
	    if(idauid>4900000) nHVdau++;
	  

	    if(pythia.event[iii].isFinal()) {  // for stable daughters of HV particles
	      Float_t d0dHV = sqrt(pow(pythia.event[iii].xProd(),2)+pow(pythia.event[iii].yProd(),2));
	      Float_t L0DHV = sqrt(pow(pythia.event[iii].xProd(),2)+pow(pythia.event[iii].yProd(),2)+pow(pythia.event[iii].zProd(),2) );

	      if(nstable==0) { // if his a particle that is stable (first one)
		hnsdau->Fill(pythia.event[i].daughter2()-pythia.event[i].daughter1());
		hd0HVs1->Fill(d0dHV);
		ndpis++;  // count HV particles that have at least one stable daughter
		nstable++;
		if(ndpis<ndpismax) ptdpis[ndpis-1]=i;
 	        hppid2HV->Fill(HV);
	        hd0dHV->Fill(d0dHV);
		if(abs(pythia.event[i].id())==4900111) { // dark pion
		  hd0gHV->Fill(L0DHV/betaHV/gammaHV);
		  hdppt->Fill(pythia.event[i].pT());
		}
	        hd0d2HV->Fill(abs(HV),d0dHV);
		if(idbg>0) {
		  std::cout<<" energy momentum mass beta gamma decayL are "<<pythia.event[i].e()<<" "<<pythia.event[i].pAbs()<<" "<<
		    massHV<<" "<<
		    betaHV<<" "<<gammaHV<<" "<<L0DHV<<endl;
		}

	      }

	      //	      if(idHV==4900021) {
	      //		std::cout<<" danger will r: dark gluon with stable child "<<pythia.event[pythia.event[i].daughter1()+ij].id()<<std::endl;
	      //		std::cout<<" daughters are "<<pythia.event[iii].daughter1()<<" "<<pythia.event[iii].daughter2()<<std::endl;
	      //	      }
	    }	// end if final
	  }  // end loop over HV daughters
	  // for dark pions that have at least one stable daughter, make a pretty plot
	  if(nstable>0&& abs(pythia.event[i].id())==4900111) {
	  for(int ij=0; ij<ndauHV; ++ij) {  // loop over all the HV particle's daughters
	    Int_t iii = pythia.event[i].daughter1()+ij;
            hdecays->Fill(partNames[pdgNum[pythia.event[iii].id()]],1);
	  }  // end loop over HV daughters
	  }  //end if dark pion with stable daughters
	  // find all the daughters 
	  vector<int> ptalldau;
	  if(nHVdau==0) {  // if none of the daughters are another HV particle
	    if(idbg>1) cout<<" making decay tree for particle "<<i<<" with number of daughters "<<ndauHV<<endl;
	    hmapt->Fill(pythia.event[i].pT());
	    for(int ij=0; ij<ndauHV; ++ij) {  // loop over all the HV particle's daughters
	      Int_t iii = pythia.event[i].daughter1()+ij;
	      if(idbg>1) std::cout<<"     adding particle "<<iii<<endl;
	      ptalldau.push_back(iii);
	    }  // end loop over HV daughters
	    bool stop=false;
	    int isize,sizeold;
            sizeold = 0;
	    while(!stop) {
	      isize = ptalldau.size();
	      if(idbg>1) std::cout<<"    current size is "<<isize<<std::endl;
	    
	      stop=true;
	      if(idbg>1) std::cout<<" size old isize is "<<sizeold<<" "<<isize<<std::endl;
	      for(int hh=sizeold;hh<isize;hh++) {
	        if(pythia.event[ptalldau[hh]].daughter1()!=0) {
		  stop=false;
	          int  idat = pythia.event[ptalldau[hh]].daughter2()-pythia.event[ptalldau[hh]].daughter1()+1;	      
	          for(int jj=0;jj<idat;jj++) {
		    ptalldau.push_back(pythia.event[ptalldau[hh]].daughter1()+jj);
		    if(idbg>1) std::cout<<" adding particle "<<pythia.event[ptalldau[hh]].daughter1()+jj<<std::endl;
	          }
		}
	      }
	      sizeold=isize;
	      if(idbg>1) std::cout<<" now sizeold is "<<sizeold<<std::endl;
	      if(sizeold>500) {
		std::cout<<" danger danger will robinson too many particles"<<std::endl;
		stop=true;
	      }
	      if(idbg>1) std::cout<<"  stop "<<stop<<std::endl;
	    }
	    if(idbg>1) std::cout<<"now making stable daughters"<<std::endl;
	    vector<int> ptstdau;
	    isize = ptalldau.size();
	    for(int hh=0;hh<isize;hh++) {
	      // make a list of the stable one
	      if(pythia.event[ptalldau[hh]].daughter1()==0) {
		ptstdau.push_back(ptalldau[hh]);
	        if(idbg>1) std::cout<<" adding stable particle "<<ptalldau[hh]<<std::endl;
	      }
	    }
	    isize = ptstdau.size();
	    for(int hh=0;hh<isize;hh++) {
              hdecays2->Fill(partNames[pdgNum[pythia.event[ptstdau[hh]].id()]],1);
	      hdaupt->Fill(pythia.event[ptstdau[hh]].pT());
	    }



	  }

	}  // end if HV partiles with daughters
      }  // end if an HV

      // look at stable, charged particles
      if (pythia.event[i].isFinal() && pythia.event[i].isCharged()!=0) {  // count if stable and charged and output to display file
        ++nCharged;
	
	if(idsp>0) outPut<<pythia.event[i].id()<<" "<<
		     pythia.event[i].xProd()<<" "<<
		     pythia.event[i].yProd()<<" "<<
		     pythia.event[i].zProd()<<" "<<
		     pythia.event[i].px()<<" "<<
		     pythia.event[i].py()<<" "<<
		     pythia.event[i].pz()<<" "<<
	  endl;
	
      }
      //look at stable and neutral
      if (pythia.event[i].isFinal() && pythia.event[i].isCharged()==0) // count if stable and neutral
        ++nNeutral;

      //look at all stable particles
      if(pythia.event[i].isFinal()) {  // if stable
	hppid->Fill( pythia.event[i].id() );  // get the type of the particle
	nTot=nTot+1;  //count
	//	cout<<"   id px py pz e "<<pythia.event[i].id()<<" "<<pythia.event[i].px()<<" "<<pythia.event[i].py()<<" "<<pythia.event[i].pz()<<" "<<pythia.event[i].e()<<std::endl;
      }


    }  // end particle list loop 
    // Fill charged multiplicity in histogram. End event loop.
    hmultch->Fill( nCharged );
    hmultneu->Fill( nNeutral );
    hndpis->Fill( ndpis );
    hndqs->Fill( ndqs );
    hndq71->Fill( ndq71 );
    hndqnm->Fill( ndqnm );



    if(idbg>0) {
      cout<<"will robinson"<<endl;
      cout<<"number dark quarks without dark quark mothers is "<<ndqnm<<endl;
      cout<<" pointers to dark quarks are "<<dq1<<" "<<dq2<<endl;
      cout<<" pt y phi are "<<pythia.event[dq1].pT()<<" "<<pythia.event[dq1].y()<<" "<<pythia.event[dq1].phi()<<endl;
      cout<<" pt y phi are "<<pythia.event[dq2].pT()<<" "<<pythia.event[dq2].y()<<" "<<pythia.event[dq2].phi()<<endl;
      cout<<" pointers to d quarks are "<<d1<<" "<<d2<<endl;
      cout<<" pt y phi are "<<pythia.event[d1].pT()<<" "<<pythia.event[d1].y()<<" "<<pythia.event[d1].phi()<<endl;
      cout<<" pt y phi are "<<pythia.event[d2].pT()<<" "<<pythia.event[d2].y()<<" "<<pythia.event[d2].phi()<<endl;
      cout<<endl;
      cout<<" number dark quarks code 71 is "<<ndq71<<endl;
    }
    if(idbg>0) {
      cout<<endl;
      cout<<" information about dark pions"<<endl;
      cout<<" number of dark pions is "<<ndpis<<endl;
      cout<<" id mother1 mother2 daughter 1 daughter2 pt y phi"<<endl;
      for(int jj=0;jj<ndpis;++jj) {
	Int_t kk = ptdpis[jj];
	 cout<<kk<<" "<<pythia.event[kk].id()<<" "<<pythia.event[kk].mother1()<<" "<<pythia.event[kk].mother2()<<" "<<
	  pythia.event[kk].daughter1()<<" "<<pythia.event[kk].daughter2()<<" "<<
        pythia.event[kk].pT()<<" "<<pythia.event[kk].y()<<" "<<pythia.event[kk].phi()<<" "<<endl;

      }
    }


    // compare code 71 dark quarks to initial dark quarks
    Float_t a1=abs(pythia.event[dq1].phi()-pythia.event[dq711].phi());
    if(a1>3.14159) a1=6.2832-a1;
    a1=sqrt(pow(pythia.event[dq1].y()-pythia.event[dq711].y(),2)+pow(a1,2));
    Float_t b1=abs(pythia.event[dq1].phi()-pythia.event[dq712].phi());
    if(b1>3.14159) b1=6.2832-b1;
    b1=sqrt(pow(pythia.event[dq1].y()-pythia.event[dq712].y(),2)+pow(b1,2));
    if(a1<b1) {
      hdRdqdq71->Fill(a1);
      hpTdqdq71->Fill(pythia.event[dq1].pT(),pythia.event[dq711].pT());
    } else {
      hdRdqdq71->Fill(b1);
      hpTdqdq71->Fill(pythia.event[dq1].pT(),pythia.event[dq712].pT());
    }


    a1=abs(pythia.event[dq2].phi()-pythia.event[dq711].phi());
    if(a1>3.14159) a1=6.2832-a1;
    a1=sqrt(pow(pythia.event[dq2].y()-pythia.event[dq711].y(),2)+pow(a1,2));
    b1=abs(pythia.event[dq2].phi()-pythia.event[dq712].phi());
    if(b1>3.14159) b1=6.2832-b1;
    b1=sqrt(pow(pythia.event[dq2].y()-pythia.event[dq712].y(),2)+pow(b1,2));
    if(a1<b1) {
      hdRdqdq71->Fill(a1);
      hpTdqdq71->Fill(pythia.event[dq2].pT(),pythia.event[dq711].pT());
    } else {
      hdRdqdq71->Fill(b1);
      hpTdqdq71->Fill(pythia.event[dq2].pT(),pythia.event[dq712].pT());
    }


    // change to code 71 dark quarks
    //    dq1=dq711;
    //    dq2=dq712;


    //get kinematic variables for initial dark quarks and d quarks

    dq1pT=pythia.event[dq1].pT();
    dq1y=pythia.event[dq1].y();
    dq1phi=pythia.event[dq1].phi();
    dq2pT=pythia.event[dq2].pT();
    dq2y=pythia.event[dq2].y();
    dq2phi=pythia.event[dq2].phi();
    d1pT=pythia.event[d1].pT();
    d1y=pythia.event[d1].y();
    d1phi=pythia.event[d1].phi();
    d2pT=pythia.event[d2].pT();
    d2y=pythia.event[d2].y();
    d2phi=pythia.event[d2].phi();

    // delta r between dark quark and d quark
    //    Float_t bbb=99999.;
    // aaatmp=abs(dq1phi-aSlowJet.phi(ijet));
    //  if(aaatmp>3.14159) aaatmp=6.2832-aaatmp;
    //  aaatmp=sqrt(pow(dq1y-aSlowJet.y(ijet),2)+pow(aaatmp,2));
    //  if(aaatmp<dq1dR) {
    //	dq1dR=aaatmp;
    //	dq1sj=ijet;
    // }
    
      


    // variables to do matching to slow jets
    dq1dR=99999.;
    dq2dR=99999.;
    d1dR=99999.;
    d2dR=99999.;


    // analyze jets
    hnjet->Fill( aSlowJet.sizeJet() );
    if(idbg>0) aSlowJet.list();
    if(aSlowJet.sizeJet()>0)  hjet1pT->Fill(aSlowJet.pT(0));
    if(aSlowJet.sizeJet()>1)  hjet2pT->Fill(aSlowJet.pT(1));
    if(aSlowJet.sizeJet()>2)  hjet3pT->Fill(aSlowJet.pT(2));
    if(aSlowJet.sizeJet()>3)  hjet4pT->Fill(aSlowJet.pT(3));

    // set up counters for number of dark pions in each jet and number of jets with at least one dark pion
    vector<int> ndqinjet(aSlowJet.sizeJet());
    for(int ii=0; ii<aSlowJet.sizeJet(); ii++) { ndqinjet[ii]=0;}

    for (int ijet =0; ijet< aSlowJet.sizeJet(); ++ijet) {
      hjetpT->Fill(aSlowJet.pT(ijet));
      hjety->Fill(aSlowJet.y(ijet));
      hjetphi->Fill(aSlowJet.phi(ijet));

      // find number of dark quarks in jet
      for(int ll=0;ll<ndpis;++ll) {
        aaatmp=abs(pythia.event[ptdpis[ll]].phi()-aSlowJet.phi(ijet));
        if(aaatmp>3.14159) aaatmp=6.2832-aaatmp;
        aaatmp=sqrt(pow(pythia.event[ptdpis[ll]].y()-aSlowJet.y(ijet),2)+pow(aaatmp,2));
	if(aaatmp<0.4) {
	  ndqinjet[ijet]=ndqinjet[ijet]+1;
	}
      }



      // find delta R to dq1
      aaatmp=abs(dq1phi-aSlowJet.phi(ijet));
      if(aaatmp>3.14159) aaatmp=6.2832-aaatmp;
      aaatmp=sqrt(pow(dq1y-aSlowJet.y(ijet),2)+pow(aaatmp,2));
      if(aaatmp<dq1dR) {
	dq1dR=aaatmp;
	dq1sj=ijet;
      }

      // find delta R to dq2
      aaatmp=abs(dq2phi-aSlowJet.phi(ijet));
      if(aaatmp>3.14159) aaatmp=6.2832-aaatmp;
      aaatmp=sqrt(pow(dq2y-aSlowJet.y(ijet),2)+pow(aaatmp,2));
      if(aaatmp<dq2dR) {
	dq2dR=aaatmp;
	dq2sj=ijet;
      }
      // find delta R to d1
      aaatmp=abs(d1phi-aSlowJet.phi(ijet));
      //      cout<<d1phi<<" "<<aSlowJet.phi(ijet)<<" "<<aaatmp<<endl;
      if(aaatmp>3.14159) aaatmp=6.2832-aaatmp;
      //      cout<<d1y<<" "<<aSlowJet.y(ijet)<<endl;
      aaatmp=sqrt(pow(d1y-aSlowJet.y(ijet),2)+pow(aaatmp,2));
      //      cout<<" ijet "<<ijet<<" aaatmp "<<aaatmp<<" d1dR "<<d1dR<<endl;
      if(aaatmp<d1dR) {
	d1dR=aaatmp;
	d1sj=ijet;
      }
      // find delta R to d2
      aaatmp=abs(d2phi-aSlowJet.phi(ijet));
      if(aaatmp>3.14159) aaatmp=6.2832-aaatmp;
      aaatmp=sqrt(pow(d2y-aSlowJet.y(ijet),2)+pow(aaatmp,2));
      if(aaatmp<d2dR) {
	d2dR=aaatmp;
	d2sj=ijet;
      }

    }  // end loop over slow jets


    if(idbg>0) {
      cout<<" slow jet matching to dq1 is "<<dq1sj<<endl;
      cout<<" slow jet matching to dq2 is "<<dq2sj<<endl;
      cout<<" slow jet matching to d1 is "<<d1sj<<endl;
      cout<<" slow jet matching to d2 is "<<d2sj<<endl;
    }

      hdRdqj->Fill(dq1dR);
      hdRdqj->Fill(dq2dR);

      hdRdj->Fill(d1dR);
      hdRdj->Fill(d2dR);

      hdqvjet->Fill(pythia.event[dq1].pT(),aSlowJet.pT(dq1sj));
      hdqvjet->Fill(pythia.event[dq2].pT(),aSlowJet.pT(dq2sj));
      Float_t Del1 = (pythia.event[dq1].pT()-aSlowJet.pT(dq1sj))/aSlowJet.pT(dq1sj);
      Float_t Del2 = (pythia.event[dq2].pT()-aSlowJet.pT(dq2sj))/aSlowJet.pT(dq2sj);
      //      if( (Del1>0.5&&aSlowJet.pT(dq1sj)<60) || (Del2>0.5&&aSlowJet.pT(dq2sj)<60) ) {
      //	cout<<"danger danger will robinson Del1 Del2 are "<<Del1<<" "<<Del2<<endl;
      //      }
    
      





    // another loop over slow jets to make plots about dark quarks matched to them
    Int_t njetdpi=0;
    if(idbg>0) cout<<" information about dark pions per jet"<<endl;
    for (int ijet =0; ijet< aSlowJet.sizeJet(); ++ijet) {
	if( (ijet==d1sj) || (ijet==d2sj) ) {
	  hndpipjd->Fill(ndqinjet[ijet]);
	}
      if(ndqinjet[ijet]>0) {
	njetdpi=njetdpi+1;
	hndpipj->Fill(ndqinjet[ijet]);
	hptjetdp->Fill(aSlowJet.pT(ijet));
	if( (ijet!=dq1sj)&& (ijet!=dq2sj) ) {
	  hndpipjndq->Fill(ndqinjet[ijet]);
  	  hptjetdpndq->Fill(aSlowJet.pT(ijet));
	} else {
	  hndpipjdq->Fill(ndqinjet[ijet]);
  	  hptjetdpdq->Fill(aSlowJet.pT(ijet));
	}
      }
      if(idbg>0) {
	cout<<ijet<<" "<<ndqinjet[ijet]<<endl;
      }
    }
    hnjetdpi->Fill(njetdpi);


    // find delta R between dark pions and dark quarts
    for (int ii =0; ii< ndpis; ++ii) {
      Int_t ipt = ptdpis[ii];
      // find delta R to dq1
      aaatmp=abs(dq1phi-pythia.event[ipt].phi());
      if(aaatmp>3.14159) aaatmp=6.2832-aaatmp;
      aaatmp=sqrt(pow(dq1y-pythia.event[ipt].y(),2)+pow(aaatmp,2));
      // find delta R to dq2
      aaatmp2=abs(dq2phi-pythia.event[ipt].phi());
      if(aaatmp2>3.14159) aaatmp2=6.2832-aaatmp2;
      aaatmp2=sqrt(pow(dq2y-pythia.event[ipt].y(),2)+pow(aaatmp2,2));
      //take minimum
      if(aaatmp2<aaatmp) aaatmp=aaatmp2;
      //      if(aaatmp>2) cout<<"danger danger will robinson aaatmp large"<<endl;
      hdRdpisdjet->Fill(aaatmp);
    }


    // for each dark quark, output daughter tree until hit stable particle
    // dark quark 1
    //    cout<<endl;
    if(idbg>0) cout<<"beginning dark quark 1 "<<dq1<<endl;
    Int_t ipt = dq1;
    //get number of daughters
    vector<int> dpts1;
    vector<bool> indq1(pythia.event.size());
    indq1[ipt]=true;
    Int_t nd = pythia.event[ipt].daughter2()-pythia.event[ipt].daughter1()+1;
    for(int kk=0;kk<nd;++kk){
      indq1[pythia.event[ipt].daughter1()+kk]=true;
      dpts1.push_back(pythia.event[ipt].daughter1()+kk);
    }

      
    while(dpts1.size()>0) {
      ipt=dpts1[0];
      dpts1.erase(dpts1.begin());
      nd = pythia.event[ipt].daughter2()-pythia.event[ipt].daughter1()+1;
      for(int kk=0;kk<nd;++kk){
	if( (pythia.event[pythia.event[ipt].daughter1()+kk].isFinal()==false)&&
	    (abs(pythia.event[pythia.event[ipt].daughter1()+kk].id())>4900000)
	   ) {
          dpts1.push_back(pythia.event[ipt].daughter1()+kk);
	  indq1[pythia.event[ipt].daughter1()+kk]=true;
	}
      }
    }
    if(idbg>0) cout<<" id mothers daughters pt y phi deltaR"<<endl;
    for(int kk=0;kk<pythia.event.size();++kk) {
      if(indq1[kk]) {
	aaatmp=abs(dq1phi-pythia.event[kk].phi());
        if(aaatmp>3.14159) aaatmp=6.2832-aaatmp;
        aaatmp=sqrt(pow(dq1y-pythia.event[kk].y(),2)+pow(aaatmp,2));

        if(idbg>0) cout<<kk<<" "<<pythia.event[kk].id()<<" "<<pythia.event[kk].mother1()<<" "<<pythia.event[kk].mother2()<<" "<<
	  pythia.event[kk].daughter1()<<" "<<pythia.event[kk].daughter2()<<" "<<
        pythia.event[kk].pT()<<" "<<pythia.event[kk].y()<<" "<<pythia.event[kk].phi()<<" "<<aaatmp<<endl;
      }
    }
    
    // dark quark 2
    //    cout<<endl;
    if(idbg>0) cout<<"beginning dark quark 2 "<<dq2<<endl;
    ipt = dq2;
    //get number of daughters
    vector<int> dpts2;
    vector<bool> indq2(pythia.event.size());
    indq2[ipt]=true;
    nd = pythia.event[ipt].daughter2()-pythia.event[ipt].daughter1()+1;
    for(int kk=0;kk<nd;++kk){
      indq2[pythia.event[ipt].daughter1()+kk]=true;
      dpts2.push_back(pythia.event[ipt].daughter1()+kk);
    }

      
    while(dpts2.size()>0) {
      ipt=dpts2[0];
      dpts2.erase(dpts2.begin());
      nd = pythia.event[ipt].daughter2()-pythia.event[ipt].daughter1()+1;
      for(int kk=0;kk<nd;++kk){
	if( (pythia.event[pythia.event[ipt].daughter1()+kk].isFinal()==false)&&
	    (abs(pythia.event[pythia.event[ipt].daughter1()+kk].id())>4900000)
	   ) {
          dpts2.push_back(pythia.event[ipt].daughter1()+kk);
	  indq2[pythia.event[ipt].daughter1()+kk]=true;
	}
      }
    }
    if(idbg>0) cout<<" id mothers daughters pt y phi deltaR"<<endl;
    for(int kk=0;kk<pythia.event.size();++kk) {
      if(indq2[kk]) {
	aaatmp=abs(dq2phi-pythia.event[kk].phi());
        if(aaatmp>3.14159) aaatmp=6.2832-aaatmp;
        aaatmp=sqrt(pow(dq2y-pythia.event[kk].y(),2)+pow(aaatmp,2));

        if(idbg>0) cout<<kk<<" "<<pythia.event[kk].id()<<" "<<pythia.event[kk].mother1()<<" "<<pythia.event[kk].mother2()<<" "<<
	  pythia.event[kk].daughter1()<<" "<<pythia.event[kk].daughter2()<<" "<<
        pythia.event[kk].pT()<<" "<<pythia.event[kk].y()<<" "<<pythia.event[kk].phi()<<" "<<aaatmp<<endl;
      }
    }
    





    //calculate trigger HT
    Float_t trigHT=0.;
    for (int ijet =0; ijet< trigSlowJet.sizeJet(); ++ijet) {
      trigHT=trigHT+trigSlowJet.pT(ijet);
    }
    htright->Fill(trigHT);

    // event selection
    Int_t icut =0;
    bool pass = true;

    hcutflow->Fill(icut+0.5); icut++;// all events

    if(trigHT>800) hcutflow->Fill(icut+0.5); icut++;  //trigger

    if(aSlowJet.sizeJet()>0) {
      if(aSlowJet.pT(0)>400) {
	if(pass) hcutflow->Fill(icut+0.5); icut++;
      }  else {
	pass=false;
      }
    }
    if(aSlowJet.sizeJet()>1) {
      if(aSlowJet.pT(1)>200) {
	if(pass) hcutflow->Fill(icut+0.5); icut++;
      }  else {
	pass=false;
      }
    }
    if(aSlowJet.sizeJet()>2) {
      if(aSlowJet.pT(2)>125) {
	if(pass) hcutflow->Fill(icut+0.5); icut++;
      }  else {
	pass=false;
      }
    }
    if(aSlowJet.sizeJet()>3) {
      if(aSlowJet.pT(3)>50) {
	if(pass) hcutflow->Fill(icut+0.5);icut++;
      }  else {
	pass=false;
      }
    }




  }  // end loop over events


  // close file for display
  if(idsp>0)  outPut.close();

  // Statistics on event generation.
  pythia.stat();


  // Save histogram on file and close file.
  hdppt->Write();
  hdRdqdq71->Write();
  hpTdqdq71->Write();
  hdqvjet->Write();
  hnjetdpi->Write();
  hndpipj->Write();
  hndpipjndq->Write();
  hndpipjdq->Write();
  hndpipjd->Write();
  hptjetdp->Write();
  hptjetdpndq->Write();
  hptjetdpdq->Write();
  hdRdpisdjet->Write();
  htright->Write();
  hcutflow->Write();
  hdRdqj->Write();
  hdRdj->Write();
  hppid2ddg->Write();
  hndpis->Write();
  hndqs->Write();
  hndq71->Write();
  hndqnm->Write();
  hmultch->Write();
  hmultneu->Write();
  hppid->Write();
  hppidHV->Write();
  hppid2HV->Write();
  hmassHV->Write();
  hqHV->Write();
  hmHV->Write();
  hm2HV->Write();
  hd0HV->Write();
  hd0gHV->Write();
  ht0HV->Write();
  hd0dHV->Write();
  hd0d2HV->Write();
  hstatus->Write();
  hstatus2->Write();
  hndau->Write();
  hnsdau->Write();
  hd0HVs1->Write();
  hndau2->Write();
  hnjet->Write();
  hjetpT->Write();
  hjet1pT->Write();
  hjet2pT->Write();
  hjet3pT->Write();
  hjet4pT->Write();
  hjety->Write();
  hjetphi->Write();

  hdecays->LabelsDeflate();
  hdecays->LabelsOption("v");
  hdecays->LabelsOption("a");
  hdecays->Write();


  hdecays2->LabelsDeflate();
  hdecays2->LabelsOption("v");
  hdecays2->LabelsOption("a");
  hdecays2->Write();

  hdaupt->Write();
  hmapt->Write();

  delete outFile;

  // Done.
  return 0;
}
