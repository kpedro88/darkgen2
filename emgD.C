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


int idbg=0;
float ConeSize=0.4;
float D0SigCut=3;
float D0Cut=0.2;
float HTCUT = 1000.;
float PT1CUT = 400;
float PT2CUT = 200;
float PT3CUT = 200;
float PT4CUT = 100;
float JETETACUT = 2;
float ALPHAMAXCUT = 0.1;

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
  TH1 *fJetAMp;
  TH1 *fJetD0max;
  TH1 *fJetD0med;
  TH1 *fJetTHmed;
  TH1 *fnJet;
  TH1 *fnTRK;
  TH1 *ftrkPT;
  TH1 *ftrkTH;
  TH1 *ftrkD0;
  TH1 *ftrkD0Error;
  TH1 *ftrkD0sig;
  TH1 *fMissingET;
  TH1 *felectronPT;
  TH1 *fmuonPT;
  TH1 *fHT;

  TH1 *fhtnm1;
  TH1 *fjpt1nm1;
  TH1 *fjpt2nm1;
  TH1 *fjpt3nm1;
  TH1 *fjpt4nm1;
  TH1 *famnm1;
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


  plots->ftrkTH = result->AddHist1D(
    "track_th", "track TH2D",
    "track TH2D", "number of tracks",
    50, 0.0, 0.7);

  plots->ftrkD0 = result->AddHist1D(
    "track_d0", "track D_{0}",
    "track D_{0}, mm", "number of tracks",
    50, -1.0, 1.0);

  plots->ftrkD0Error = result->AddHist1D(
    "track_d0Error", "track D_{0} Error",
    "track D_{0} Error, mm", "number of tracks",
    50, -0.5, 0.5);



  plots->ftrkD0sig = result->AddHist1D(
    "track_d0sig", "track D_{0} sig",
    "track D_{0} sig", "number of tracks",
    50, -5.0, 5.0);


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
    50, 0.0, 1);


  plots->fJetAMp = result->AddHist1D(
    "jet_alphamaxp", "jet alphamaxp",
    "alphamaxp 4 leading jets", "number of jet",
    50, 0.0, 1);


  plots->fJetD0max = result->AddHist1D(
    "jet_D0max", "jet d0 max",
    "d0max 4 leading jets", "number of jet",
    50, 0.0, 1.0);

  plots->fJetD0med = result->AddHist1D(
    "jet_D0med", "jet d0 med",
    "d0med 4 leading jets", "number of jet",
    50, 0.0, 1.0);

  plots->fJetTHmed = result->AddHist1D(
    "jet_THmed", "jet th med",
    "theta2d med 4 leading jets", "number of jet",
    50, 0.0, 0.7);





  // book more histograms


  plots->fMissingET = result->AddHist1D(
    "missing_et", "Missing E_{T}",
    "Missing E_{T}, GeV", "number of events",
    100, 0.0, 1000.0);


  plots->fHT = result->AddHist1D(
    "HT", "HT",
    "HT, GeV", "number of events",
    100, 0.0, 5000.0);


  plots->felectronPT = result->AddHist1D(
    "electronPT", "electronPT",
    "electron PT, GeV", "number of events",
    100, 0.0, 500.0);


  plots->fmuonPT = result->AddHist1D(
    "muonPT", "muonPT",
    "muon PT, GeV", "number of events",
    100, 0.0, 500.0);


  //N-1 histograms
  plots->fhtnm1 = result->AddHist1D(
    "HTnm1", "HTnm1",
    "HT n-1, GeV", "number of events",
    100, 0.0, 5000.0);

  plots->fjpt1nm1 = result->AddHist1D(
    "jet1ptnm1", "jet1 P_{T} nm1",
    "jet1 P_{T} n-1, GeV/c", "number of jet",
    50, 0.0, 500.0);


  plots->fjpt2nm1 = result->AddHist1D(
    "jet2ptnm1", "jet2 P_{T} nm1",
    "jet2 P_{T} n-1, GeV/c", "number of jet",
    50, 0.0, 500.0);


  plots->fjpt3nm1 = result->AddHist1D(
    "jet3ptnm1", "jet3 P_{T} nm1",
    "jet3 P_{T} n-1, GeV/c", "number of jet",
    50, 0.0, 500.0);


  plots->fjpt4nm1 = result->AddHist1D(
    "jet4ptnm1", "jet4 P_{T} nm1",
    "jet4 P_{T} n-1, GeV/c", "number of jet",
    50, 0.0, 500.0);



  plots->famnm1 = result->AddHist1D(
    "jetalphamaxnm1", "jet alphamax nm1",
    "alphamax n-1 4 leading jets", "number of jet",
    50, 0.0, 1);


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
  plots->ftrkTH->SetStats();
  plots->ftrkD0->SetStats();
  plots->ftrkD0Error->SetStats();
  plots->ftrkD0sig->SetStats();
  plots->fJetAM->SetStats();
  plots->fJetAMp->SetStats();
  plots->fJetTHmed->SetStats();
}

