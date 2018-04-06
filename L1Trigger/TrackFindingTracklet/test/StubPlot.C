#include "TROOT.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TMath.h"
#include <TError.h>

#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <utility>

using namespace std;

void SetPlotStyle();
void mySmallText(Double_t x,Double_t y,Color_t color,char *text);

void StubPlot(TString type) {
  
  gROOT->SetBatch();
  gErrorIgnoreLevel = kWarning;
  
  SetPlotStyle();

  // Get ROOT tree

  TChain* tree = new TChain("L1TrackNtuple/eventTree");
  tree->Add(type+".root");
  
  if (tree->GetEntries() == 0) {
    cout << "File doesn't exist or is empty, returning..." << endl;
    return;
  }
  
  // Define variables

  vector<float>* allstub_trigBend;
  vector<float>* allstub_matchTP_pt;
  vector<int>* allstub_matchTP_pdgid;
  vector<int>* allstub_isBarrel;
  vector<int>* allstub_layer;
  vector<float>* allstub_x;
  vector<float>* allstub_y;
  vector<float>* allstub_z;
  vector<float>* allstub_ptFromBend;
  vector<float>* allstub_det_r;
  vector<float>* allstub_det_z;
  vector<float>* allstub_det_tiltAngle;
  vector<float>* allstub_det_sensorSpacing;
  vector<float>* allstub_det_pitch;
  vector<float>* allstub_det_theta0;

  TBranch* b_allstub_trigBend;
  TBranch* b_allstub_matchTP_pt;
  TBranch* b_allstub_matchTP_pdgid;
  TBranch* b_allstub_isBarrel;
  TBranch* b_allstub_layer;
  TBranch* b_allstub_x;
  TBranch* b_allstub_y;
  TBranch* b_allstub_z;
  TBranch* b_allstub_ptFromBend;
  TBranch* b_allstub_det_r;
  TBranch* b_allstub_det_z;
  TBranch* b_allstub_det_tiltAngle;
  TBranch* b_allstub_det_sensorSpacing;
  TBranch* b_allstub_det_pitch;
  TBranch* b_allstub_det_theta0;

  allstub_trigBend = 0;
  allstub_matchTP_pt = 0;
  allstub_matchTP_pdgid = 0;
  allstub_isBarrel = 0;
  allstub_layer = 0;
  allstub_x = 0;
  allstub_y = 0;
  allstub_z = 0;
  allstub_ptFromBend = 0;
  allstub_det_r = 0;
  allstub_det_z = 0;
  allstub_det_tiltAngle = 0;
  allstub_det_sensorSpacing = 0;
  allstub_det_pitch = 0;
  allstub_det_theta0 = 0;

 
  tree->SetBranchAddress("allstub_trigBend", &allstub_trigBend, &b_allstub_trigBend);
  tree->SetBranchAddress("allstub_matchTP_pt", &allstub_matchTP_pt, &b_allstub_matchTP_pt);
  tree->SetBranchAddress("allstub_matchTP_pdgid", &allstub_matchTP_pdgid, &b_allstub_matchTP_pdgid);
  tree->SetBranchAddress("allstub_isBarrel", &allstub_isBarrel, &b_allstub_isBarrel);
  tree->SetBranchAddress("allstub_layer", &allstub_layer, &b_allstub_layer);
  tree->SetBranchAddress("allstub_x", &allstub_x, &b_allstub_x);
  tree->SetBranchAddress("allstub_y", &allstub_y, &b_allstub_y);
  tree->SetBranchAddress("allstub_z", &allstub_z, &b_allstub_z);
  tree->SetBranchAddress("allstub_ptFromBend", &allstub_ptFromBend, &b_allstub_ptFromBend);
  tree->SetBranchAddress("allstub_det_r", &allstub_det_r, &b_allstub_det_r);
  tree->SetBranchAddress("allstub_det_z", &allstub_det_z, &b_allstub_det_z);
  tree->SetBranchAddress("allstub_det_tiltAngle", &allstub_det_tiltAngle, &b_allstub_det_tiltAngle);
  tree->SetBranchAddress("allstub_det_sensorSpacing", &allstub_det_sensorSpacing, &b_allstub_det_sensorSpacing);
  tree->SetBranchAddress("allstub_det_pitch", &allstub_det_pitch, &b_allstub_det_pitch);
  tree->SetBranchAddress("allstub_det_theta0", &allstub_det_theta0, &b_allstub_det_theta0);
  
  // Define histograms
  TH1F* hist1 = new TH1F("h1",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* hist1i = new TH1F("h1i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histL1 = new TH1F("hL1",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histL1i = new TH1F("hL1i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histL2 = new TH1F("hL2",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histL2i = new TH1F("hL2i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histL3 = new TH1F("hL3",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histL3i = new TH1F("hL3i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histL4 = new TH1F("hL4",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histL4i = new TH1F("hL4i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histL5 = new TH1F("hL5",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histL5i = new TH1F("hL5i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histL6 = new TH1F("hL6",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histL6i = new TH1F("hL6i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histD1 = new TH1F("hD1",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histD1i = new TH1F("hD1i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histD2 = new TH1F("hD2",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histD2i = new TH1F("hD2i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histD3 = new TH1F("hD3",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histD3i = new TH1F("hD3i",";p_{T} difference; Count", 100,-50,50);

  TH1F* histD4 = new TH1F("hD4",";Inverse p_{T} difference; Count", 80,-4,4);
  TH1F* histD4i = new TH1F("hD4i",";p_{T} difference; Count", 100,-50,50);

  int nevt = tree->GetEntries();

  // Event loop

  int count=0;

  for (int i=0; i<nevt; i++) {

    tree->GetEntry(i,0);

    // Stub loop

    for (int it=0; it<(int)allstub_trigBend->size(); it++) {
      int pdgid = allstub_matchTP_pdgid->at(it);
      float charge = (allstub_matchTP_pdgid->at(it))/(fabs(allstub_matchTP_pdgid->at(it)));
      if (fabs(allstub_matchTP_pdgid->at(it)) < 21) {
	charge = -charge;
      }
      // Might want to look more closely at this...
      float pt = charge*allstub_matchTP_pt->at(it); // Giving a sign to pt
      float x_pos = allstub_x->at(it);
      float y_pos = allstub_y->at(it);
      float z_pos = allstub_z->at(it);
      float r_pos = sqrt(x_pos*x_pos + y_pos*y_pos);
      float bend = allstub_trigBend->at(it);
      float bcharge = (bend)/(fabs(bend));
      int barrel = allstub_isBarrel->at(it);
      int layer = allstub_layer->at(it);
      float pt_st = allstub_ptFromBend->at(it);
      float phi = atan2(y_pos,x_pos);
      float det_r = allstub_det_r->at(it);
      float det_z = allstub_det_z->at(it);
      float det_tiltAngle = allstub_det_tiltAngle->at(it);
      float det_sensorSpacing = allstub_det_sensorSpacing->at(it);
      float det_pitch = allstub_det_pitch->at(it);
      float det_theta0 = allstub_det_theta0->at(it);

      //cout << pt <<" "<< pt_st << endl;
      
      if (pt==999) {
	continue;
      }

      if (fabs(pt)<2) {
	continue;
      }

      //cout << pt <<" "<< pt_st <<" "<< bcharge << endl;

      float diff = 1.0/pt - 1.0/pt_st;
      float diffi = pt - pt_st;
      
      // Fill histograms
      hist1->Fill(diff);
      hist1i->Fill(diffi);
      
      if (barrel) {
	if (layer == 1) {
	  histL1->Fill(diff);
	  histL1i->Fill(diffi);
	}
	if (layer == 2) {
          histL2->Fill(diff);
          histL2i->Fill(diffi);
        }
	if (layer == 3) {
          histL3->Fill(diff);
          histL3i->Fill(diffi);
        }
	if (layer == 4) {
          histL4->Fill(diff);
          histL4i->Fill(diffi);
        }
	if (layer == 5) {
	  histL5->Fill(diff);
	  histL5i->Fill(diffi);
	}
	if (layer == 6) {
          histL6->Fill(diff);
          histL6i->Fill(diffi);
        }
      }
      else {
	if (layer == 1) {
	  histD1->Fill(diff);
	  histD1i->Fill(diffi);
	}
	if (layer == 2) {
          histD2->Fill(diff);
          histD2i->Fill(diffi);
        }
	if (layer == 3) {
          histD3->Fill(diff);
          histD3i->Fill(diffi);
        }
	if (layer == 4) {
          histD4->Fill(diff);
          histD4i->Fill(diffi);
        }
      }

      count++;

    }
  }

  cout << count << endl;

  // Write histograms

  TCanvas c;

  TString DIR = "StubPlots/";

  type = DIR+type;

  hist1->GetXaxis()->SetTitle("pt_{inv} diff");
  hist1->GetYaxis()->SetTitle("Counts");
  hist1->Draw();
  c.SaveAs(type+"_diff.png");

  hist1i->GetXaxis()->SetTitle("pt diff");
  hist1i->GetYaxis()->SetTitle("Counts");
  hist1i->Draw();
  c.SaveAs(type+"_diffi.png");

  histL1->GetXaxis()->SetTitle("pt_{inv} diff");
  histL1->GetYaxis()->SetTitle("Counts");
  histL1->Draw();
  c.SaveAs(type+"_diff_L1.png");

  histL1i->GetXaxis()->SetTitle("pt diff");
  histL1i->GetYaxis()->SetTitle("Counts");
  histL1i->Draw();
  c.SaveAs(type+"_diffi_L1.png");

  histL2->GetXaxis()->SetTitle("pt_{inv} diff");
  histL2->GetYaxis()->SetTitle("Counts");
  histL2->Draw();
  c.SaveAs(type+"_diff_L2.png");

  histL2i->GetXaxis()->SetTitle("pt diff");
  histL2i->GetYaxis()->SetTitle("Counts");
  histL2i->Draw();
  c.SaveAs(type+"_diffi_L2.png");

  histL3->GetXaxis()->SetTitle("pt_{inv} diff");
  histL3->GetYaxis()->SetTitle("Counts");
  histL3->Draw();
  c.SaveAs(type+"_diff_L3.png");

  histL3i->GetXaxis()->SetTitle("pt diff");
  histL3i->GetYaxis()->SetTitle("Counts");
  histL3i->Draw();
  c.SaveAs(type+"_diffi_L3.png");

  histL4->GetXaxis()->SetTitle("pt_{inv} diff");
  histL4->GetYaxis()->SetTitle("Counts");
  histL4->Draw();
  c.SaveAs(type+"_diff_L4.png");

  histL4i->GetXaxis()->SetTitle("pt diff");
  histL4i->GetYaxis()->SetTitle("Counts");
  histL4i->Draw();
  c.SaveAs(type+"_diffi_L4.png");

  histL5->GetXaxis()->SetTitle("pt_{inv} diff");
  histL5->GetYaxis()->SetTitle("Counts");
  histL5->Draw();
  c.SaveAs(type+"_diff_L5.png");

  histL5i->GetXaxis()->SetTitle("pt diff");
  histL5i->GetYaxis()->SetTitle("Counts");
  histL5i->Draw();
  c.SaveAs(type+"_diffi_L5.png");

  histL6->GetXaxis()->SetTitle("pt_{inv} diff");
  histL6->GetYaxis()->SetTitle("Counts");
  histL6->Draw();
  c.SaveAs(type+"_diff_L6.png");

  histL6i->GetXaxis()->SetTitle("pt diff");
  histL6i->GetYaxis()->SetTitle("Counts");
  histL6i->Draw();
  c.SaveAs(type+"_diffi_L6.png");

  histD1->GetXaxis()->SetTitle("pt_{inv} diff");
  histD1->GetYaxis()->SetTitle("Counts");
  histD1->Draw();
  c.SaveAs(type+"_diff_D1.png");

  histD1i->GetXaxis()->SetTitle("pt diff");
  histD1i->GetYaxis()->SetTitle("Counts");
  histD1i->Draw();
  c.SaveAs(type+"_diffi_D1.png");

  histD2->GetXaxis()->SetTitle("pt_{inv} diff");
  histD2->GetYaxis()->SetTitle("Counts");
  histD2->Draw();
  c.SaveAs(type+"_diff_D2.png");

  histD2i->GetXaxis()->SetTitle("pt diff");
  histD2i->GetYaxis()->SetTitle("Counts");
  histD2i->Draw();
  c.SaveAs(type+"_diffi_D2.png");

  histD3->GetXaxis()->SetTitle("pt_{inv} diff");
  histD3->GetYaxis()->SetTitle("Counts");
  histD3->Draw();
  c.SaveAs(type+"_diff_D3.png");

  histD3i->GetXaxis()->SetTitle("pt diff");
  histD3i->GetYaxis()->SetTitle("Counts");
  histD3i->Draw();
  c.SaveAs(type+"_diffi_D3.png");

  histD4->GetXaxis()->SetTitle("pt_{inv} diff");
  histD4->GetYaxis()->SetTitle("Counts");
  histD4->Draw();
  c.SaveAs(type+"_diff_D4.png");

  histD4i->GetXaxis()->SetTitle("pt diff");
  histD4i->GetYaxis()->SetTitle("Counts");
  histD4i->Draw();
  c.SaveAs(type+"_diffi_D14.png");


}

void SetPlotStyle() {

  // use plain black on white colors
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetHistLineColor(1);

  gStyle->SetPalette(1);

  // set the paper & margin sizes
  gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.16);
  gStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  gStyle->SetTitleXOffset(1.4);
  gStyle->SetTitleYOffset(1.4);

  // use large fonts
  gStyle->SetTextFont(42);
  gStyle->SetTextSize(0.05);
  gStyle->SetLabelFont(42,"x");
  gStyle->SetTitleFont(42,"x");
  gStyle->SetLabelFont(42,"y");
  gStyle->SetTitleFont(42,"y");
  gStyle->SetLabelFont(42,"z");
  gStyle->SetTitleFont(42,"z");
  gStyle->SetLabelSize(0.05,"x");
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetLabelSize(0.05,"y");
  gStyle->SetTitleSize(0.05,"y");
  gStyle->SetLabelSize(0.05,"z");
  gStyle->SetTitleSize(0.05,"z");

  // use bold lines and markers
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth(2.);
  gStyle->SetLineStyleString(2,"[12 12]");

  // get rid of error bar caps
  gStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(1);
  gStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);

}

void mySmallText(Double_t x,Double_t y,Color_t color,char *text) {
  Double_t tsize=0.044;
  TLatex l;
  l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}
