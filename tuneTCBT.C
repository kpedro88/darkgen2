#include "TSystem.h"

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#endif
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TGraphAsymmErrors.h"
#include "TStyle.h"
#include "TLegend.h"

#include <string>
#include <sstream>
#include <iostream>
#include <set>
#include <iterator>
#include <utility>

template<class T>
TLegend* makeLegend(int nobjs, T* objs, std::string* names, std::string opt){
	TLegend* leg = new TLegend(0.15,0.87-nobjs*0.05,0.35,0.87);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextSize(0.05);
	leg->SetTextFont(42);

	for(int i = 0; i < nobjs; ++i){
		leg->AddEntry(objs[i],names[i].c_str(),opt.c_str());
	}

	return leg;
}

//version=0: old
//version=1: 2D IP
//version=2: 3D IP (todo)
void tuneTCBT(std::string filename, int version=1, float fSigMin=6.5, unsigned fNtracks=3, float fDeltaR=0.3, float fPtMin=1.0, float fIPmax=2.0){
	gSystem->Load("libDelphes");
	gStyle->SetOptStat(0);

	TChain *chain = new TChain("Delphes");
	chain->Add(filename.c_str());

	ExRootTreeReader *treeReader = new ExRootTreeReader(chain);
	ExRootResult *result = new ExRootResult();

	TClonesArray *branchParticle = treeReader->UseBranch("Particle");
	TClonesArray *branchTRK = treeReader->UseBranch("EFlowTrack");
	TClonesArray *branchJet = treeReader->UseBranch("Jet");

	Long64_t allEntries = treeReader->GetEntries();

	std::cout << "** Chain contains " << allEntries << " events" << std::endl;

	Track *trk;
	Jet *jet;
	GenParticle *prt;

	Long64_t entry;

	//declare some histos
	const int nflav = 4;
	int flavors[nflav] = {0, 4, 5, -1};
	std::string flavnames[nflav] = {"udsg","c","b","dark"};
	Color_t colors[nflav] = {kBlack, kRed, kBlue, kMagenta};
	TH1F* h_sip[nflav];
	TH1F* h_pass[nflav];
	TH1F* h_numer[nflav];
	TH1F* h_denom[nflav];
	TGraphAsymmErrors* g_eff[nflav];
	const int npt = 16;
	Double_t xbins[npt+1] = {20,30,40,50,60,70,80,100,120,160,210,260,320,400,500,600,800};
	stringstream sh;
	string sd = "2D";
	if(version==2) sd = "3D";
	sh << "S_{IP}^{" << sd << "}(n_{trk}=" << fNtracks << ")";
	for(int f = 0; f < nflav; ++f){
		h_sip[f] = new TH1F(("h_sip_"+flavnames[f]).c_str(),"",100,-30,30); h_sip[f]->SetLineColor(colors[f]);
		h_sip[f]->GetXaxis()->SetTitle(sh.str().c_str());
		h_pass[f] = new TH1F(("h_pass_"+flavnames[f]).c_str(),"",100,-30,30);
		//eff vs pt
		h_numer[f] = new TH1F(("h_numer_"+flavnames[f]).c_str(),"",npt,xbins);
		h_denom[f] = new TH1F(("h_denom_"+flavnames[f]).c_str(),"",npt,xbins);
	}

	// Loop over all events

	int ijloop = allEntries;
	for(entry = 0; entry < ijloop; ++entry){
		treeReader->ReadEntry(entry);
		
		int njet = branchJet->GetEntriesFast();
		int ntrk = branchTRK->GetEntriesFast();
		int ngen = branchParticle->GetEntriesFast();

		//find dark quarks
		int firstdq = -1, firstadq = -1;
		vector<TLorentzVector> vdrk;
		for(int g=0;g<ngen;g++){
			if(firstdq>0 and firstadq>0) break;

			prt = (GenParticle*) branchParticle->At(g);
			int id = prt->PID;

			if((id==4900101)&&(firstdq<0)){
				firstdq = g;
				vdrk.emplace_back(prt->Px,prt->Py,prt->Pz,prt->E);
			}
			if((id==-4900101)&&(firstadq<0)){
				firstadq = g;
				vdrk.emplace_back(prt->Px,prt->Py,prt->Pz,prt->E);
			}
		}

		for(int j=0;j<njet;j++){
			jet = (Jet*) branchJet->At(j);
			TLorentzVector vjet;
			vjet.SetPtEtaPhiM(jet->PT,jet->Eta,jet->Phi,jet->Mass);
			int flav = jet->Flavor;
			std::set<double,std::greater<double>> sipset;

			//check if dark quark
			if((vdrk.size()>0 and vjet.DeltaR(vdrk[0])<0.04) or (vdrk.size()>1 and vjet.DeltaR(vdrk[1])<0.04)) flav = -1;
			
			//default
			if(flav!=4 and flav!=5 and flav!=-1) flav = 0;

			for(int t=0;t<ntrk;t++ ){
				trk = (Track*) branchTRK->At(t);
				TLorentzVector vtrk;
				vtrk.SetPtEtaPhiM(trk->PT,trk->Eta,trk->Phi,0.0);
				double dr = vjet.DeltaR(vtrk);
				double pt = trk->PT;
				double d0 = 0.0;
				int sign = 0;
				double sip = 0.0;
				if(version==1 or version==2){
					d0 = TMath::Abs(trk->D0);
				}
				else if(version==0){
					d0 = TMath::Hypot(trk->Xd,trk->Yd);
				}
				sign = (vjet.Px()*trk->Xd+vjet.Py()*trk->Yd > 0.0) ? 1 : -1;
				sip = sign*d0/TMath::Abs(trk->ErrorD0);
				if(version==2){
					double dz = TMath::Abs(trk->DZ);
					sign = (vjet.Px()*trk->Xd + vjet.Py()*trk->Yd + vjet.Pz()*trk->Zd > 0.0) ? 1 : -1;
					sip = sign*TMath::Sqrt(TMath::Power(d0/TMath::Abs(trk->ErrorD0),2) + TMath::Power(dz/TMath::Abs(trk->ErrorDZ),2));
				}

				if(pt < fPtMin) {continue;}
				if(dr > fDeltaR) {continue;}
				if(d0 > fIPmax) {continue;}

				sipset.emplace(sip);
			}

			//if less, failed
			double sip = -100.0;
			if(sipset.size()>=fNtracks){
				sip = *std::next(sipset.begin(),fNtracks-1);
			}
			for(int f = 0; f < nflav; ++f){
				if(flav==flavors[f]) {
					h_sip[f]->Fill(sip);
					h_denom[f]->Fill(jet->PT);
					if(sip>fSigMin) {
						h_pass[f]->Fill(sip);
						h_numer[f]->Fill(jet->PT);
					}
					break;
				}
			}
		}
	}

	//print efficiencies
	bool nodark = false;
	for(int f = 0; f < nflav; ++f){
		double denom = h_sip[f]->Integral(0,h_sip[f]->GetNbinsX()+1);
		if(f==nflav-1 and denom==0.0) { nodark = true; continue; }
		std::cout << "eff_" << flavors[f] << " = " << h_pass[f]->Integral(0,h_pass[f]->GetNbinsX()+1)/denom << std::endl;
		g_eff[f] = new TGraphAsymmErrors(h_numer[f],h_denom[f]);
		g_eff[f]->SetLineColor(colors[f]);
		g_eff[f]->SetMarkerColor(colors[f]);
	}
	int nflav_ = nflav;
	if(nodark) nflav_ -= 1; 

	std::stringstream ss;
	ss << "h_sip_v" << version << "_n" << fNtracks << "_sig" << fSigMin << "_" << filename.substr(0,filename.size()-5);
	TCanvas* can_sip = new TCanvas(ss.str().c_str(),ss.str().c_str(), 700, 550);
	can_sip->cd();
	can_sip->SetLogy();
	for(int f = 0; f < nflav_; ++f){
		h_sip[f]->Scale(1.0/h_sip[f]->Integral(0,h_sip[f]->GetNbinsX()+1));
		if(f==0) h_sip[f]->Draw("hist");
		else h_sip[f]->Draw("hist same");
	}
	TLegend* leg_sip = makeLegend(nflav_,h_sip,flavnames,"l");
	leg_sip->Draw("same");
	//cut line
	can_sip->Update();
	double y1 = can_sip->GetLogy() ? pow(10,can_sip->GetUymin()) : can_sip->GetUymin();
	double y2 = can_sip->GetLogy() ? pow(10,can_sip->GetUymax()) : can_sip->GetUymax();
	TLine* cutline = new TLine(fSigMin,y1,fSigMin,y2);
	cutline->SetLineColor(kBlack);
	cutline->SetLineStyle(7);
	cutline->SetLineWidth(2);
	cutline->Draw("same");
	can_sip->Print((ss.str()+".png").c_str(),"png");

	std::stringstream ss2;
	ss2 << "g_eff_v" << version << "_n" << fNtracks << "_sig" << fSigMin << "_" << filename.substr(0,filename.size()-5);
	TCanvas* can_eff = new TCanvas(ss2.str().c_str(),ss2.str().c_str(), 700, 550);
	can_eff->cd();
	can_eff->SetLogx();
	TH1F* h_axis = new TH1F("h_axis","",npt,xbins);
	h_axis->GetYaxis()->SetRangeUser(0,1);
	h_axis->GetXaxis()->SetTitle("jet p_{T} [GeV]");
	h_axis->GetYaxis()->SetTitle("efficiency");
	h_axis->Draw();
	for(int f = 0; f < nflav_; ++f){
		g_eff[f]->Draw("pz same");
	}
	TLegend* leg_eff = makeLegend(nflav_,g_eff,flavnames,"pel");
	leg_eff->Draw("same");
	can_eff->Print((ss2.str()+".png").c_str(),"png");
}

