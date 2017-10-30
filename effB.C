#include "TSystem.h"

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#endif

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TH1.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TStyle.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

void effB(std::vector<std::string> filenames, int flav=5, int btag=0){
	gStyle->SetOptStat(0);
	gSystem->Load("libDelphes");

	std::vector<TGraphAsymmErrors*> effs(filenames.size(),nullptr);
	Color_t colors[] = {kBlack, kBlue, kMagenta, kRed};
	const int npt = 16;
	Double_t xbins[npt+1] = {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600,800};

	for(unsigned i = 0; i < filenames.size(); ++i){
		TChain *chain = new TChain("Delphes");
		chain->Add(filenames[i].c_str());

		ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
		ExRootResult *result = new ExRootResult();

		TClonesArray *branchJet = treeReader->UseBranch("Jet");

		Long64_t allEntries = treeReader->GetEntries();

		Jet *jet;

		Long64_t entry;

		// Loop over all events

		//histos
		TH1F* h_numer = new TH1F("h_numer","",npt,xbins);
		TH1F* h_denom = new TH1F("h_denom","",npt,xbins);

		int ijloop = allEntries;
		for(entry = 0; entry < ijloop; ++entry){
			treeReader->ReadEntry(entry);
		
			int njet = branchJet->GetEntriesFast();

			for(int j=0;j<njet;j++){
				jet = (Jet*) branchJet->At(j);
				int jflav = jet->Flavor;
				if(jflav!=4 and jflav!=5) jflav = 0;
				if(jflav==flav){
					h_denom->Fill(jet->PT);
					if((jet->BTag>>btag) & 0x1) h_numer->Fill(jet->PT);
				}
			}
		}

		effs[i] = new TGraphAsymmErrors(h_numer,h_denom);
		effs[i]->SetLineColor(colors[i]);
		effs[i]->SetMarkerColor(colors[i]);
	}

	std::stringstream ss;
	ss << "h_eff_flav" << flav << "_btag" << btag;
	TCanvas* can_eff = new TCanvas(ss.str().c_str(),ss.str().c_str());
	can_eff->cd();
	can_eff->SetLogx();
	TH1F* h_axis = new TH1F("h_axis","",npt,xbins);
	h_axis->GetYaxis()->SetRangeUser(0,1);
	h_axis->Draw();
	for(unsigned i = 0; i < effs.size(); ++i){
		effs[i]->Draw("pz same");
	}
	can_eff->Print((ss.str()+".png").c_str(),"png");
}

