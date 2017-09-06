
#include "tdrstyle.C"
#include "TH1.h"
#include "TH1F.h"

int dolog=1;
void Overlay() 
{ 
    char* hname ="fatjet_tau32";
    char* atitle = "fat jet tau32";
    int scaletoxs = 0;
    float ttbarxs = 888000.; // in fb
    float darkxs = 18.45402; // in fb
    float lumi = 100.; // fb^-1
    TFile *f1 = new TFile("results_signal.root");
    TFile *f2 = new TFile("results_ttbar.root");  


    gStyle->SetOptStat(0);

    TString canvName = "Fig_";
    canvName += "overlay";


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

    //TH1* h_pt = new TH1F("h_pt"," ",100,0,500);
    //h_pt->GetXaxis()->SetNdivisions(6,5,0);
    //h_pt->GetXaxis()->SetTitle("Dark Pion p_{T} (GeV)");  
    //h_pt->GetXaxis()->SetTitleSize(0.05);  
    //h_pt->GetYaxis()->SetNdivisions(6,5,0);
    //h_pt->GetYaxis()->SetTitleOffset(1);
    //h_pt->GetYaxis()->SetTitle("Events / 5 GeV");  
    //h_pt->GetYaxis()->SetTitleSize(0.05);  

    //int max=  test->GetMaximum() + test->GetMaximum()*0.2; 
    //int max=  2000.;
    //h_pt->SetMaximum(max);
    //cout << max << endl;

    //h_pt->Draw();

    // int histLineColor = kOrange+7;
    //int histFillColor = kOrange-2;
    //float markerSize  = 1.0;

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


    // get signal hist
    std::cout<<"getting first"<<std::endl;
    TH1F *A_pt = static_cast<TH1F*>(f1->Get(hname)->Clone());
    A_pt->SetDirectory(0);
    double aaA = A_pt->Integral();
    std::cout<<" first entries is "<<aaA<<std::endl;
    if (scaletoxs) {
        std::cout << "scaling to xs" << std::endl;
        A_pt->Scale((darkxs*lumi)/aaA);}
    else { A_pt->Scale(1./aaA);}


    // get bkg hist
    std::cout<<"getting second"<<std::endl;
    TH1F *B_pt = static_cast<TH1F*>(f2->Get(hname)->Clone());
    B_pt->SetDirectory(0);
    //  B_pt->Rebin(25);
    double aaB = B_pt->Integral();
    std::cout<<" second entries is "<<aaB<<std::endl;
    if (scaletoxs) {
        std::cout << "scaling to xs" << std::endl;
        B_pt->Scale((ttbarxs*lumi)/aaB);}
    else { B_pt->Scale(1./aaB);}


    float max = std::max(A_pt->GetMaximum(),B_pt->GetMaximum());
    A_pt->SetMaximum(max*1.3);

    A_pt->GetYaxis()->SetTitle("percent ");  
    A_pt->GetYaxis()->SetTitleSize(0.05);  
    A_pt->GetXaxis()->SetTitle(atitle);  
    A_pt->GetXaxis()->SetTitleSize(0.05);  



    A_pt->SetLineColor(3);
    A_pt->SetLineWidth(3);
    A_pt->SetStats(0);
    A_pt->Draw("");



    B_pt->SetLineColor(2);
    B_pt->SetLineWidth(3);
    B_pt->SetStats(0);

    //B_pt->Draw("esame");
    B_pt->Draw("same");
    /*  
        std::cout<<"getting third"<<std::endl;
        TH1F *C_pt = static_cast<TH1F*>(f3->Get("haMgj")->Clone());
        C_pt->SetDirectory(0);
        double aaC = C_pt->Integral();
        std::cout<<" third entries is "<<aaC<<std::endl;
        C_pt->Scale(1/aaC);


        C_pt->SetLineColor(4);
        C_pt->SetLineWidth(3);
        C_pt->SetStats(0);

        C_pt->Draw("same");
        */



    //lgd->AddEntry(A_pt, "Monte Carlo QCD", "l");
    //lgd->AddEntry(B_pt, "Monte Carlo W to mu", "l");
    // lgd->AddEntry(C_pt, "data W to mu", "l");


    lgd->AddEntry(A_pt, "Signal", "l");
    lgd->AddEntry(B_pt, "SM ttbar", "l");
    //lgd->AddEntry(C_pt, "ModelBx500", "l");

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


