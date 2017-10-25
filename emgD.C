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

    const float deltaz = 0.01;
    int idbg=0;
    float ConeSize=0.4;
    float D0SigCut=3;
    float D0Cut=0.2;
    float LepPtCut = 21;
    float LepEtaCut = 2.4;
    float D0MEDCUT=0.05;
    float IP3DSIGCUT=8;
    float HTCUT = 1000.;
    float JETPTCUT = 30.;
    float JetLepSepCut = 0.4;
    float PT1CUT = 400.;
    float PT2CUT = 200.;
    float PT3CUT = 100.;
    float PT4CUT = 50.;
    float PT5CUT = 50.;
    float PT6CUT = 50.;
    float JETETACUT = 2.;
    float ALPHAMAXCUT = 0.1;
    //float MAXPTCUT = 0.6;
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
    TH1 *fDarkJetAM;
    TH1 *fBJetAM;
    TH1 *fJetAMp;
    TH1 *fJetD0max;
    TH1 *fJetD0ave;
    TH1 *fJetTHave;
    TH1 *fnJet;
    TH1 *fnJet_afterkin;
    TH1 *fFatJetPT;
    TH1 *fFatJetTau21;
    TH1 *fFatJetTau32;
    TH1 *fFatJetMSD;
    TH1 *fFatJetMPR;
    TH1 *fnFatJet;
    TH1 *fnTRK;
    TH1 *ftrkPT;
    TH1 *ftrkTH;
    TH1 *ftrkD0;
    TH1 *ftrkD0Error;
    TH1 *ftrkD0sig;
    TH1 *fMissingET;
    TH1 *fMissingET_afterkin;
    TH1 *felectronPT;
    TH1 *fmuonPT;
    TH1 *fmuonPT_afterkin;
    TH1 *fHT;
    TH1 *fdqd0;
    TH1 *fdd0;

    TH1 *fhtnm1;
    TH1 *fjpt1nm1;
    TH1 *fjpt2nm1;
    TH1 *fjpt3nm1;
    TH1 *fjpt4nm1;
    TH1 *fjpt5nm1;
    TH1 *fjpt6nm1;
    TH1 *famnm1;

    TH1 *fnBJet;
    TH1 *fBJetPT;
    TH1 *fnDarkJet;
    TH1 *fDarkJetPT;

    TH1 *fptTop;
    TH1 *fptTopW;

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

    plots->fnJet_afterkin = result->AddHist1D(
            "nJet_afterkin", "number of jets",
            "number of jets", "number of events",
            50, 0.0, 50.0);

    plots->fJetPT = result->AddHist1D(
            "jet_pt", "jet P_{T}",
            "jet P_{T}, GeV/c", "number of jet",
            50, 0.0, 500.0);

    plots->fJetAM = result->AddHist1D(
            "jet_alphamax", "jet alphamax",
            "alphamax 6 leading jets", "number of jet",
            50, 0.0, 1);

    plots->fDarkJetAM = result->AddHist1D(
            "darkjet_alphamax", "dark jet alphamax",
            "alphamax 6 leading jets", "number of jet",
            50, 0.0, 1);

    plots->fBJetAM = result->AddHist1D(
            "bjet_alphamax", "b jet alphamax",
            "alphamax 6 leading jets", "number of jet",
            50, 0.0, 1);

    plots->fJetAMp = result->AddHist1D(
            "jet_alphamaxp", "jet alphamaxp",
            "alphamaxp 6 leading jets", "number of jet",
            50, 0.0, 1);

    plots->fJetD0max = result->AddHist1D(
            "jet_D0max", "jet d0 max",
            "d0max 6 leading jets", "number of jet",
            50, 0.0, 1.0);

    plots->fJetD0ave = result->AddHist1D(
            "jet_D0ave", "jet d0 ave",
            "d0ave 6 leading jets", "number of jet",
            50, 0.0, 1.0);

    plots->fJetTHave = result->AddHist1D(
            "jet_THave", "jet th ave",
            "theta2d ave 6 leading jets", "number of jet",
            50, 0.0, 0.7);

    plots->fdqd0 = result->AddHist1D(
            "fdqd0", "dq d0",
            "impact parameter for tracks in jets matched to dark quarks", "number of tracks",
            50, -1., 1.);

    plots->fdd0 = result->AddHist1D(
            "fdd0", "d d0",
            "impact parameter for tracks in jets matched to down quarks", "number of tracks",
            50, -1., 1.);

    plots->fnBJet = result->AddHist1D(
            "nBJet", "number of bjets",
            "number of bjets", "number of events",
            50, 0.0, 50.0);

    plots->fBJetPT = result->AddHist1D(
            "bjet_pt", "bjet P_{T}",
            "bjet P_{T}, GeV/c", "number of bjets",
            50, 0.0, 500.0);

    plots->fnDarkJet = result->AddHist1D(
            "nDarkJet", "number of dark quark jets",
            "number of dark quark jets", "number of dark quark jets",
            50, 0.0, 500.0);

    plots->fDarkJetPT = result->AddHist1D(
            "darkjet_pt", "dark jet P_{T}",
            "dark jet P_{T}", "number of dark quark jets",
            50, 0.0, 500.0);

    // book histograms for fat jets
    plots->fnFatJet = result->AddHist1D(
            "nFatJet", "number of fat jets",
            "number of fat jets", "number of events",
            50, 0.0, 50.0);

    plots->fFatJetPT = result->AddHist1D(
            "fatjet_pt", "fat jet P_{T}",
            "fat jet P_{T}, GeV/c", "number of jet",
            100, 0.0, 1000.0);

    plots->fFatJetTau21 = result->AddHist1D(
            "fatjet_tau21", "#tau_{21}",
            "#tau_{21}", "number of jet",
            50, 0.0, 1.0);

    plots->fFatJetTau32 = result->AddHist1D(
            "fatjet_tau32", "#tau_{32}",
            "#tau_{32}", "number of jet",
            50, 0.0, 1.0);

    plots->fFatJetMSD = result->AddHist1D(
            "fatjet_msd", "m_{SD}",
            "m_{SD}, GeV/c^{2}", "number of jet",
            50, 0.0, 500.0);

    plots->fFatJetMPR = result->AddHist1D(
            "fatjet_mpr", "m_{pruned}",
            "m_{pruned}, GeV/c^{2}", "number of jet",
            50, 0.0, 500.0);


    // plots about tops
    plots->fptTop = result->AddHist1D(
            "ptTop", "pt of top quarks",
            "pt of top quarks", "number of events",
            50, 0.0, 1000.0);

    plots->fptTopW = result->AddHist1D(
            "ptTopW", "pt of W from top quarks",
            "pt of W from top quarks", "number of events",
            50, 0.0, 1000.0);

    // book more histograms
    plots->fMissingET = result->AddHist1D(
            "missing_et", "Missing E_{T}",
            "Missing E_{T}, GeV", "number of events",
            100, 0.0, 1000.0);

    plots->fMissingET_afterkin = result->AddHist1D(
            "missing_et_afterkin", "Missing E_{T}",
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

    plots->fmuonPT_afterkin = result->AddHist1D(
            "muonPT_afterkin", "muonPT_afterkin",
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

    plots->fjpt5nm1 = result->AddHist1D(
            "jet5ptnm1", "jet5 P_{T} nm1",
            "jet5 P_{T} n-1, GeV/c", "number of jet",
            50, 0.0, 500.0);

    plots->fjpt6nm1 = result->AddHist1D(
            "jet6ptnm1", "jet6 P_{T} nm1",
            "jet6 P_{T} n-1, GeV/c", "number of jet",
            50, 0.0, 500.0);

    plots->famnm1 = result->AddHist1D(
            "jetalphamaxnm1", "jet alphamax nm1",
            "alphamax n-1 6 leading jets", "number of jet",
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
    plots->fJetTHave->SetStats();

    plots->fBJetPT->SetStats();
    plots->fFatJetPT->SetStats();
    plots->fFatJetTau21->SetStats();
    plots->fFatJetTau32->SetStats();
    plots->fFatJetMSD->SetStats();
    plots->fFatJetMPR->SetStats();
    plots->fnDarkJet->SetStats();
    plots->fDarkJetPT->SetStats();
}

//------------------------------------------------------------------------------

void AnalyseEvents(ExRootTreeReader *treeReader, MyPlots *plots)
{
    TClonesArray *branchParticle = treeReader->UseBranch("Particle");
    TClonesArray *branchTRK = treeReader->UseBranch("Track");
    TClonesArray *branchJet = treeReader->UseBranch("Jet");
    TClonesArray *branchFatJet = treeReader->UseBranch("FatJet");
    TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
    TClonesArray *branchScalarHT = treeReader->UseBranch("ScalarHT");
    TClonesArray *branchElectron = treeReader->UseBranch("Electron");
    TClonesArray *branchMuon = treeReader->UseBranch("Muon");



    Long64_t allEntries = treeReader->GetEntries();

    cout << "** Chain contains " << allEntries << " events" << endl;

    GenParticle *prt;
    GenParticle *prt2;
    GenParticle *prtT;
    Track *trk;
    Jet *jet;
    Jet *fatjet;
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
      { // loop over all entries
        if(idbg>0) myfile<<std::endl;
        if(idbg>0) myfile<<"event "<<entry<<std::endl;
        // Load selected branches with data from specified event
        treeReader->ReadEntry(entry);

        // Analyse gen particles
        int ngn = branchParticle->GetEntriesFast();
        int firstdq = -1;
        int firstadq = -1;
        int firstq = -1;
        int firstaq = -1;
        vector<int> pointtops;
        for(int i=0;i<ngn;i++ ) {
            prt = (GenParticle*) branchParticle->At(i);
            int id=(prt->PID);

            //find the initial daughters of the mediator
            if((id==4900101)&&(firstdq<0)) {
                firstdq=i;
                if(idbg>0) myfile<<" first dark quark"<<std::endl;
                firstq=i-1;
                prt2 = (GenParticle*) branchParticle->At(firstq);
            }
            if((id==-4900101)&&(firstadq<0)) {
                firstadq=i;
                if(idbg>0) myfile<<" first dark antiquark"<<std::endl;
                firstaq=i-1;
                prt2 = (GenParticle*) branchParticle->At(firstaq);
            }
            if(idbg>20) {
                myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt of "<<prt->PT<<" status "<<prt->Status<<" mothers "<<prt->M1<<" "<<prt->M2<<std::endl;
            }

            //to help with background studies, find top and anti top
            if(abs(id)==6) {
                if(idbg>0) {
                    std::cout<<" top at particle "<<i<<std::endl;
                    std::cout<<" daugters are particles "<<prt->D1<<" "<<prt->D2<<std::endl;
                }
                prtT = (GenParticle*) branchParticle->At(prt->D1);
                int idpid1=abs(prtT->PID);
                if(idbg>0) std::cout<<"daughter 1 has pid "<<idpid1<<std::endl;
                prtT = (GenParticle*) branchParticle->At(prt->D2);
                int idpid2=abs(prtT->PID);
                if(idbg>0) std::cout<<"daughter 2 has pid "<<idpid2<<std::endl;

                //find the one that decays to W
                if((idpid1==24)||(idpid2==24) ) {
                    pointtops.push_back(i);
                    if(idbg>0) std::cout<<"choosing this top"<<std::endl;
                }
            }

        }

        if(idbg>0) {
            if((firstdq<0)||(firstadq<0)||(firstq<0)||(firstaq<0)) {
                std::cout<<"danger danger will robinson did not find initial partons"<<std::endl;
            } else {
                prt = (GenParticle*) branchParticle->At(firstdq);
                myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<std::endl;
                prt = (GenParticle*) branchParticle->At(firstq);
                myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<std::endl;
                prt = (GenParticle*) branchParticle->At(firstadq);
                myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<std::endl;
                prt = (GenParticle*) branchParticle->At(firstaq);
                myfile<<"genparticle "<<i<<" has pid "<<prt->PID<<" and pt, eta, phi  of "<<prt->PT<<" "<<prt->Eta<<" "<<prt->Phi<<std::endl;
            }
        }


        //make some plots about the tops in the events
        for(int i=0;i<pointtops.size();i++) {
            prt = (GenParticle*) branchParticle->At(pointtops[i]);
            plots->fptTop->Fill(prt->PT);
            prtT = (GenParticle*) branchParticle->At(prt->D1);
            if(abs(prtT->PID)==24) plots->fptTopW->Fill(prtT->PT);
            prtT = (GenParticle*) branchParticle->At(prt->D2);
            if(abs(prtT->PID)==24) plots->fptTopW->Fill(prtT->PT);
        }

        // find all status 0 particles in initial cone

        if(idbg>2) {
            vector<int> motherpartons;
            if(firstdq>0) motherpartons.push_back(firstdq);
            if(firstadq>0) motherpartons.push_back(firstadq);
            if(firstq>0) motherpartons.push_back(firstq);
            if(firstaq>0) motherpartons.push_back(firstaq);

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
            plots->ftrkD0Error->Fill(fabs(trk->ErrorD0));  // for some reason, delphes pulls this from a gaussian with a mean of zero, so half the time it is neg, which makes no sense to me
            //      std::cout<<"track d0 d0error "<<trk->D0<<" "<<trk->ErrorD0<<std::endl;
            if((trk->ErrorD0)>0) plots->ftrkD0sig->Fill(fabs((trk->D0)/(trk->ErrorD0)));
        }


        // plots for fat jets
        int nfatjet = branchFatJet->GetEntriesFast();
        plots->fnFatJet->Fill(nfatjet);
        for(int i=0;i<nfatjet;i++) {
            jet = (Jet*) branchFatJet->At(i);
            plots->fFatJetPT->Fill(jet->PT);
            plots->fFatJetTau21->Fill(jet->Tau[1]>0 ? jet->Tau[2]/jet->Tau[1] : 0.0);
            plots->fFatJetTau32->Fill(jet->Tau[2]>0 ? jet->Tau[3]/jet->Tau[2] : 0.0);
            plots->fFatJetMSD->Fill(jet->SoftDroppedP4[0].M());
            plots->fFatJetMPR->Fill(jet->PrunedP4[0].M());
        }

        // plots for jets and calculate displaced jet variables
        int njet = branchJet->GetEntriesFast();
        plots->fnJet->Fill(njet);

        vector<float> alphaMax(njet);  // not really alpha max but best we can do here
        vector<float> alphaMaxp(njet);
        vector<float> D0Max(njet);
        vector<float> D0Ave(njet);
        vector<float> THAve(njet);
	vector<float> D0Med(njet);
	vector<float> jet_fpt(njet);
	vector<float> jet_ptmax(njet);
        vector<int> ntrk1(njet);
        vector<bool> goodjet(njet);
        float allpT,cutpT,cutpTp, ptmaxtrk, sumpile;
        int ntrkj;
        vector<bool> adkq(njet);
        vector<bool> adq(njet);
        vector<bool> abq(njet);
        if(idbg>0) myfile<<" number of jets is "<<njet<<std::endl;
        int nbjets = 0;
	int nelectrons = 0;
	int nmuons = 0;
        int ndarkjets = 0;

        for(int i=0;i<njet;i++) {
            jet = (Jet*) branchJet->At(i);
	    vector<float> medianIP;
            bool isBJet = (jet->BTag>>0) & 0x1; 
            // btag working points are accessed by bit-shifting
            // use 0 for loose, 1 for medium, and 2 for tight
            if (isBJet) {
                nbjets++;
                plots->fBJetPT->Fill(jet->PT); }
            abq[i] = isBJet;

            if(idbg>0) myfile<<"jet "<<i<<"  with pt, eta, phi of "<<jet->PT<<" "<<jet->Eta<<" "<<jet->Phi<<std::endl;
            plots->fJetPT->Fill(jet->PT);
            adkq[i]=false;
            adq[i]=false;
            //see if it matches a dark or down quark
            if(firstdq>0) {
                prt2 = (GenParticle*) branchParticle->At(firstdq);
                float dr1=DeltaR(jet->Eta,jet->Phi,prt2->Eta,prt2->Phi);
                if(dr1<0.04) { 
                    adkq[i]=true;
                    plots->fDarkJetPT->Fill(jet->PT);
                }
            }
            if(firstadq>0) {
                prt2 = (GenParticle*) branchParticle->At(firstadq);
                float dr1=DeltaR(jet->Eta,jet->Phi,prt2->Eta,prt2->Phi);
                if(dr1<0.04) { 
                    adkq[i]=true;
                    plots->fDarkJetPT->Fill(jet->PT);
                }
            }
            if(firstq>0) {
                prt2 = (GenParticle*) branchParticle->At(firstq);
                float dr1=DeltaR(jet->Eta,jet->Phi,prt2->Eta,prt2->Phi);
                if(dr1<0.04) adq[i]=true;
            }
            if(firstaq>0) {
                prt2 = (GenParticle*) branchParticle->At(firstaq);
                float dr1=DeltaR(jet->Eta,jet->Phi,prt2->Eta,prt2->Phi);
                if(dr1<0.04) adq[i]=true;
            }

            // calculate track based variables
            alphaMax[i]=1.; // cut on d0sig
            alphaMaxp[i]=1.; // cut on d0
            goodjet[i]=false;
            D0Max[i]=0.;
            D0Ave[i]=0.;
	    D0Med[i] = 0.;
	    jet_fpt[i] = 0.;
	    jet_ptmax[i] = 0.;
            THAve[i]=0.;
            allpT=0.;
            cutpT=0;
            cutpTp=0;
	    ptmaxtrk=0.;
            ntrkj=0;

            for(int j=0;j<ntrk;j++) {
	      trk = (Track*) branchTRK->At(j);
	      dR=DeltaR(jet->Eta,jet->Phi,trk->Eta,trk->Phi);
	      if(dR<ConeSize) { // if track is within 0.4 of jet axis
		if((trk->PT)>1) { // if track pt > 1
		  if(adkq[i]) {
		    plots->fdqd0->Fill(trk->D0); // plot first dark quark track d0
		  }
		  if(adq[i]) {
		    plots->fdd0->Fill(trk->D0); // plot first quark track d0
		  }
		  ntrkj+=1; // count number of tracks with pt >= 1 GeV
		  if((trk->D0)>D0Max[i]) D0Max[i]=(trk->D0);
		  if((trk->PT)>ptmaxtrk) ptmaxtrk = trk->PT;
		  D0Ave[i]=D0Ave[i]+(trk->D0);
		  THAve[i]=THAve[i]+trkTheta[j];
		  medianIP.push_back(fabs(trk->D0));
		  allpT+=(trk->PT); // sum pt of tracks with pt >= 1 GeV
		  if((fabs(trk->ErrorD0))>0) {  // this is not implemented by default.  Hope I did it right!
		    if(fabs((trk->D0)/(trk->ErrorD0))<D0SigCut) { // if track D0Sig < cut
		      cutpT+=trk->PT;
		    } // if track D0Sig < cut
		  } // if |track D0 error| > 0
		  // if(fabs((trk->D0))<D0Cut) { // if track Dxy < cut

		  double dxy_term = (trk->D0)/(trk->ErrorD0);
		  double dz_term = (trk->DZ)/deltaz;
		  double ip3dsig = sqrt((dxy_term)*(dxy_term) + (dz_term)*(dz_term)); // estimate of 3d ip
		  if(ip3dsig<IP3DSIGCUT) { // if track D0 < cut
		    cutpTp+=trk->PT;
		  } // if track D0 < cut
		  if(i<6) { // first 6 jets, used to be 4
		    if(idbg>3) myfile<<"   contains track "<<j<<" with pt, eta, phi of "<<trk->PT<<" "<<trk->Eta<<" "<<trk->Phi<<" d0 of "<<trk->D0<<
				 //" and D0error of "<<trk->ErrorD0<<
				 std::endl;
		    prt = (GenParticle*) trk->Particle.GetObject();
		    if(idbg>3) myfile<<"     which matches to get particle with XY of "<<prt->X<<" "<<prt->Y<<std::endl;
		    
		  }  // end first 6 jets, used to be 4
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
	      D0Ave[i]=D0Ave[i]/ntrkj;
	      THAve[i]=THAve[i]/ntrkj;
            }
	    jet_fpt[i] = ptmaxtrk/jet->PT; // fraction of jet pt coming from max track pt
	    jet_ptmax[i] = ptmaxtrk; // max track pt of this jet
	    std::sort(medianIP.begin(), medianIP.end());
	    std::reverse(medianIP.begin(), medianIP.end());
	    if (medianIP.size() != 0)
	      {
		int thesize = medianIP.size();
		if (thesize % 2 == 0)
		  D0Med[i] = (medianIP[thesize/2]+medianIP[thesize/2 - 1])/2.;
		else
		  D0Med[i] = medianIP[thesize/2];
		  
	      }
            if((fabs(jet->Eta)<JETETACUT)&&(ntrk1[i]>0)) goodjet[i]=true;
	    
            if(idbg>0) myfile<<"alpha max is "<<alphaMax[i]<<std::endl;
        } // end loop over all jets
        plots->fnBJet->Fill(nbjets);
	
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
	    nelectrons++;
        }

	ht = (ScalarHT*) branchScalarHT->At(0);
        // Loop over all muons in event                                                       
        for(i = 0; i < branchMuon->GetEntriesFast(); ++i)
        {
            muon = (Muon*) branchMuon->At(i);
	    plots->fmuonPT->Fill(muon->PT);
	    nmuons++;
        }



        //count number of the 6 (used to be 4) leading jets with alpha max < a cut
        int nalpha=0;
	int nem = 0;
        int iloop=min(6,njet);
        for(int i=0;i<iloop;i++) {
            plots->fJetAM->Fill(alphaMax[i]);
            if (adkq[i]) plots->fDarkJetAM->Fill(alphaMax[i]);
            if (abq[i]) plots->fBJetAM->Fill(alphaMax[i]);
            plots->fJetAMp->Fill(alphaMaxp[i]);
            plots->fJetD0max->Fill(D0Max[i]);
            plots->fJetD0ave->Fill(D0Ave[i]);
            plots->fJetTHave->Fill(THAve[i]);
            if(alphaMaxp[i]<ALPHAMAXCUT) { // if alphamax < cut
	      nalpha+=1;
	      if(idbg>0) myfile<<" jet "<<i<<" passes alphamax cut with alphamax of "<<alphaMax[i]<<std::endl;
	      if(D0Med[i]>D0MEDCUT) { // if d0med < cut
		nem+=1;
	      } // if d0med > cut
            } // if alphamax < cut
        }

        // do pseudo emerging jets analysis

        // see if passes cuts
        bool Pnjet=false;
	bool Pnbjet=false;
	bool Pnlepton=false;
	bool Pleppt=false;
        bool Pht=false;
        bool Ppt1=false;
        bool Ppt2=false;
        bool Ppt3=false;
        bool Ppt4=false;
        bool Ppt5=false;
        bool Ppt6=false;
        bool Pam=false;
	bool PJetLepSep1=false;
	bool PJetLepSep2=false;
	bool PJetLepSep3=false;
	bool PJetLepSep4=false;
	bool PJetLepSep5=false;
	bool PJetLepSep6=false;
        if(njet>=6) Pnjet=true;
	if(nbjets>=1) Pnbjet=true;
	if((nelectrons+nmuons)>=1) Pnlepton=true;
        if(njet>=6) {
            if((ht->HT)>HTCUT) Pht=true;
            jet = (Jet*) branchJet->At(0);
            if(((jet->PT)>PT1CUT)&&goodjet[0]) Ppt1=true;
            jet = (Jet*) branchJet->At(1);
            if(((jet->PT)>PT2CUT)&&goodjet[1]) Ppt2=true;
            jet = (Jet*) branchJet->At(2);
            if(((jet->PT)>PT3CUT)&&goodjet[2]) Ppt3=true;
            jet = (Jet*) branchJet->At(3);
            if(((jet->PT)>PT4CUT)&&goodjet[3]) Ppt4=true;
            jet = (Jet*) branchJet->At(4);
            if(((jet->PT)>PT5CUT)&&goodjet[3]) Ppt5=true;
            jet = (Jet*) branchJet->At(5);
            if(((jet->PT)>PT6CUT)&&goodjet[3]) Ppt6=true;
            //if(nalpha>1) Pam=true;
            if(nem>1) Pam=true;
        }
	float elpt = 0;
	float eleta = 0;
	float elphi = 0;
	float mupt = 0;
	float mueta = 0;
	float muphi = 0;
	if(nelectrons>=1) {
	  electron = (Electron*) branchElectron->At(0);
	  elpt = electron->PT;
	  eleta = electron->Eta;
	}
	if(nmuons>=1) {
	  muon = (Muon*) branchMuon->At(0);
	  mupt = muon->PT;
	  mueta = muon->Eta;
	}
	if ((elpt >= LepPtCut && eleta <= LepEtaCut) || (mupt >= LepPtCut && mueta <= LepEtaCut)) Pleppt = true;
	if(Pleppt && Pnjet){
	  //take the highest pT lepton
	  float lepeta = 0.;
	  float lepphi = 0.;

	  if (elpt > mupt) {
	    lepeta = eleta;
	    lepphi = elphi;
	  }
	  else{
	    lepeta = mueta;
	    lepphi = muphi;
	  }

	  // and check that all jets have a minimum deltaR from that lepton
	  jet = (Jet*) branchJet->At(0);
	  if (DeltaR(jet->Eta,jet->Phi,lepeta,lepphi)>JetLepSepCut) PJetLepSep1 = true;
	  jet = (Jet*) branchJet->At(1);
	  if (DeltaR(jet->Eta,jet->Phi,lepeta,lepphi)>JetLepSepCut) PJetLepSep2 = true;
	  jet = (Jet*) branchJet->At(2);
	  if (DeltaR(jet->Eta,jet->Phi,lepeta,lepphi)>JetLepSepCut) PJetLepSep3 = true;
	  jet = (Jet*) branchJet->At(3);
	  if (DeltaR(jet->Eta,jet->Phi,lepeta,lepphi)>JetLepSepCut) PJetLepSep4 = true;
	  jet = (Jet*) branchJet->At(4);
	  if (DeltaR(jet->Eta,jet->Phi,lepeta,lepphi)>JetLepSepCut) PJetLepSep5 = true;
	  jet = (Jet*) branchJet->At(5);
	  if (DeltaR(jet->Eta,jet->Phi,lepeta,lepphi)>JetLepSepCut) PJetLepSep6 = true;
	}

        //n-1 plots

        for(i = 0; i < branchMuon->GetEntriesFast(); ++i)
        {
            muon = (Muon*) branchMuon->At(i);
	    if (Pnjet&&Pht&&Ppt1&&Ppt2&&Ppt3&&Ppt4&&Ppt5&&Ppt6)
	      plots->fmuonPT_afterkin->Fill(muon->PT);
        }

	if (Pht&&Ppt1&&Ppt2&&Ppt3&&Ppt4&&Ppt5&&Ppt6)
	  {
	    plots->fnJet_afterkin->Fill(njet);
	    if(branchMissingET->GetEntriesFast() > 0)
	      {
		met = (MissingET*) branchMissingET->At(0);
		plots->fMissingET_afterkin->Fill(met->MET);
	      }
	  }

        if(Pnjet&&Ppt1&&Ppt2&&Ppt3&&Ppt4&&Ppt5&&Ppt6/*&&Pam*/) plots->fhtnm1->Fill(ht->HT); // kinematics cuts only!
        jet = (Jet*) branchJet->At(0);
        if(Pnjet&&Pht&&Ppt2&&Ppt3&&Ppt4&&Ppt5&&Ppt6&&Pam) plots->fjpt1nm1->Fill(jet->PT);
        jet = (Jet*) branchJet->At(1);
        if(Pnjet&&Pht&&Ppt1&&Ppt3&&Ppt4&&Ppt5&&Ppt6&&Pam) plots->fjpt2nm1->Fill(jet->PT);
        jet = (Jet*) branchJet->At(2);
        if(Pnjet&&Pht&&Ppt1&&Ppt2&&Ppt4&&Ppt5&&Ppt6&&Pam) plots->fjpt3nm1->Fill(jet->PT);
        jet = (Jet*) branchJet->At(3);
        if(Pnjet&&Pht&&Ppt1&&Ppt2&&Ppt3&&Ppt5&&Ppt6&&Pam) plots->fjpt4nm1->Fill(jet->PT);
        jet = (Jet*) branchJet->At(4);
        if(Pnjet&&Pht&&Ppt1&&Ppt2&&Ppt3&&Ppt4&&Ppt6&&Pam) plots->fjpt5nm1->Fill(jet->PT);
        jet = (Jet*) branchJet->At(5);
        if(Pnjet&&Pht&&Ppt1&&Ppt2&&Ppt3&&Ppt4&&Ppt5&&Pam) plots->fjpt6nm1->Fill(jet->PT);

        if(Pnjet&&Pht&&Ppt1&&Ppt2&&Ppt3&&Ppt4&&Ppt5&&Ppt6) {
            plots->famnm1->Fill(alphaMaxp[0]);
            plots->famnm1->Fill(alphaMaxp[1]);
            plots->famnm1->Fill(alphaMaxp[2]);
            plots->famnm1->Fill(alphaMaxp[3]);
            plots->famnm1->Fill(alphaMaxp[4]);
            plots->famnm1->Fill(alphaMaxp[5]);
        }



        plots->Count->Fill("All",1);
        if(Pnjet) {
            plots->Count->Fill("6 jets",1);
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
				if(Ppt5) {
				  plots->Count->Fill("PT5CUT",1);
				  if(Ppt6) {
				    plots->Count->Fill("PT6CUT",1);
				    if(Pam) {
				      plots->Count->Fill("AM",1);
				      if(idbg>0) myfile<<" event passes all cuts"<<std::endl;
				    } //if Pam
				  }//if Ppt6
				}//if Ppt5
                            } //if Ppt4
			} //if Ppt3
		    } //if Ppt2
		} //if Ppt1
            } //if Pht
        } //if Pnjet
       


      } // loop over all entries
}

//------------------------------------------------------------------------------

void PrintHistograms(ExRootResult *result, MyPlots *plots)
{
    result->Print("png");
}

//------------------------------------------------------------------------------

void emgD(const string inputName)
{
    gSystem->Load("libDelphes");
    string infilename = inputName;
    const string suffix = ".root";
    infilename += suffix;
    const char *inputFile = infilename.c_str();
    cout << "Input file: " << infilename << endl;

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

    string outfilename = "results_" + infilename;
    cout << "Output file: " << outfilename << endl;
    const char* outputFile = outfilename.c_str();
    result->Write(outputFile);

    myfile.close();

    cout << "** Exiting..." << endl;

    delete plots;
    delete result;
    delete treeReader;
    delete chain;
}

//------------------------------------------------------------------------------
