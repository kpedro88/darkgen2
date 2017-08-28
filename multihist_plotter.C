
#include "tdrstyle.C"
#include "TH1.h"
#include "TH1F.h"

int dolog=1;
void multihist_plotter() 
{ 
    // create vector with signal histogram names
    // add push_back lines to input the names of the histograms
    // you're interested in plotting from the signal samples
    std::vector<char*> sighnames;
    sighnames.push_back("bjet_alphamax");
    sighnames.push_back("darkjet_alphamax");
    // create vector with background histogram names
    // works same as the signal vector, above
    std::vector<char*> bkghnames;
    bkghnames.push_back("jet_alphamax");

    char* atitle = "jet_alphamax";
     
    // lists of colors for the different histograms
    int sig_colors[4] = {2,3,4,5};
    int bkg_colors[4] = {6,7,8,9};
 
    // option to scale to cross section (rather than just 1)
    // along with xs and lumi values to use to do so
    int scaletoxs = 0;
    float ttbarxs = 888000.; // in fb
    float darkxs = 18.45402; // in fb
    float lumi = 100.; // fb^-1

    TFile *f1 = new TFile("results_signal.root");
    TFile *f2 = new TFile("results_ttbar.root");  


    gStyle->SetOptStat(0);

    TString canvName = "Fig_";
    canvName += atitle;


    int W = 800;
    int H = 600;
    TCanvas* canv = new TCanvas(canvName,canvName,50,50,W,H);
    // references for T, B, L, R
    float T = 0.08*H;
    float B = 0.12*H; 
    float L = 0.12*W;
    float R = 0.04*W;

    //canv = new TCanvas(canvName,canvName,50,50,W,H);
    canv->SetFillColor(0);
    canv->SetBorderMode(0);
    canv->SetFrameFillStyle(0);
    canv->SetFrameBorderMode(0);
    //canv->SetLeftMargin( L/W );
    //canv->SetRightMargin( R/W );
    //canv->SetTopMargin( T/H );
    //canv->SetBottomMargin( B/H );
    canv->SetTickx(0);
    canv->SetTicky(0);

    if (dolog) canv->SetLogy();


    TLatex latex;

    int n_ = 2;

    float x1_l = 1.2;
    //  float x1_l = 0.75;
    float y1_l = 0.80;

    float dx_l = 0.60;
    float dy_l = 0.1;
    float x0_l = x1_l-dx_l;
    float y0_l = y1_l-dy_l;

    TLegend *lgd = new TLegend(x0_l,y0_l,x1_l, y1_l); 
    lgd->SetBorderSize(0); lgd->SetTextSize(0.04); lgd->SetTextFont(62); lgd->SetFillColor(0);

    std::vector<double> maxima;


    // get signal hists
    std::cout<<"getting signal hists"<<std::endl;
    std::vector<TH1F*> signal_hists;
    for (int i = 0; i < sighnames.size(); i++) {
        signal_hists.push_back(static_cast<TH1F*>(f1->Get(sighnames[i])->Clone()));
        signal_hists[i]->SetDirectory(0); 
        double integral = signal_hists[i]->Integral();
        std::cout << "Integral is " << integral << std::endl;
        if (scaletoxs) {
            std::cout << "scaling to xs" << std::endl;
            signal_hists[i]->Scale((darkxs*lumi)/integral);
            maxima.push_back(signal_hists[i]->GetMaximum());
            //std::cout << "Maximum is " << signal_hists[i]->GetMaximum() << std::endl;}
        else { 
            signal_hists[i]->Scale(1./integral);
            maxima.push_back(signal_hists[i]->GetMaximum());
            //std::cout << "Maximum is " << signal_hists[i]->GetMaximum() << std::endl;}
    }


    // get bkg hists
    std::cout<<"getting bkg hists"<<std::endl;
    std::vector<TH1F*> bkg_hists;
    for (int i = 0; i < bkghnames.size(); i++) {
        bkg_hists.push_back(static_cast<TH1F*>(f1->Get(bkghnames[i])->Clone()));
        bkg_hists[i]->SetDirectory(0); 
        double integral = bkg_hists[i]->Integral();
        std::cout << "Integral is " << integral << std::endl;
        if (scaletoxs) {
            std::cout << "scaling to xs" << std::endl;
            bkg_hists[i]->Scale((darkxs*lumi)/integral);
            maxima.push_back(bkg_hists[i]->GetMaximum());
            //std::cout << "Maximum is " << bkg_hists[i]->GetMaximum() << std::endl;}
        else { 
            bkg_hists[i]->Scale(1./integral);
            maxima.push_back(bkg_hists[i]->GetMaximum());
            //std::cout << "Maximum is " << bkg_hists[i]->GetMaximum() << std::endl;}
    }


    float max = *std::max_element(maxima.begin(), maxima.end());
    std::cout << "Final maximum is " << max << std::endl;
    for (int i = 0; i < signal_hists.size(); i++) {
        signal_hists[i]->SetMaximum(max*1.3);

        signal_hists[i]->GetYaxis()->SetTitle("percent ");  
        signal_hists[i]->GetYaxis()->SetTitleSize(0.05);  
        signal_hists[i]->GetXaxis()->SetTitle(atitle);  
        signal_hists[i]->GetXaxis()->SetTitleSize(0.05);  

        signal_hists[i]->SetLineColor(sig_colors[i]);
        signal_hists[i]->SetLineWidth(3);
        signal_hists[i]->SetStats(0);
        signal_hists[i]->Draw("same");
    }

    for (int i = 0; i < bkg_hists.size(); i++) {
        bkg_hists[i]->SetLineColor(bkg_colors[i]);
        bkg_hists[i]->SetLineWidth(3);
        bkg_hists[i]->SetStats(0);

        bkg_hists[i]->Draw("same");
    }



    for (int i = 0; i < signal_hists.size(); i++) {
        std::string sig("Signal ");
        std::string title(sig+std::string(signal_hists[i]->GetTitle()));
        lgd->AddEntry(signal_hists[i], title.c_str(), "l");}
    for (int i = 0; i < bkg_hists.size(); i++) {
        std::string bkg("SM ttbar ");
        std::string title(bkg+std::string(bkg_hists[i]->GetTitle()));
        lgd->AddEntry(bkg_hists[i], title.c_str(), "l");}

    lgd->Draw();



    canv->Update();
    canv->RedrawAxis();
    canv->GetFrame()->Draw();
    lgd->Draw();


    if (dolog) {
        canv->Print(canvName+"_log.pdf",".pdf");
        canv->Print(canvName+"_log.png",".png");}
    else{ 
        canv->Print(canvName+".pdf",".pdf");
        canv->Print(canvName+".png",".png");}
    return;
}


