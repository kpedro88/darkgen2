/*
Simple macro showing how to access branches from the delphes output root file,
loop over events, store histograms in a root file and print them as image files.

root -l examples/Example2.C'("delphes_output.root")'
*/

#include "TH1.h"
#include "TSystem.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#endif


int idbg=1;
float ConeSize=0.4;
float D0SigCut=0.2;
float HTCUT = 1000.;
float PT1CUT = 400;
float PT2CUT = 200;
float PT3CUT = 200;
float PT4CUT = 100;
float JETETACUT = 2;
float ALPHAMAXCUT = 0.2;

  std::ofstream myfile;



//------------------------------------------------------------------------------

float DeltaR(float eta1, float phi1, float eta2, float phi2) {
  
  float dR=0.;
  float deta = std::fabs(eta1-eta2);
  float dphi = std::fabs(phi1-phi2);
  if(dphi>3.14159) dphi = 2.*3.14159-dphi;
  dR=std::sqrt(deta*deta+dphi*dphi);
		     
  return dR;
}

struct MyPlots
{
  TH1 *Count;
  TH1 *fJetPT;
  TH1 *fJetAM;
  TH1 *fnJet;
  TH1 *fnTRK;
  TH1 *ftrkPT;
  TH1 *ftrkD0;
  TH1 *fMissingET;
  TH1 *fHT;
};

//------------------------------------------------------------------------------

class ExRootResult;
class ExRootTreeReader;

//------------------------------------------------------------------------------

void BookHistograms(ExRootResult *result, MyPlots *plots)
{
  THStack *stack;
  TLegend *legend;
  TPaveText *comment;


  //book histograms for tracks

  plots->fnTRK = result->AddHist1D(
    "nTRK", "number of tracks",
    "number of tracks", "number of events",
    500, 0.0, 500.0);

  plots->ftrkPT = result->AddHist1D(
    "track_pt", "track P_{T}",
    "track P_{T}, GeV/c", "number of tracks",
    50, 0.0, 50.0);

  plots->ftrkD0 = result->AddHist1D(
    "track_d0", "track D_{0}",
    "track D_{0}, mm", "number of tracks",
    50, -1.0, 1.0);


  // book histograms for jets

  plots->fnJet = result->AddHist1D(
    "nJet", "number of jets",
    "number of jets", "number of events",
    50, 0.0, 50.0);

  plots->fJetPT = result->AddHist1D(
    "jet_pt", "jet P_{T}",
    "jet P_{T}, GeV/c", "number of jet",
    50, 0.0, 500.0);


  plots->fJetAM = result->AddHist1D(
    "jet_alphamax", "jet alphamax",
    "alphamax 4 leading jets", "number of jet",
    50, 0.0, 1.0);


  // book more histograms


  plots->fMissingET = result->AddHist1D(
    "missing_et", "Missing E_{T}",
    "Missing E_{T}, GeV", "number of events",
    100, 0.0, 1000.0);


  plots->fHT = result->AddHist1D(
    "HT", "HT",
    "HT, GeV", "number of events",
    100, 0.0, 5000.0);


  // cut flow
  plots->Count = result->AddHist1D(
      "Count", "Count","cut flow","number of events",3,0,3);
  plots->Count->SetStats(0);
  plots->Count->SetCanExtend(TH1::kAllAxes);


  // book general comment

  /*
  comment = result->AddComment(0.64, 0.86, 0.98, 0.98);
  comment->AddText("demonstration plot");
  comment->AddText("emg");

  // attach comment to single histograms

  result->Attach(plots->fJetPT[0], comment);
  result->Attach(plots->fJetPT[1], comment);
  */

  // show histogram statisics for MissingET
  plots->fMissingET->SetStats();
  plots->fHT->SetStats();
  plots->fJetPT->SetStats();
  plots->ftrkPT->SetStats();
}

//------------------------------------------------------------------------------

