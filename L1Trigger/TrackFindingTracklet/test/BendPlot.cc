#include "TMath.h"
#include "TRint.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphPainter.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TGaxis.h"
#include "TFile.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <string>
#include <stdlib.h>


void mySmallText(Double_t x,Double_t y,Color_t color,const char *text) {
  Double_t tsize=0.044;
  TLatex l;
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}


using namespace std;

void BendPlot(){
//
// To see the output of this macro, click here.



//


gROOT->Reset();

gROOT->SetStyle("Plain");

gStyle->SetCanvasColor(kWhite);

gStyle->SetCanvasBorderMode(0);     // turn off canvas borders
gStyle->SetPadBorderMode(0);
gStyle->SetOptStat(1);
gStyle->SetOptTitle(0);

  // For publishing:
  gStyle->SetLineWidth(1);
  gStyle->SetTextSize(1.1);
  gStyle->SetLabelSize(0.06,"xy");
  gStyle->SetTitleSize(0.06,"xy");
  gStyle->SetTitleOffset(1.0,"x");
  gStyle->SetTitleOffset(1.6,"y");
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadRightMargin(0.06);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.18);

  // define histograms

  int nbins = 29;
  double xm = 7.25;

  TH1 *hist1 = new TH1F("h1","",nbins,-xm,xm);
  TH1 *histL1 = new TH1F("hL1","",nbins,-xm,xm);
  TH1 *histL2 = new TH1F("hL2","",nbins,-xm,xm);
  TH1 *histL3 = new TH1F("hL3","",nbins,-xm,xm);
  TH1 *histL4 = new TH1F("hL4","",nbins,-xm,xm);
  TH1 *histL5 = new TH1F("hL5","",nbins,-xm,xm);
  TH1 *histL6 = new TH1F("hL6","",nbins,-xm,xm);

  TH1 *histD1 = new TH1F("hD1","",nbins,-xm,xm);
  TH1 *histD2 = new TH1F("hD2","",nbins,-xm,xm);
  TH1 *histD3 = new TH1F("hD3","",nbins,-xm,xm);
  TH1 *histD4 = new TH1F("hD4","",nbins,-xm,xm);

  TH1 *histdL1 = new TH1F("hdL1","",26,-6.5,6.5);
  TH1 *histdL2 = new TH1F("hdL2","",26,-6.5,6.5);
  TH1 *histdL3 = new TH1F("hdL3","",42,-10.5,10.5);
  TH1 *histdL4 = new TH1F("hdL4","",42,-10.5,10.5);
  TH1 *histdL5 = new TH1F("hdL5","",50,-12.5,12.5);
  TH1 *histdL6 = new TH1F("hdL6","",50,-12.5,12.5);

  TH1 *histdL1c = new TH1F("Bend Diff L1","Bend Diff L1",32,-8,8);
  TH1 *histdL2c = new TH1F("Bend Diff L2","Bend Diff L2",32,-8,8);
  TH1 *histdL3c = new TH1F("Bend Diff L3","Bend Diff L3",32,-8,8);
  TH1 *histdL4c = new TH1F("Bend Diff L4","Bend Diff L4",32,-8,8);
  TH1 *histdL5c = new TH1F("Bend Diff L5","Bend Diff L5",32,-8,8);
  TH1 *histdL6c = new TH1F("Bend Diff L6","Bend Diff L6",32,-8,8);

  TH1 *histdD1c = new TH1F("Bend Diff D1","Bend Diff D1",32,-8,8);
  TH1 *histdD2c = new TH1F("Bend Diff D2","Bend Diff D2",32,-8,8);
  TH1 *histdD3c = new TH1F("Bend Diff D3","Bend Diff D3",32,-8,8);
  TH1 *histdD4c = new TH1F("Bend Diff D4","Bend Diff D4",32,-8,8);

  /*
  TH1 *histDD = new TH1F("hDD","",300,-1.5,1.5);
  TH1 *histD1 = new TH1F("hD1","",300,-1.5,1.5);
  TH1 *histD3 = new TH1F("hD3","",300,-1.5,1.5);
  TH1 *histDL1 = new TH1F("hDL1","",300,-1.5,1.5);
  TH1 *histDL2 = new TH1F("hDL2","",300,-1.5,1.5);
  TH1 *histc = new TH1F("hc","",300,-15,15);
  */

 ifstream in2("trackletenginebend2_singmup_PU0_2000.txt");

 int count=0;
 int countp=0;
 int countm=0;
 int countz=0;
 int countbp=0;
 int countbm=0;
 
 while (in2.good()) {
   string tmp;
   double rinv,layer1,bend1,theta1,sensorSpacing1,invpitch1,tiltAngle1,r1,layer2,bend2,theta2,sensorSpacing2,invpitch2,tiltAngle2,r2;
   in2 >> tmp >> rinv >> layer1 >> bend1 >> theta1 >> sensorSpacing1 >> invpitch1 >> tiltAngle1 >> r1 >> layer2 >> bend2 >> theta2 >> sensorSpacing2 >> invpitch2 >> tiltAngle2 >> r2;

   if (tmp == "LL") {
   
     //double cbend1 = (sin(theta1)*sensorSpacing1*sqrt(invpitch1))/(cos(theta1-tiltAngle1)*sqrt(pow(0.01998613/(r1*rinv),2)-1));
     //double cbend2 = (sin(theta2)*sensorSpacing2*sqrt(invpitch2))/(cos(theta2-tiltAngle2)*sqrt(pow(0.01998613/(r2*rinv),2)-1));
     //double cbend1 = (50*fabs(rinv)*r1*sensorSpacing1*sqrt(invpitch1)*((sin(theta1))/(cos(theta1-tiltAngle1))));
     //double cbend2 = (50*fabs(rinv)*r2*sensorSpacing2*sqrt(invpitch2)*((sin(theta2))/(cos(theta2-tiltAngle2))));
     double cbend1 = fabs(50*rinv*r1*1.8*sqrt(invpitch1));
     double cbend2 = fabs(50*rinv*r2*1.8*sqrt(invpitch2));
     
     hist1->Fill(bend1);
     hist1->Fill(bend2);
     if (layer1==1) {
       histL1->Fill(bend1);
       histdL1->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdL1c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer1==2) {
       histL2->Fill(bend1);
       histdL2->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdL2c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer1==3) {
       histL3->Fill(bend1);
       histdL3->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdL3c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer1==4) {
       histL4->Fill(bend1);
       histdL4->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdL4c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer1==5) {
       histL5->Fill(bend1);
       histdL5->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdL5c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer1==6) {
       histL6->Fill(bend1);
       histdL6->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdL6c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer2==1) {
       histL1->Fill(bend2);
       histdL1->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdL1c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }
     if (layer2==2) {
       histL2->Fill(bend2);
       histdL2->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdL2c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }
     if (layer2==3) {
       histL3->Fill(bend2);
       histdL3->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdL3c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }
     if (layer2==4) {
       histL4->Fill(bend2);
       histdL4->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdL4c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }
     if (layer2==5) {
       histL5->Fill(bend2);
       histdL5->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdL5c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }
     if (layer2==6) {
       histL6->Fill(bend2);
       histdL6->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdL6c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }
   }
   else {
     double cbend1 = (sin(theta1)*sensorSpacing1*sqrt(invpitch1))/(cos(theta1)*sqrt(pow(0.01998613/(r1*rinv),2)-1));
     double cbend2 = (sin(theta2)*sensorSpacing2*sqrt(invpitch2))/(cos(theta2)*sqrt(pow(0.01998613/(r2*rinv),2)-1));
     //double cbend1 = (50*fabs(rinv)*r1*sensorSpacing1*sqrt(invpitch1)*((sin(theta1))/(cos(theta1))));
     //double cbend2 = (50*fabs(rinv)*r2*sensorSpacing2*sqrt(invpitch2)*((sin(theta2))/(cos(theta2))));
     //double cbend1 = (theta1/fabs(theta1))*fabs(50*rinv*r1*1.8*sqrt(invpitch1));
     //double cbend2 = (theta2/fabs(theta2))*fabs(50*rinv*r2*1.8*sqrt(invpitch2));

     //cout << bend1 <<" "<< cbend1*(-rinv/fabs(rinv)) << endl;

     //cout << theta1 << endl;

     if (bend1 > 0) {
       countp++;
     }
     if (bend1 < 0) {
       countm++;
     }
     if (bend1 == 0) {
       countz++;
     }
     if (bend2 > 0) {
       countp++;
     }
     if (bend2 < 0) {
       countm++;
     }
     if (bend2 ==0) {
       countz++;
     }

     if (theta1 < 0) {
       bend1=-bend1;
     }
     if (theta2 < 0) {
       bend2=-bend2;
     }

     if (bend1 > 0) {
       countbp++;
     }
     if (bend1 < 0) {
       countbm++;
     }
     if (bend2 > 0) {
       countbp++;
     }
     if (bend2 < 0) {
       countbm++;
     }


     //cout << bend1 <<" "<< cbend1*(-rinv/fabs(rinv)) <<" "<< sin(theta1)/cos(theta1) << endl;

     if (layer1==1) {
       histD1->Fill(bend1);
       //histdL1->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdD1c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer1==2) {
       histD2->Fill(bend1);
       //histdL2->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdD2c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer1==3) {
       histD3->Fill(bend1);
       //histdL3->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdD3c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer1==4) {
       histD4->Fill(bend1);
       //histdL4->Fill(bend1-(cbend1*(rinv/fabs(rinv))));
       histdD4c->Fill(bend1-(cbend1*(-rinv/fabs(rinv))));
     }
     if (layer2==1) {
       histD1->Fill(bend2);
       //histdL1->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdD1c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }
     if (layer2==2) {
       histD2->Fill(bend2);
       //histdL2->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdD2c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }
     if (layer2==3) {
       histD3->Fill(bend2);
       //histdL3->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdD3c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }
     if (layer2==4) {
       histD4->Fill(bend2);
       //histdD4->Fill(bend2-(cbend2*(rinv/fabs(rinv))));
       histdD4c->Fill(bend2-(cbend2*(-rinv/fabs(rinv))));
     }

   }

   count++;

 }

 cout << "Processed: "<<count<<endl;
 cout << "Bend positive before: "<<countp<<endl;
 cout << "Bend negative before: "<<countm<<endl;
 cout << "Bend zero: "<<countz<<endl;
 cout << "Bend positive after: "<<countbp<<endl;
 cout << "Bend negative after: "<<countbm<<endl;
 
 TString type = "singmup_0PU_em_L_simple";

 TCanvas c;

 TFile* fout = new TFile("output_"+type+".root","recreate");
 
 hist1->GetXaxis()->SetTitle("Bend");
 hist1->GetYaxis()->SetTitle("Counts");
 hist1->Draw();
 c.SaveAs(type+"_bend.png");

 histL1->GetXaxis()->SetTitle("Bend");
 histL1->GetYaxis()->SetTitle("Counts");
 histL1->Draw();
 histL1->Write();
 c.SaveAs(type+"_bend_L1.png");

 histL2->GetXaxis()->SetTitle("Bend");
 histL2->GetYaxis()->SetTitle("Counts");
 histL2->Draw();
 histL2->Write();
 c.SaveAs(type+"_bend_L2.png");

 histL3->GetXaxis()->SetTitle("Bend");
 histL3->GetYaxis()->SetTitle("Counts");
 histL3->Draw();
 histL3->Write();
 c.SaveAs(type+"_bend_L3.png");

 histL4->GetXaxis()->SetTitle("Bend");
 histL4->GetYaxis()->SetTitle("Counts");
 histL4->Draw();
 histL4->Write();
 c.SaveAs(type+"_bend_L4.png");

 histL5->GetXaxis()->SetTitle("Bend");
 histL5->GetYaxis()->SetTitle("Counts");
 histL5->Draw();
 histL5->Write();
 c.SaveAs(type+"_bend_L5.png");

 histL6->GetXaxis()->SetTitle("Bend");
 histL6->GetYaxis()->SetTitle("Counts");
 histL6->Draw();
 histL6->Write();
 c.SaveAs(type+"_bend_L6.png");

 histD1->GetXaxis()->SetTitle("Bend");
 histD1->GetYaxis()->SetTitle("Counts");
 histD1->Draw();
 histD1->Write();
 c.SaveAs(type+"_bend_D1.png");

 histD2->GetXaxis()->SetTitle("Bend");
 histD2->GetYaxis()->SetTitle("Counts");
 histD2->Draw();
 histD2->Write();
 c.SaveAs(type+"_bend_D2.png");

 histD3->GetXaxis()->SetTitle("Bend");
 histD3->GetYaxis()->SetTitle("Counts");
 histD3->Draw();
 histD3->Write();
 c.SaveAs(type+"_bend_D3.png");

 histD4->GetXaxis()->SetTitle("Bend");
 histD4->GetYaxis()->SetTitle("Counts");
 histD4->Draw();
 histD4->Write();
 c.SaveAs(type+"_bend_D4.png");

 histdL1->GetXaxis()->SetTitle("Bend Diff");
 histdL1->GetYaxis()->SetTitle("Counts");
 histdL1->Draw();
 c.SaveAs(type+"_bend_dL1.png");

 histdL2->GetXaxis()->SetTitle("Bend Diff");
 histdL2->GetYaxis()->SetTitle("Counts");
 histdL2->Draw();
 c.SaveAs(type+"_bend_dL2.png");

 histdL3->GetXaxis()->SetTitle("Bend Diff");
 histdL3->GetYaxis()->SetTitle("Counts");
 histdL3->Draw();
 c.SaveAs(type+"_bend_dL3.png");

 histdL4->GetXaxis()->SetTitle("Bend Diff");
 histdL4->GetYaxis()->SetTitle("Counts");
 histdL4->Draw();
 c.SaveAs(type+"_bend_dL4.png");

 histdL5->GetXaxis()->SetTitle("Bend Diff");
 histdL5->GetYaxis()->SetTitle("Counts");
 histdL5->Draw();
 c.SaveAs(type+"_bend_dL5.png");

 histdL6->GetXaxis()->SetTitle("Bend Diff");
 histdL6->GetYaxis()->SetTitle("Counts");
 histdL6->Draw();
 c.SaveAs(type+"_bend_dL6.png");

 histdL1c->GetXaxis()->SetTitle("Bend Diff");
 histdL1c->GetYaxis()->SetTitle("Counts");
 histdL1c->Draw();
 histdL1c->Write();
 c.SaveAs(type+"_bend_dL1c.png");

 histdL1c->Fit("gaus");
 histdL1c->GetFunction("gaus")->SetLineColor(kRed);
 c.SaveAs(type+"_bend_dL1c_fit.png");

 histdL2c->GetXaxis()->SetTitle("Bend Diff");
 histdL2c->GetYaxis()->SetTitle("Counts");
 histdL2c->Draw();
 histdL2c->Write();
 c.SaveAs(type+"_bend_dL2c.png");
 
 histdL2c->Fit("gaus");
 histdL2c->GetFunction("gaus")->SetLineColor(kRed);
 c.SaveAs(type+"_bend_dL2c_fit.png");

 histdL3c->GetXaxis()->SetTitle("Bend Diff");
 histdL3c->GetYaxis()->SetTitle("Counts");
 histdL3c->Draw();
 histdL3c->Write();
 c.SaveAs(type+"_bend_dL3c.png");

 histdL3c->Fit("gaus");
 histdL3c->GetFunction("gaus")->SetLineColor(kRed);
 c.SaveAs(type+"_bend_dL3c_fit.png");

 histdL4c->GetXaxis()->SetTitle("Bend Diff");
 histdL4c->GetYaxis()->SetTitle("Counts");
 histdL4c->Draw();
 histdL4c->Write();
 c.SaveAs(type+"_bend_dL4c.png");
 
 histdL4c->Fit("gaus");
 histdL4c->GetFunction("gaus")->SetLineColor(kRed);
 c.SaveAs(type+"_bend_dL4c_fit.png");

 histdL5c->GetXaxis()->SetTitle("Bend Diff");
 histdL5c->GetYaxis()->SetTitle("Counts");
 histdL5c->Draw();
 histdL5c->Write();
 c.SaveAs(type+"_bend_dL5c.png");
 
 histdL5c->Fit("gaus");
 histdL5c->GetFunction("gaus")->SetLineColor(kRed);
 c.SaveAs(type+"_bend_dL5c_fit.png");
 
 histdL6c->GetXaxis()->SetTitle("Bend Diff");
 histdL6c->GetYaxis()->SetTitle("Counts");
 histdL6c->Draw();
 histdL6c->Write();
 c.SaveAs(type+"_bend_dL6c.png");

 histdL6c->Fit("gaus");
 histdL6c->GetFunction("gaus")->SetLineColor(kRed);
 c.SaveAs(type+"_bend_dL6c_fit.png");

 histdD1c->GetXaxis()->SetTitle("Bend Diff");
 histdD1c->GetYaxis()->SetTitle("Counts");
 histdD1c->Draw();
 histdD1c->Write();
 c.SaveAs(type+"_bend_dD1c.png");

 histdD2c->GetXaxis()->SetTitle("Bend Diff");
 histdD2c->GetYaxis()->SetTitle("Counts");
 histdD2c->Draw();
 histdD2c->Write();
 c.SaveAs(type+"_bend_dD2c.png");

 histdD3c->GetXaxis()->SetTitle("Bend Diff");
 histdD3c->GetYaxis()->SetTitle("Counts");
 histdD3c->Draw();
 histdD3c->Write();
 c.SaveAs(type+"_bend_dD3c.png");

 histdD4c->GetXaxis()->SetTitle("Bend Diff");
 histdD4c->GetYaxis()->SetTitle("Counts");
 histdD4c->Draw();
 histdD4c->Write();
 c.SaveAs(type+"_bend_dD4c.png");


 fout->Close();

 cout << "L1 RMS: " << histdL1c->GetRMS() << endl;
 cout << "L2 RMS: " << histdL2c->GetRMS() << endl;
 cout << "L3 RMS: " << histdL3c->GetRMS() << endl;
 cout << "L4 RMS: " << histdL4c->GetRMS() << endl;
 cout << "L5 RMS: " << histdL5c->GetRMS() << endl;
 cout << "L6 RMS: " << histdL6c->GetRMS() << endl;
 cout << "D1 RMS: " << histdD1c->GetRMS() << endl;
 cout << "D2 RMS: " << histdD2c->GetRMS() << endl;
 cout << "D3 RMS: " << histdD3c->GetRMS() << endl;
 cout << "D4 RMS: " << histdD4c->GetRMS() << endl;

 
 /*
 hist2p->GetXaxis()->SetTitle("Bend Difference");
 hist2p->GetYaxis()->SetTitle("Number of stub pairs");
 hist2p->Draw();
 //hist2->Write();
 c.SaveAs(DIR+type+"_bend_diff_L23p.png");

 hist3p->GetXaxis()->SetTitle("Bend Difference");
 hist3p->GetYaxis()->SetTitle("Number of stub pairs");
 hist3p->Draw();
 //hist3->Write();
 c.SaveAs(DIR+type+"_bend_diff_L34p.png");

 hist4p->GetXaxis()->SetTitle("Bend Difference");
 hist4p->GetYaxis()->SetTitle("Number of stub pairs");
 hist4p->Draw();
 //hist4->Write();
 c.SaveAs(DIR+type+"_bend_diff_L45p.png");

 hist5p->GetXaxis()->SetTitle("Bend Difference");
 hist5p->GetYaxis()->SetTitle("Number of stub pairs");
 hist5p->Draw();
 //hist5->Write();
 c.SaveAs(DIR+type+"_bend_diff_L56p.png");
 
 hist1m->GetXaxis()->SetTitle("Bend Difference");
 hist1m->GetYaxis()->SetTitle("Number of stub pairs");
 hist1m->Draw();
 // hist1->Write();
 c.SaveAs(DIR+type+"_bend_diff_L12m.png");

 hist2m->GetXaxis()->SetTitle("Bend Difference");
 hist2m->GetYaxis()->SetTitle("Number of stub pairs");
 hist2m->Draw();
 //hist2->Write();
 c.SaveAs(DIR+type+"_bend_diff_L23m.png");

 hist3m->GetXaxis()->SetTitle("Bend Difference");
 hist3m->GetYaxis()->SetTitle("Number of stub pairs");
 hist3m->Draw();
 //hist3->Write();
 c.SaveAs(DIR+type+"_bend_diff_L34m.png");

 hist4m->GetXaxis()->SetTitle("Bend Difference");
 hist4m->GetYaxis()->SetTitle("Number of stub pairs");
 hist4m->Draw();
 //hist4->Write();
 c.SaveAs(DIR+type+"_bend_diff_L45m.png");

 hist5m->GetXaxis()->SetTitle("Bend Difference");
 hist5m->GetYaxis()->SetTitle("Number of stub pairs");
 hist5m->Draw();
 //hist5->Write();
 c.SaveAs(DIR+type+"_bend_diff_L56m.png");
 */
 /*
 hist1->GetXaxis()->SetTitle("Generation");
 hist1->GetYaxis()->SetTitle("Log(Population Size)");
 hist1->SetMarkerStyle(2);
 hist1->SetMarkerSize(2);
 hist1->Draw();
 // hist1->Write();
 c.SaveAs(type+"_N.png");

 hist2->GetXaxis()->SetTitle("Generation");
 hist2->GetYaxis()->SetTitle("Use Fitness");
 hist2->SetMarkerStyle(2);
 hist2->SetMarkerSize(2);
 hist2->Draw();
 // hist1->Write();
 c.SaveAs(type+"_W.png");

 hist2->Draw();
 hist4->SetMarkerStyle(2);
 hist4->SetMarkerSize(2);
 hist4->SetMarkerColor(kRed);
 hist4->Draw("same");
 hist6->SetMarkerStyle(2);
 hist6->SetMarkerSize(2);
 hist6->SetMarkerColor(kMagenta);
 hist6->Draw("same");
 hist8->SetMarkerStyle(2);
 hist8->SetMarkerSize(2);
 hist8->SetMarkerColor(kGreen);
 hist8->Draw("same");
 hist0->SetMarkerStyle(2);
 hist0->SetMarkerSize(2);
 hist0->SetMarkerColor(kBlue);
 hist0->Draw("same");
 c.SaveAs(type+"_W_overlay.png");

 hist1->Draw();
 hist3->SetMarkerStyle(2);
 hist3->SetMarkerSize(2);
 hist3->SetMarkerColor(kRed);
 hist3->Draw("same");
 hist5->SetMarkerStyle(2);
 hist5->SetMarkerSize(2);
 hist5->SetMarkerColor(kMagenta);
 hist5->Draw("same");
 hist7->SetMarkerStyle(2);
 hist7->SetMarkerSize(2);
 hist7->SetMarkerColor(kGreen);
 hist7->Draw("same");
 hist9->SetMarkerStyle(2);
 hist9->SetMarkerSize(2);
 hist9->SetMarkerColor(kBlue);
 hist9->Draw("same");
 c.SaveAs(type+"_N_overlay.png");
*/

 /*
 TFitResultPtr r1 = hist2->Fit("gaus","S","",7.4,7.5);
 TFitResultPtr r2 = hist2->Fit("gaus","S","",7.5,7.6);

 TF1 *f1 = new TF1("f1", "gaus(0)+gaus(3)");
 f1->SetParameter(0,r1->Parameter(0));
 f1->SetParameter(1,r1->Parameter(1));
 f1->SetParameter(2,r1->Parameter(2));
 f1->SetParameter(3,r2->Parameter(0));
 f1->SetParameter(4,r2->Parameter(1));
 f1->SetParameter(5,r2->Parameter(2));
 hist2->Fit("f1");
 c.SaveAs(type+"_Am_fit.png");
 
 hist3->GetXaxis()->SetTitle("Energy");
 hist3->GetYaxis()->SetTitle("Counts");
 hist3->Draw();
 // hist1->Write();
 c.SaveAs(type+"_Gd.png");

 TFitResultPtr r3 = hist3->Fit("gaus","S","",3,5);

 TF1 *f2 = new TF1("f2","gaus");

 f2->SetParameter(0,r3->Parameter(0));
 f2->SetParameter(1,r3->Parameter(1));
 f2->SetParameter(2,r3->Parameter(2));
 hist3->Fit("f2");
 c.SaveAs(type+"_Gd_fit.png");

 TF1 *f3 = new TF1("f3","[0]*x + [1]");
 
 //hist4->Fit("f3");
 //c.SaveAs(type+"_slope_fit.png");
 
 g1->SetMarkerStyle(3);
 g1->Fit("f3");
 g1->Draw("ap");
 c.SaveAs(type+"_slope_fit_g.png");
 
 g1e->SetMarkerStyle(3);
 g1e->SetMarkerSize(3);
 g1e->Fit("f3");
 g1e->Draw("ap");
 c.SaveAs(type+"_slope_fit_ge.png");
 */ 
 /*
 hist2->GetXaxis()->SetTitle("Bend Difference");
 hist2->GetYaxis()->SetTitle("Number of stub pairs");
 hist2->Draw();
 //hist2->Write();
 c.SaveAs(DIR+type+"_bend_diff_L23.png");

 hist3->GetXaxis()->SetTitle("Bend Difference");
 hist3->GetYaxis()->SetTitle("Number of stub pairs");
 hist3->Draw();
 //hist3->Write();
 c.SaveAs(DIR+type+"_bend_diff_L34.png");

 hist4->GetXaxis()->SetTitle("Bend Difference");
 hist4->GetYaxis()->SetTitle("Number of stub pairs");
 hist4->Draw();
 //hist4->Write();
 c.SaveAs(DIR+type+"_bend_diff_L45.png");

 hist5->GetXaxis()->SetTitle("Bend Difference");
 hist5->GetYaxis()->SetTitle("Number of stub pairs");
 hist5->Draw();
 //hist5->Write();
 c.SaveAs(DIR+type+"_bend_diff_L56.png");
 */


}