//------------------------------------------------------------------------------

void AnalyseEvents(ExRootTreeReader *treeReader, MyPlots *plots)
{
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchTRK = treeReader->UseBranch("Track");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  TClonesArray *branchScalarHT = treeReader->UseBranch("ScalarHT");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");



  Long64_t allEntries = treeReader->GetEntries();

  cout << "** Chain contains " << allEntries << " events" << endl;

  GenParticle *prt;
  GenParticle *prt2;
  Track *trk;
  Jet *jet;
  MissingET *met;
  ScalarHT *ht;
  Electron *electron;
  Muon *muon;

  Long64_t entry;

  Int_t i;
  float dR;

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
	//	if(abs(prt2->PID)!=1) std::cout<<"danger danger did not find d"<<std::endl;
      }
      if((id==-4900101)&&(firstadq<0)) {
	firstadq=i;
	if(idbg>0) myfile<<" first dark antiquark"<<std::endl;
	firstad=i-1;
        prt2 = (GenParticle*) branchParticle->At(firstad);
	//	if(abs(prt2->PID)!=1) std::cout<<"danger danger did not find antid"<<std::endl;
      }
      if(idbg>20) {
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


    // find all status 0 particles in initial cone

    if(idbg>2) {
      vector<int> motherpartons;
      if(firstdq>0) motherpartons.push_back(firstdq);
      if(firstadq>0) motherpartons.push_back(firstadq);
      if(firstd>0) motherpartons.push_back(firstd);
      if(firstad>0) motherpartons.push_back(firstad);

      for(int i=0;i<motherpartons.size();i++ ) {
	myfile<<"finding stable particles in cone for particle "<<i<<std::endl;
        prt2 = (GenParticle*) branchParticle->At(motherpartons[i]);
	float pttotal=0.;
        for(int j=0;j<ngn;j++ ) {
          prt = (GenParticle*) branchParticle->At(j);
	  //	  myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<" and status "<<prt->Status<<std::endl;

	  if(prt->Status==1) {
	    dR=DeltaR(prt->Eta,prt->Phi,prt2->Eta,prt2->Phi);      
	    if(dR<ConeSize) {
              myfile<<"   contains particle "<<j<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<std::endl;
	      pttotal=pttotal+(prt->PT);
	    }
	  }
	}
	myfile<<" total pT in cone is "<<pttotal<<std::endl;
      }
    }


    // Analyse tracks
    int ntrk = branchTRK->GetEntriesFast();
    vector<float> trkTheta(ntrk);
    plots->fnTRK->Fill(ntrk);
    for(int i=0;i<ntrk;i++ ) {
      trk = (Track*) branchTRK->At(i);
      // doing this at the generator level because I am too lazy to figure out the formulas
      // for the reconstructed
      // this would not be the right formula for pileup or if there
      // were a realistic vertex z distribution
      prt = (GenParticle*) trk->Particle.GetObject();
      float x1=prt->X;
      float y1=prt->Y;
      float z1=prt->Z;
      float px1=prt->Px;
      float py1=prt->Py;
      float pz1=prt->Pz;
      trkTheta[i]=0.;
      if((fabs(prt->X)>0.001)||(fabs(prt->Y)>0.001)) {
        float costt = (x1*px1+y1*py1+z1*pz1)/sqrt(x1*x1+y1*y1+z1*z1)/sqrt(px1*px1+py1*py1+pz1*pz1);
        trkTheta[i]=acos(costt);
      }
      plots->ftrkTH->Fill(trkTheta[i]);
      plots->ftrkPT->Fill(trk->PT);
      plots->ftrkD0->Fill(trk->D0);
      plots->ftrkD0Error->Fill(fabs(trk->ErrorD0));  // for some reason, delphse pulls this from a caussian with a mean of zero, so half the time it is neg, which makes no sense to me
      //      std::cout<<"track d0 d0error "<<trk->D0<<" "<<trk->ErrorD0<<std::endl;
      if((trk->ErrorD0)>0) plots->ftrkD0sig->Fill(fabs((trk->D0)/(trk->ErrorD0)));
    }


    // plots for jets and calculate displaced jet variables
    int njet = branchJet->GetEntriesFast();
    plots->fnJet->Fill(njet);

    vector<float> alphaMax(njet);  // not really alpha max but best we can do here
    vector<float> alphaMaxp(njet);
    vector<float> D0Max(njet);
    vector<float> D0Med(njet);
    vector<float> THMed(njet);
    vector<int> ntrk1(njet);
    vector<bool> goodjet(njet);
    float allpT,cutpT,cutpTp;
    int ntrkj;
    if(idbg>0) myfile<<" number of jets is "<<njet<<std::endl;
    for(int i=0;i<njet;i++) {
      jet = (Jet*) branchJet->At(i);
      if(idbg>0) myfile<<"jet "<<i<<"  with pt, eta, phi of "<<jet->PT<<" "<<jet->Eta<<" "<<jet->Phi<<std::endl;
      plots->fJetPT->Fill(jet->PT);
      alphaMax[i]=1.;
      alphaMaxp[i]=1.;
      goodjet[i]=false;
      D0Max[i]=0.;
      D0Med[i]=0.;
      THMed[i]=0.;
      allpT=0.;
      cutpT=0;
      cutpTp=0;
      ntrkj=0;
      
      for(int j=0;j<ntrk;j++) {
        trk = (Track*) branchTRK->At(j);
	dR=DeltaR(jet->Eta,jet->Phi,trk->Eta,trk->Phi);
	if(dR<ConeSize) {
	  if((trk->PT)>1) {
	    ntrkj+=1;
	    if((trk->D0)>D0Max[i]) D0Max[i]=(trk->D0);
	    D0Med[i]=D0Med[i]+(trk->D0);
	    THMed[i]=THMed[i]+trkTheta[j];
	    allpT+=(trk->PT);
	    if((fabs(trk->ErrorD0))>0) {  // this is not implemented by default.  Hope I did it right!
	      if(fabs((trk->D0)/(trk->ErrorD0))<D0SigCut) {
	        cutpT+=trk->PT;
	      }}
	    if(fabs((trk->D0))<D0Cut) {
	      cutpTp+=trk->PT;
	    }
	    if(i<4) {
	      if(idbg>3) myfile<<"   contains track "<<j<<" with pt, eta, phi of "<<trk->PT<<" "<<trk->Eta<<" "<<trk->Phi<<" d0 of "<<trk->D0<<
		       //" and D0error of "<<trk->ErrorD0<<
std::endl;
	      prt = (GenParticle*) trk->Particle.GetObject();
	      if(idbg>3) myfile<<"     which matches to get particle with XY of "<<prt->X<<" "<<prt->Y<<std::endl;

	    }  // end first 4 jets
	  }  //end pT cut of 1 GeV
        } //end in cone

      }  //end loop over tracks
      
      if(allpT>0) {
	alphaMax[i]=cutpT/allpT;
	alphaMaxp[i]=cutpTp/allpT;
      }
      if(alphaMax[i]>0.99999) alphaMax[i]=0.99999;
      if(alphaMaxp[i]>0.99999) alphaMaxp[i]=0.99999;

      ntrk1[i]=ntrkj;
      if(ntrkj>0) {
	D0Med[i]=D0Med[i]/ntrkj;
	THMed[i]=THMed[i]/ntrkj;
      }
      if((fabs(jet->Eta)<JETETACUT)&&(ntrk1[i]>0)) goodjet[i]=true;

      if(idbg>0) myfile<<"alpha max is "<<alphaMax[i]<<std::endl;
    } // end loop over all jets


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


    // Loop over all electrons in event                                                       
    for(i = 0; i < branchElectron->GetEntriesFast(); ++i)
      {
	electron = (Electron*) branchElectron->At(i);
	plots->felectronPT->Fill(electron->PT);
      }
 

   // Loop over all muons in event                                                       
    for(i = 0; i < branchMuon->GetEntriesFast(); ++i)
      {
	muon = (Muon*) branchMuon->At(i);
	plots->fmuonPT->Fill(muon->PT);
      }
 


    //count number of the 4 leading jets with alpha max < a cut
    int nalpha=0;
    int iloop=min(4,njet);
    for(int i=0;i<iloop;i++) {
      plots->fJetAM->Fill(alphaMax[i]);
      plots->fJetAMp->Fill(alphaMaxp[i]);
      plots->fJetD0max->Fill(D0Max[i]);
      plots->fJetD0med->Fill(D0Med[i]);
      plots->fJetTHmed->Fill(THMed[i]);
      if(alphaMax[i]<ALPHAMAXCUT) {
	nalpha+=1;
	if(idbg>0) myfile<<" jet "<<i<<" passes alphamax cut with alphamax of "<<alphaMax[i]<<std::endl;
      }
    }

    // do pseudo emerging jets analysis

    // see if passes cuts
    bool Pnjet=false;
    bool Pht=false;
    bool Ppt1=false;
    bool Ppt2=false;
    bool Ppt3=false;
    bool Ppt4=false;
    bool Pam=false;
    if(njet>3) Pnjet=true;
    if(njet>3) {
    if((ht->HT)>HTCUT) Pht=true;
    jet = (Jet*) branchJet->At(0);
    if(((jet->PT)>PT1CUT)&&goodjet[0]) Ppt1=true;
    jet = (Jet*) branchJet->At(1);
    if(((jet->PT)>PT2CUT)&&goodjet[1]) Ppt2=true;
    jet = (Jet*) branchJet->At(2);
    if(((jet->PT)>PT3CUT)&&goodjet[2]) Ppt3=true;
    jet = (Jet*) branchJet->At(3);
    if(((jet->PT)>PT4CUT)&&goodjet[3]) Ppt4=true;
    if(nalpha>1) Pam=true;
    }


      //n-1 plots

    if(Pnjet&&Ppt1&&Ppt2&&Ppt3&&Ppt4&&Pam) plots->fhtnm1->Fill(ht->HT);
    jet = (Jet*) branchJet->At(0);
    if(Pnjet&&Pht&&Ppt2&&Ppt3&&Ppt4&&Pam) plots->fjpt1nm1->Fill(jet->PT);
    jet = (Jet*) branchJet->At(1);
    if(Pnjet&&Pht&&Ppt1&&Ppt3&&Ppt4&&Pam) plots->fjpt2nm1->Fill(jet->PT);
    jet = (Jet*) branchJet->At(2);
    if(Pnjet&&Pht&&Ppt1&&Ppt2&&Ppt4&&Pam) plots->fjpt3nm1->Fill(jet->PT);
    jet = (Jet*) branchJet->At(3);
    if(Pnjet&&Pht&&Ppt1&&Ppt2&&Ppt3&&Pam) plots->fjpt4nm1->Fill(jet->PT);
    /*
    if(Pnjet&&Pht&&Ppt1&&Ppt2&&Ppt3&&Ppt4) {
      plots->famnm1->Fill(alphaMax[0]);
      plots->famnm1->Fill(alphaMax[1]);
      plots->famnm1->Fill(alphaMax[2]);
      plots->famnm1->Fill(alphaMax[3]);
    }
*/


    plots->Count->Fill("All",1);
    if(Pnjet) {
      plots->Count->Fill("4 jets",1);
      if(Pht) {
        plots->Count->Fill("HT",1);
	if(Ppt1) {
          plots->Count->Fill("PT1CUT",1);
	if(Ppt2) {
          plots->Count->Fill("PT2CUT",1);
	if(Ppt3) {
          plots->Count->Fill("PT3CUT",1);
	if(Ppt4) {
          plots->Count->Fill("PT4CUT",1);
	  if(Pam) {
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