void AnalyseEvents(ExRootTreeReader *treeReader, MyPlots *plots)
{
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchTRK = treeReader->UseBranch("Track");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  TClonesArray *branchScalarHT = treeReader->UseBranch("ScalarHT");



  Long64_t allEntries = treeReader->GetEntries();

  cout << "** Chain contains " << allEntries << " events" << endl;

  GenParticle *prt;
  GenParticle *prt2;
  Track *trk;
  Jet *jet;
  MissingET *met;
  ScalarHT *ht;

  Long64_t entry;

  Int_t i;

  // Loop over all events

  int ijloop = allEntries;
  if(idbg>0) ijloop = 10;
  for(entry = 0; entry < ijloop; ++entry)
  {
    if(idbg>0) myfile<<std::endl;
    if(idbg>0) myfile<<"event "<<entry<<std::endl;
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);



    // Analyse gen particles
    int ngn = branchParticle->GetEntriesFast();
    int firstdq = -1;
    int firstadq = -1;
    int firstd = -1;
    int firstad = -1;
    for(int i=0;i<ngn;i++ ) {
      prt = (GenParticle*) branchParticle->At(i);
      int id=(prt->PID);
      if((id==4900101)&&(firstdq<0)) {
	firstdq=i;
	if(idbg>0) myfile<<" first dark quark"<<std::endl;
	firstd=i-1;
        prt2 = (GenParticle*) branchParticle->At(firstd);
	if(abs(prt2->PID)!=1) std::cout<<"danger danger did not find d"<<std::endl;
      }
      if((id==-4900101)&&(firstadq<0)) {
	firstadq=i;
	if(idbg>0) myfile<<" first dark antiquark"<<std::endl;
	firstad=i-1;
        prt2 = (GenParticle*) branchParticle->At(firstad);
	if(abs(prt2->PID)!=1) std::cout<<"danger danger did not find antid"<<std::endl;
      }
      if(idbg>5) {
	  myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt of "<<prt->PT<<" status "<<prt->Status<<" mothers "<<prt->M1<<" "<<prt->M2<<std::endl;
      }
    }


    if(idbg>0) {
      if((firstdq<0)||(firstadq<0)||(firstd<0)||(firstad<0)) {
	  std::cout<<"danger danger will robinson did not find initial partons"<<std::endl;
      } else {
        prt = (GenParticle*) branchParticle->At(firstdq);
        myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<std::endl;
        prt = (GenParticle*) branchParticle->At(firstd);
        myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<std::endl;
        prt = (GenParticle*) branchParticle->At(firstadq);
        myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<std::endl;
        prt = (GenParticle*) branchParticle->At(firstad);
        myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<std::endl;
      }
    }


    // Analyse tracks
    int ntrk = branchTRK->GetEntriesFast();
    plots->fnTRK->Fill(ntrk);
    for(int i=0;i<ntrk;i++ ) {
      trk = (Track*) branchTRK->At(i);
      plots->ftrkPT->Fill(trk->PT);
      plots->ftrkD0->Fill(trk->D0);
    }


    // plots for jets and calculate displaced jet variables
    int njet = branchJet->GetEntriesFast();
    plots->fnJet->Fill(njet);

    vector<float> alphaMax(njet);  // not really alpha max but best we can do here
    vector<float> D0Max(njet);
    float allpT,cutpT,dR;
    if(idbg>0) myfile<<" number of jets is "<<njet<<std::endl;
    for(int i=0;i<njet;i++) {
      jet = (Jet*) branchJet->At(i);
      if(idbg>0) myfile<<"jet "<<i<<"  with pt, eta, phi of "<<jet->PT<<" "<<jet->Eta<<" "<<jet->Phi<<std::endl;
      plots->fJetPT->Fill(jet->PT);
      alphaMax[i]=1.;
      D0Max[i]=0.;
      allpT=0.;
      cutpT=0;
      for(int j=0;j<ntrk;j++) {
        trk = (Track*) branchTRK->At(j);
	dR=DeltaR(jet->Eta,jet->Phi,trk->Eta,trk->Phi);
	if(dR<ConeSize) {
	  if(i<4) {
	    if(idbg>3) myfile<<"   contains track "<<j<<" with pt, eta, phi of "<<trk->PT<<" "<<trk->Eta<<" "<<trk->Phi<<" d0 of "<<trk->D0<<
		       //" and D0error of "<<trk->ErrorD0<<
std::endl;
	    prt = (GenParticle*) trk->Particle.GetObject();
	    if(idbg>3) myfile<<"     which matches to get particle with XY of "<<prt->X<<" "<<prt->Y<<std::endl;
	  if((trk->D0)>D0Max[i]) D0Max[i]=(trk->D0);
	  allpT+=trk->PT;
	  //	  if((trk->ErrorD0)>0) {  // this does not seem to be implemented
	  //	    if(((trk->D0)/(trk->ErrorD0))<D0SigCut) {
	  if(fabs((trk->D0))<D0SigCut) {
	      cutpT+=trk->PT;
	    }
	      //}
	  }}
      }
      if(allpT>0) {
	alphaMax[i]=cutpT/allpT;
      }
      if(idbg>0) myfile<<"alpha max is "<<alphaMax[i]<<std::endl;
    }


    // Analyse missing ET
    if(branchMissingET->GetEntriesFast() > 0)
    {
      met = (MissingET*) branchMissingET->At(0);
      plots->fMissingET->Fill(met->MET);
    }


    // Analyse  HT
    if(branchScalarHT->GetEntriesFast() > 0)
    {
      ht = (ScalarHT*) branchScalarHT->At(0);
      plots->fHT->Fill(ht->HT);
    }


    //count number of the 4 leading jets with alpha max < a cut
    int nalpha=0;
    int iloop=min(4,njet);
    for(int i=0;i<iloop;i++) {
      plots->fJetAM->Fill(alphaMax[i]);
      if(alphaMax[i]<ALPHAMAXCUT) {
	nalpha+=1;
	if(idbg>0) myfile<<" jet "<<i<<" passes alphamax cut with alphamax of "<<alphaMax[i]<<std::endl;
      }
    }

    // do pseudo emerging jets analysis
    plots->Count->Fill("All",1);
    if(njet>3) {
      plots->Count->Fill("4 jets",1);
      if((ht->HT)>HTCUT) {
        plots->Count->Fill("HT",1);
        jet = (Jet*) branchJet->At(0);
	if(((jet->PT)>PT1CUT)&&(fabs(jet->Eta)<JETETACUT)) {
          plots->Count->Fill("PT1CUT",1);
        jet = (Jet*) branchJet->At(1);
	if(((jet->PT)>PT2CUT)&&(fabs(jet->Eta)<JETETACUT)) {
          plots->Count->Fill("PT2CUT",1);
        jet = (Jet*) branchJet->At(2);
	if(((jet->PT)>PT3CUT)&&(fabs(jet->Eta)<JETETACUT)) {
          plots->Count->Fill("PT3CUT",1);
        jet = (Jet*) branchJet->At(3);
	if(((jet->PT)>PT4CUT)&&(fabs(jet->Eta)<JETETACUT)) {
          plots->Count->Fill("PT4CUT",1);
	  if(nalpha>1) {
          plots->Count->Fill("AM",1);
	  if(idbg>0) myfile<<" event passes all cuts"<<std::endl;
	  }
	}}}}
      }
    }
    






      //


  }
}

//------------------------------------------------------------------------------

void PrintHistograms(ExRootResult *result, MyPlots *plots)
{
  result->Print("png");
}

//------------------------------------------------------------------------------

void emgD(const char *inputFile)
{
  gSystem->Load("libDelphes");

  TChain *chain = new TChain("Delphes");
  chain->Add(inputFile);

  ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
  ExRootResult *result = new ExRootResult();

  MyPlots *plots = new MyPlots;

  myfile.open("debug.txt");

  BookHistograms(result, plots);

  AnalyseEvents(treeReader, plots);

  plots->Count->LabelsDeflate();
  plots->Count->LabelsOption("v");


  PrintHistograms(result, plots);

  result->Write("results.root");

  myfile.close();

  cout << "** Exiting..." << endl;

  delete plots;
  delete result;
  delete treeReader;
  delete chain;
}

//------------------------------------------------------------------------------
