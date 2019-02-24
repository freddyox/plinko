#include "TFile.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TH1D.h"

void analyze_outputs(){

  TFile *f1 = new TFile("output_plinko_100.root");
  TFile *f2 = new TFile("output_plinko_1000.root");
  TFile *f3 = new TFile("output_plinko_10000.root");
  TFile *f4 = new TFile("output_plinko_100000.root");
  TFile *f5 = new TFile("output_plinko_1000000.root");
  if( !f1 || !f2 || !f3 || !f4 || !f5 ){
    std::cout << "Rootfiles did not load..." << std::endl;
    return;
  }

  TH1D *h1 = (TH1D*)f1->Get("hFinalDis");
  TH1D *h2 = (TH1D*)f2->Get("hFinalDis");
  TH1D *h3 = (TH1D*)f3->Get("hFinalDis");
  TH1D *h4 = (TH1D*)f4->Get("hFinalDis");
  TH1D *h5 = (TH1D*)f5->Get("hFinalDis");

  gStyle->SetOptFit(1111111);
  
  TCanvas *c1 = new TCanvas("c1","", 900,500);
  c1->Divide(2,1);

  int n1 = h1->GetEntries();
  int n3 = h3->GetEntries();
  
  
  c1->cd(1);
  h1->Draw();
  
  // c1->cd(2);
  // h2->Draw();
  
   c1->cd(2);
   h3->Draw();
  
  // c1->cd(4);
  // h4->Draw();
  
  //c1->cd(2);
  //h5->Draw();


  return;
}
