/*
Simple macro showing how to access branches from the delphes output root file,
loop over events, store histograms in a root file and print them as image files.

root -l examples/Example2.C'("delphes_output.root")'
*/

#include "TH1.h"
#include "TSystem.h"

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#endif

//------------------------------------------------------------------------------

struct MyPlots
{
  TH1 *fJetPT[4];
  TH1 *fnJet;
  TH1 *fnTRK;
  TH1 *ftrkPT;
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
    50, 0.0, 100.0);

  plots->ftrkPT = result->AddHist1D(
    "track_pt", "track P_{T}",
    "track P_{T}, GeV/c", "number of tracks",
    50, 0.0, 50.0);


  // book histograms for jets

  plots->fnJet = result->AddHist1D(
    "nJet", "number of jets",
    "number of jets", "number of events",
    50, 0.0, 100.0);


  plots->fJetPT[0] = result->AddHist1D(
    "jet_pt_0", "leading jet P_{T}",
    "jet P_{T}, GeV/c", "number of jets",
    100, 0.0, 1000.0);

  plots->fJetPT[1] = result->AddHist1D(
    "jet_pt_1", "2nd leading jet P_{T}",
    "jet P_{T}, GeV/c", "number of jets",
    100, 0.0, 1000.0);


  plots->fJetPT[2] = result->AddHist1D(
    "jet_pt_2", "3rd leading jet P_{T}",
    "jet P_{T}, GeV/c", "number of jets",
    100, 0.0, 1000.0);


  plots->fJetPT[3] = result->AddHist1D(
    "jet_pt_3", "4th leading jet P_{T}",
    "jet P_{T}, GeV/c", "number of jets",
    100, 0.0, 1000.0);

  plots->fJetPT[0]->SetLineColor(kRed);
  plots->fJetPT[1]->SetLineColor(kBlue);
  plots->fJetPT[2]->SetLineColor(kGreen);
  plots->fJetPT[3]->SetLineColor(kBlack);

  // book 1 stack of 2 histograms

  stack = result->AddHistStack("jet_pt_all", "1st and 2nd jets P_{T}");
  stack->Add(plots->fJetPT[0]);
  stack->Add(plots->fJetPT[1]);
  stack->Add(plots->fJetPT[2]);
  stack->Add(plots->fJetPT[3]);

  // book legend for stack of 2 histograms

  legend = result->AddLegend(0.72, 0.86, 0.98, 0.98);
  legend->AddEntry(plots->fJetPT[0], "leading jet", "l");
  legend->AddEntry(plots->fJetPT[1], "second jet", "l");
  legend->AddEntry(plots->fJetPT[2], "third jet", "l");
  legend->AddEntry(plots->fJetPT[3], "fourth jet", "l");

  // attach legend to stack (legend will be printed over stack in .eps file)

  result->Attach(stack, legend);

  // book more histograms


  plots->fMissingET = result->AddHist1D(
    "missing_et", "Missing E_{T}",
    "Missing E_{T}, GeV", "number of events",
    100, 0.0, 1000.0);


  plots->fHT = result->AddHist1D(
    "HT", "HT",
    "HT, GeV", "number of events",
    100, 0.0, 5000.0);

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
}

//------------------------------------------------------------------------------

void AnalyseEvents(ExRootTreeReader *treeReader, MyPlots *plots)
{
  TClonesArray *branchTRK = treeReader->UseBranch("Track");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  TClonesArray *branchScalarHT = treeReader->UseBranch("ScalarHT");

  Long64_t allEntries = treeReader->GetEntries();

  cout << "** Chain contains " << allEntries << " events" << endl;

  Track *trk;
  Jet *jet[4];
  MissingET *met;
  ScalarHT *ht;

  Long64_t entry;

  Int_t i;

  // Loop over all events
  for(entry = 0; entry < allEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);



    // plots for jets
    int njet = branchJet->GetEntriesFast();
    plots->fnTRK->Fill(njet);

    if(njet >= 4)  // for events with at least 4 jets
    {
      jet[0] = (Jet*) branchJet->At(0);
      jet[1] = (Jet*) branchJet->At(1);
      jet[2] = (Jet*) branchJet->At(2);
      jet[3] = (Jet*) branchJet->At(3);

      plots->fJetPT[0]->Fill(jet[0]->PT);
      plots->fJetPT[1]->Fill(jet[1]->PT);
      plots->fJetPT[2]->Fill(jet[2]->PT);
      plots->fJetPT[3]->Fill(jet[3]->PT);
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



    // Analyse tracks
    int ntrk = branchTRK->GetEntriesFast();
    plots->fnJet->Fill(ntrk);
    for(int i=0;i<ntrk;i++ ) {
      trk = (Track*) branchTRK->At(i);
      plots->ftrkPT->Fill(trk->PT);
    }



    //find emerging jets


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

  BookHistograms(result, plots);

  AnalyseEvents(treeReader, plots);

  PrintHistograms(result, plots);

  result->Write("results.root");

  cout << "** Exiting..." << endl;

  delete plots;
  delete result;
  delete treeReader;
  delete chain;
}

//------------------------------------------------------------------------------
