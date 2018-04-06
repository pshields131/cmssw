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

void StubPlotter(TString type) {
  
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

  vector<float>* tp_pt;
  vector<float>* allstub_matchTP_eta;
  vector<int>* tp_nstub;
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

  TBranch* b_tp_pt;
  TBranch* b_allstub_matchTP_eta;
  TBranch* b_tp_nstub;
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

  tp_pt = 0;
  allstub_matchTP_eta = 0;
  tp_nstub = 0;
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

  tree->SetBranchAddress("tp_pt", &tp_pt, &b_tp_pt);
  tree->SetBranchAddress("allstub_matchTP_eta", &allstub_matchTP_eta, &b_allstub_matchTP_eta);
  tree->SetBranchAddress("tp_nstub", &tp_nstub, &b_tp_nstub);
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

  TH1F* hist1b = new TH1F("h1b",";Bend difference; Count", 100,-10,10);

  TH1F* histbL1 = new TH1F("hbL1",";Bend; Count",16,-4,4);

  TH2F* plotL1 = new TH2F("pL1",";Bend; Predicted Bend", 17,-4.25,4.25, 40,-4,4);
  TH2F* plotL2 = new TH2F("pL2",";Bend; Predicted Bend", 21,-5.25,5.25, 50,-5,5);
  TH2F* plotL3 = new TH2F("pL3",";Bend; Predicted Bend", 25,-6.25,6.25, 60,-6,6);
  TH2F* plotL4 = new TH2F("pL4",";Bend; Predicted Bend", 29,-7.25,7.25, 70,-7,7);
  TH2F* plotL5 = new TH2F("pL5",";Bend; Predicted Bend", 33,-8.25,8.25, 80,-8,8);
  TH2F* plotL6 = new TH2F("pL6",";Bend; Predicted Bend", 33,-8.25,8.25, 90,-9,9);

  TH2F* plotptL1 = new TH2F("pptL1",";p_{T}; {delta}Bend", 80,-10,10,40,-5,5);
  TH2F* plotptL2 = new TH2F("pptL2",";p_{T}; {delta}Bend", 80,-10,10,40,-5,5);
  TH2F* plotptL3 = new TH2F("pptL3",";p_{T}; {delta}Bend", 80,-10,10,48,-6,6);
  TH2F* plotptL4 = new TH2F("pptL4",";p_{T}; {delta}Bend", 80,-10,10,56,-7,7);
  TH2F* plotptL5 = new TH2F("pptL5",";p_{T}; {delta}Bend", 80,-10,10,64,-8,8);
  TH2F* plotptL6 = new TH2F("pptL6",";p_{T}; {delta}Bend", 80,-10,10,72,-9,9);

  TH2F* plotbendL1 = new TH2F("pbendL1",";Bend; {delta}Bend", 16,-4,4,40,-5,5);
  TH2F* plotbendL2 = new TH2F("pbendL2",";Bend; {delta}Bend", 20,-5,5,40,-5,5);
  TH2F* plotbendL3 = new TH2F("pbendL3",";Bend; {delta}Bend", 24,-6,6,48,-6,6);
  TH2F* plotbendL4 = new TH2F("pbendL4",";Bend; {delta}Bend", 28,-7,7,56,-7,7);
  TH2F* plotbendL5 = new TH2F("pbendL5",";Bend; {delta}Bend", 32,-8,8,64,-8,8);
  TH2F* plotbendL6 = new TH2F("pbendL6",";Bend; {delta}Bend", 32,-8,8,72,-9,9);

  TH2F* plotbendL1m = new TH2F("pbendL1",";Bend; {delta}Bend", 16,-4,4,40,-5,5);
  TH2F* plotbendL2m = new TH2F("pbendL2",";Bend; {delta}Bend", 20,-5,5,40,-5,5);
  TH2F* plotbendL3m = new TH2F("pbendL3",";Bend; {delta}Bend", 24,-6,6,48,-6,6);
  TH2F* plotbendL4m = new TH2F("pbendL4",";Bend; {delta}Bend", 28,-7,7,56,-7,7);
  TH2F* plotbendL5m = new TH2F("pbendL5",";Bend; {delta}Bend", 32,-8,8,64,-8,8);
  TH2F* plotbendL6m = new TH2F("pbendL6",";Bend; {delta}Bend", 32,-8,8,72,-9,9);

  TProfile* tpptL1 = new TProfile("tpptL1",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL2 = new TProfile("tpptL2",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL3 = new TProfile("tpptL3",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL4 = new TProfile("tpptL4",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL5 = new TProfile("tpptL5",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL6 = new TProfile("tpptL6",";p_{T}; {delta}Bend", 32,2,10);

  TProfile* tpptL1m = new TProfile("tpptL1m",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL2m = new TProfile("tpptL2m",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL3m = new TProfile("tpptL3m",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL4m = new TProfile("tpptL4m",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL5m = new TProfile("tpptL5m",";p_{T}; {delta}Bend", 32,2,10);
  TProfile* tpptL6m = new TProfile("tpptL6m",";p_{T}; {delta}Bend", 32,2,10);

  TProfile* tpetaL1 = new TProfile("tpetaL1",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL2 = new TProfile("tpetaL2",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL3 = new TProfile("tpetaL3",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL4 = new TProfile("tpetaL4",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL5 = new TProfile("tpetaL5",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL6 = new TProfile("tpetaL6",";eta; {delta}Bend", 48,-2.4,2.4);

  TProfile* tpetaL1m = new TProfile("tpetaL1m",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL2m = new TProfile("tpetaL2m",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL3m = new TProfile("tpetaL3m",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL4m = new TProfile("tpetaL4m",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL5m = new TProfile("tpetaL5m",";eta; {delta}Bend", 48,-2.4,2.4);
  TProfile* tpetaL6m = new TProfile("tpetaL6m",";eta; {delta}Bend", 48,-2.4,2.4);

  TProfile* tpbendL1 = new TProfile("tpbendL1",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL2 = new TProfile("tpbendL2",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL3 = new TProfile("tpbendL3",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL4 = new TProfile("tpbendL4",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL5 = new TProfile("tpbendL5",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL6 = new TProfile("tpbendL6",";bend; {delta}Bend", 32,-8,8);

  TProfile* tpbendL1m = new TProfile("tpbendL1m",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL2m = new TProfile("tpbendL2m",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL3m = new TProfile("tpbendL3m",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL4m = new TProfile("tpbendL4m",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL5m = new TProfile("tpbendL5m",";bend; {delta}Bend", 32,-8,8);
  TProfile* tpbendL6m = new TProfile("tpbendL6m",";bend; {delta}Bend", 32,-8,8);

  TProfile2D* tp2dL1 = new TProfile2D("tp2dL1",";p_{T}; Eta",80,-10,10,20,-1,1);
  TProfile2D* tp2dL2 = new TProfile2D("tp2dL2",";p_{T}; Eta",80,-10,10,20,-1,1);
  TProfile2D* tp2dL3 = new TProfile2D("tp2dL3",";p_{T}; Eta",80,-10,10,20,-1,1);
  TProfile2D* tp2dL4 = new TProfile2D("tp2dL4",";p_{T}; Eta",80,-10,10,20,-1,1);
  TProfile2D* tp2dL5 = new TProfile2D("tp2dL5",";p_{T}; Eta",80,-10,10,20,-1,1);
  TProfile2D* tp2dL6 = new TProfile2D("tp2dL6",";p_{T}; Eta",80,-10,10,20,-1,1);

  int nevt = tree->GetEntries();

  // Event loop

  int count=0;
  int count1=0;
  int count2=0;

  for (int i=0; i<nevt; i++) {

    tree->GetEntry(i,0);

    // Stub loop

    for (int it=0; it<(int)allstub_trigBend->size(); it++) {
      int pdgid = allstub_matchTP_pdgid->at(it);
      float charge = (allstub_matchTP_pdgid->at(it))/(fabs(allstub_matchTP_pdgid->at(it)));
      if (fabs(allstub_matchTP_pdgid->at(it)) < 21) {
        charge = -charge;
      }
      float pt = charge*allstub_matchTP_pt->at(it); // Giving a sign to pt
      float eta = allstub_matchTP_eta->at(it);
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

      float det_pitch = allstub_det_pitch->at(it)*0.001;
      float det_theta0 = allstub_det_theta0->at(it);

      if (fabs(pt)==999) {
	continue;
      }
      
      if (fabs(pt) < 2) {
	continue;
      }

      //float bend_p = -charge*fabs(((sin(det_theta0)*det_sensorSpacing)/(det_pitch*cos(det_theta0-det_tiltAngle)))*sqrt(1/((pow((pt)/(0.57*det_r),2))-1)));
      //float bend_p = -charge*fabs((sin(det_theta0)*det_sensorSpacing*0.57*det_r)/(det_pitch*cos(det_theta0-det_tiltAngle)*pt));
      float bend_p = -charge*fabs((1.8*0.57*det_r)/(det_pitch*pt));

      float diffb = bend - bend_p;

      if (barrel) {
	if (layer == 1) {
	  plotL1->Fill(bend,bend_p);
	  tp2dL1->Fill(pt,eta,diffb);
	  plotptL1->Fill(pt,diffb);
	  if (pdgid < 0) {
	    histbL1->Fill(bend);
	    tpptL1->Fill(fabs(pt),diffb);
	    tpetaL1->Fill(eta,diffb);
	    tpbendL1->Fill(bend,diffb);
	    plotbendL1->Fill(bend,diffb);
	    if (bend==-2.5) {
	      //cout << bend <<" "<< pt <<" "<< diffb <<" "<< eta << endl;
	    }
	  }
	  else {
	    tpptL1m->Fill(fabs(pt),diffb);
	    tpetaL1m->Fill(eta,diffb);
            tpbendL1m->Fill(bend,diffb);
	    plotbendL1m->Fill(bend,diffb);
	  }
	}
	if (layer == 2) {
          plotL2->Fill(bend,bend_p);
          tp2dL2->Fill(pt,eta,diffb);
	  plotptL2->Fill(pt,diffb);
	  if (pdgid < 0) {
            tpptL2->Fill(fabs(pt),diffb);
	    tpetaL2->Fill(eta,diffb);
            tpbendL2->Fill(bend,diffb);
	    plotbendL2->Fill(bend,diffb);
	  }
          else {
            tpptL2m->Fill(fabs(pt),diffb);
	    tpetaL2m->Fill(eta,diffb);
            tpbendL2m->Fill(bend,diffb);
	    plotbendL2m->Fill(bend,diffb);
	  }
        }
	if (layer == 3) {
          plotL3->Fill(bend,bend_p);
          tp2dL3->Fill(pt,eta,diffb);
	  plotptL3->Fill(pt,diffb);
	  if (pdgid < 0) {
            tpptL3->Fill(fabs(pt),diffb);
	    tpetaL3->Fill(eta,diffb);
            tpbendL3->Fill(bend,diffb);
	    plotbendL3->Fill(bend,diffb);
	  }
          else {
            tpptL3m->Fill(fabs(pt),diffb);
	    tpetaL3m->Fill(eta,diffb);
            tpbendL3m->Fill(bend,diffb);
	    plotbendL3m->Fill(bend,diffb);
	  }
        }
	if (layer == 4) {
          plotL4->Fill(bend,bend_p);
          tp2dL4->Fill(pt,eta,diffb);
	  plotptL4->Fill(pt,diffb);
	  if (pdgid < 0) {
            tpptL4->Fill(fabs(pt),diffb);
	    tpetaL4->Fill(eta,diffb);
            tpbendL4->Fill(bend,diffb);
	    plotbendL4->Fill(bend,diffb);
	  }
          else {
            tpptL4m->Fill(fabs(pt),diffb);
	    tpetaL4m->Fill(eta,diffb);
            tpbendL4m->Fill(bend,diffb);
	    plotbendL4m->Fill(bend,diffb);
	  }
        }
	if (layer == 5) {
          plotL5->Fill(bend,bend_p);
          tp2dL5->Fill(pt,eta,diffb);
	  plotptL5->Fill(pt,diffb);
	  if (pdgid < 0) {
            tpptL5->Fill(fabs(pt),diffb);
	    tpetaL5->Fill(eta,diffb);
            tpbendL5->Fill(bend,diffb);
	    plotbendL5->Fill(bend,diffb);
	  }
          else {
            tpptL5m->Fill(fabs(pt),diffb);
	    tpetaL5m->Fill(eta,diffb);
            tpbendL5m->Fill(bend,diffb);
	    plotbendL5m->Fill(bend,diffb);
	  }
        }
	if (layer == 6) {
	  plotL6->Fill(bend,bend_p);
	  tp2dL6->Fill(pt,eta,diffb);
	  plotptL6->Fill(pt,diffb);
	  if (pdgid < 0) {
            tpptL6->Fill(fabs(pt),diffb);
	    tpetaL6->Fill(eta,diffb);
            tpbendL6->Fill(bend,diffb);
	    plotbendL6->Fill(bend,diffb);
	    if (bend > 2) {
	      cout << bend <<" "<< pt <<" "<< diffb <<" "<< r_pos - det_r <<" "<< det_theta0 << endl;
	    }
	  }
          else {
            tpptL6m->Fill(fabs(pt),diffb);
	    tpetaL6m->Fill(eta,diffb);
            tpbendL6m->Fill(bend,diffb);
	    plotbendL6m->Fill(bend,diffb);
	  }
	}
      }

      hist1b->Fill(diffb);
      
      count++;

    }
  }

  cout << count << endl;

  // Write histograms

  TCanvas c;

  TString DIR = "ColorPlots/";

  TFile* fout = new TFile("output_bend_"+type+".root","recreate");

  type = DIR+type+"_SIMP";

  hist1b->GetXaxis()->SetTitle("#Delta Bend");
  hist1b->GetYaxis()->SetTitle("Counts");
  hist1b->Draw();
  c.SaveAs(type+"_diffb.png");

  histbL1->GetXaxis()->SetTitle("Bend");
  histbL1->GetYaxis()->SetTitle("Counts");
  histbL1->Draw();
  c.SaveAs(type+"_bendL1.png");

  TF1* f1 = new TF1("f1","x",-10,10);

  plotL1->GetXaxis()->SetTitle("Actual Bend");
  plotL1->GetYaxis()->SetTitle("Predicted Bend");
  plotL1->Draw("COLZ");
  f1->Draw("SAME");
  plotL1->Write();
  c.SaveAs(type+"_bendvspbend_L1.png");
  
  plotL2->GetXaxis()->SetTitle("Actual Bend");
  plotL2->GetYaxis()->SetTitle("Predicted Bend");
  plotL2->Draw("COLZ");
  f1->Draw("SAME");
  plotL2->Write();
  c.SaveAs(type+"_bendvspbend_L2.png");

  plotL3->GetXaxis()->SetTitle("Actual Bend");
  plotL3->GetYaxis()->SetTitle("Predicted Bend");
  plotL3->Draw("COLZ");
  f1->Draw("SAME");
  plotL3->Write();
  c.SaveAs(type+"_bendvspbend_L3.png");

  plotL4->GetXaxis()->SetTitle("Actual Bend");
  plotL4->GetYaxis()->SetTitle("Predicted Bend");
  plotL4->Draw("COLZ");
  f1->Draw("SAME");
  plotL4->Write();
  c.SaveAs(type+"_bendvspbend_L4.png");

  plotL5->GetXaxis()->SetTitle("Actual Bend");
  plotL5->GetYaxis()->SetTitle("Predicted Bend");
  plotL5->Draw("COLZ");
  f1->Draw("SAME");
  plotL5->Write();
  c.SaveAs(type+"_bendvspbend_L5.png");

  plotL6->GetXaxis()->SetTitle("Actual Bend");
  plotL6->GetYaxis()->SetTitle("Predicted Bend");
  plotL6->Draw("COLZ");
  f1->Draw("SAME");
  plotL6->Write();
  c.SaveAs(type+"_bendvspbend_L6.png");
  
  plotptL1->GetXaxis()->SetTitle("p_{T}");
  plotptL1->GetYaxis()->SetTitle("#Delta Bend");
  plotptL1->Draw("COLZ");
  plotptL1->Write();
  c.SaveAs(type+"_dbendvspt_L1.png");

  plotptL2->GetXaxis()->SetTitle("p_{T}");
  plotptL2->GetYaxis()->SetTitle("#Delta Bend");
  plotptL2->Draw("COLZ");
  plotptL2->Write();
  c.SaveAs(type+"_dbendvspt_L2.png");  
  
  plotptL3->GetXaxis()->SetTitle("p_{T}");
  plotptL3->GetYaxis()->SetTitle("#Delta Bend");
  plotptL3->Draw("COLZ");
  plotptL3->Write();
  c.SaveAs(type+"_dbendvspt_L3.png");

  plotptL4->GetXaxis()->SetTitle("p_{T}");
  plotptL4->GetYaxis()->SetTitle("#Delta Bend");
  plotptL4->Draw("COLZ");
  plotptL4->Write();
  c.SaveAs(type+"_dbendvspt_L4.png");

  plotptL5->GetXaxis()->SetTitle("p_{T}");
  plotptL5->GetYaxis()->SetTitle("#Delta Bend");
  plotptL5->Draw("COLZ");
  plotptL5->Write();
  c.SaveAs(type+"_dbendvspt_L5.png");

  plotptL6->GetXaxis()->SetTitle("p_{T}");
  plotptL6->GetYaxis()->SetTitle("#Delta Bend");
  plotptL6->Draw("COLZ");
  plotptL6->Write();
  c.SaveAs(type+"_dbendvspt_L6.png");

  plotbendL1->GetXaxis()->SetTitle("Bend");
  plotbendL1->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL1->Draw("COLZ");
  plotbendL1->Write();
  c.SaveAs(type+"_dbendvsbend_L1.png");
  
  plotbendL2->GetXaxis()->SetTitle("Bend");
  plotbendL2->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL2->Draw("COLZ");
  plotbendL2->Write();
  c.SaveAs(type+"_dbendvsbend_L2.png");

  plotbendL3->GetXaxis()->SetTitle("Bend");
  plotbendL3->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL3->Draw("COLZ");
  plotbendL3->Write();
  c.SaveAs(type+"_dbendvsbend_L3.png");

  plotbendL1m->GetXaxis()->SetTitle("Bend");
  plotbendL1m->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL1m->Draw("COLZ");
  plotbendL1m->Write();
  c.SaveAs(type+"_dbendvsbend_L1m.png");

  plotbendL2m->GetXaxis()->SetTitle("Bend");
  plotbendL2m->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL2m->Draw("COLZ");
  plotbendL2m->Write();
  c.SaveAs(type+"_dbendvsbend_L2m.png");

  plotbendL3m->GetXaxis()->SetTitle("Bend");
  plotbendL3m->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL3m->Draw("COLZ");
  plotbendL3m->Write();
  c.SaveAs(type+"_dbendvsbend_L3m.png");

  plotbendL4->GetXaxis()->SetTitle("Bend");
  plotbendL4->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL4->Draw("COLZ");
  plotbendL4->Write();
  c.SaveAs(type+"_dbendvsbend_L4.png");

  plotbendL5->GetXaxis()->SetTitle("Bend");
  plotbendL5->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL5->Draw("COLZ");
  plotbendL5->Write();
  c.SaveAs(type+"_dbendvsbend_L5.png");

  plotbendL6->GetXaxis()->SetTitle("Bend");
  plotbendL6->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL6->Draw("COLZ");
  plotbendL6->Write();
  c.SaveAs(type+"_dbendvsbend_L6.png");

  plotbendL4m->GetXaxis()->SetTitle("Bend");
  plotbendL4m->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL4m->Draw("COLZ");
  plotbendL4m->Write();
  c.SaveAs(type+"_dbendvsbend_L4m.png");

  plotbendL5m->GetXaxis()->SetTitle("Bend");
  plotbendL5m->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL5m->Draw("COLZ");
  plotbendL5m->Write();
  c.SaveAs(type+"_dbendvsbend_L5m.png");

  plotbendL6m->GetXaxis()->SetTitle("Bend");
  plotbendL6m->GetYaxis()->SetTitle("#Delta Bend");
  plotbendL6m->Draw("COLZ");
  plotbendL6m->Write();
  c.SaveAs(type+"_dbendvsbend_L6m.png");

  tpptL1->GetXaxis()->SetTitle("p_{T} [GeV]");
  tpptL1->GetYaxis()->SetTitle("#Delta Bend");
  tpptL1->GetYaxis()->SetRangeUser(-0.8,0.8);
  tpptL1->Draw();
  tpptL1m->Draw("SAME");
  tpptL1m->SetMarkerColor(kRed);
  tpptL1->Write();
  c.SaveAs(type+"_tp_dbendvspt_L1.png");

  tpptL2->GetXaxis()->SetTitle("p_{T} [GeV]");
  tpptL2->GetYaxis()->SetTitle("#Delta Bend");
  tpptL2->GetYaxis()->SetRangeUser(-0.8,0.8);
  tpptL2->Draw();
  tpptL2m->Draw("SAME");
  tpptL2m->SetMarkerColor(kRed);
  tpptL2->Write();
  c.SaveAs(type+"_tp_dbendvspt_L2.png");

  tpptL3->GetXaxis()->SetTitle("p_{T} [GeV]");
  tpptL3->GetYaxis()->SetTitle("#Delta Bend");
  tpptL3->GetYaxis()->SetRangeUser(-0.8,0.8);
  tpptL3->Draw();
  tpptL3m->Draw("SAME");
  tpptL3m->SetMarkerColor(kRed);
  tpptL3->Write();
  c.SaveAs(type+"_tp_dbendvspt_L3.png");

  tpptL4->GetXaxis()->SetTitle("p_{T} [GeV]");
  tpptL4->GetYaxis()->SetTitle("#Delta Bend");
  tpptL4->GetYaxis()->SetRangeUser(-0.8,0.8);
  tpptL4->Draw();
  tpptL4m->Draw("SAME");
  tpptL4m->SetMarkerColor(kRed);
  tpptL4->Write();
  c.SaveAs(type+"_tp_dbendvspt_L4.png");

  tpptL5->GetXaxis()->SetTitle("p_{T} [GeV]");
  tpptL5->GetYaxis()->SetTitle("#Delta Bend");
  tpptL5->GetYaxis()->SetRangeUser(-0.8,0.8);
  tpptL5->Draw();
  tpptL5m->Draw("SAME");
  tpptL5m->SetMarkerColor(kRed);
  tpptL5->Write();
  c.SaveAs(type+"_tp_dbendvspt_L5.png");

  tpptL6->GetXaxis()->SetTitle("p_{T} [GeV]");
  tpptL6->GetYaxis()->SetTitle("#Delta Bend");
  tpptL6->GetYaxis()->SetRangeUser(-0.8,0.8);
  tpptL6->Draw();
  tpptL6m->Draw("SAME");
  tpptL6m->SetMarkerColor(kRed);
  tpptL6->Write();
  c.SaveAs(type+"_tp_dbendvspt_L6.png");

  tpetaL1->GetXaxis()->SetTitle("#eta");
  tpetaL1->GetYaxis()->SetTitle("#Delta Bend");
  tpetaL1->GetYaxis()->SetRangeUser(-0.5,0.5);
  tpetaL1->Draw();
  tpetaL1m->Draw("SAME");
  tpetaL1m->SetMarkerColor(kRed);
  tpetaL1->Write();
  c.SaveAs(type+"_tp_dbendvseta_L1.png");

  tpetaL2->GetXaxis()->SetTitle("#eta");
  tpetaL2->GetYaxis()->SetTitle("#Delta Bend");
  tpetaL2->GetYaxis()->SetRangeUser(-0.5,0.5);
  tpetaL2->Draw();
  tpetaL2m->Draw("SAME");
  tpetaL2m->SetMarkerColor(kRed);
  tpetaL2->Write();
  c.SaveAs(type+"_tp_dbendvseta_L2.png");

  tpetaL3->GetXaxis()->SetTitle("#eta");
  tpetaL3->GetYaxis()->SetTitle("#Delta Bend");
  tpetaL3->GetYaxis()->SetRangeUser(-0.5,0.5);
  tpetaL3->Draw();
  tpetaL3m->Draw("SAME");
  tpetaL3m->SetMarkerColor(kRed);
  tpetaL3->Write();
  c.SaveAs(type+"_tp_dbendvseta_L3.png");

  tpetaL4->GetXaxis()->SetTitle("#eta");
  tpetaL4->GetYaxis()->SetTitle("#Delta Bend");
  tpetaL4->GetYaxis()->SetRangeUser(-0.5,0.5);
  tpetaL4->Draw();
  tpetaL4m->Draw("SAME");
  tpetaL4m->SetMarkerColor(kRed);
  tpetaL4->Write();
  c.SaveAs(type+"_tp_dbendvseta_L4.png");

  tpetaL5->GetXaxis()->SetTitle("#eta");
  tpetaL5->GetYaxis()->SetTitle("#Delta Bend");
  tpetaL5->GetYaxis()->SetRangeUser(-0.5,0.5);
  tpetaL5->Draw();
  tpetaL5m->Draw("SAME");
  tpetaL5m->SetMarkerColor(kRed);
  tpetaL5->Write();
  c.SaveAs(type+"_tp_dbendvseta_L5.png");

  tpetaL6->GetXaxis()->SetTitle("#eta");
  tpetaL6->GetYaxis()->SetTitle("#Delta Bend");
  tpetaL6->GetYaxis()->SetRangeUser(-0.5,0.5);
  tpetaL6->Draw();
  tpetaL6m->Draw("SAME");
  tpetaL6m->SetMarkerColor(kRed);
  tpetaL6->Write();
  c.SaveAs(type+"_tp_dbendvseta_L6.png");

  tpbendL1->GetXaxis()->SetTitle("Bend");
  tpbendL1->GetYaxis()->SetTitle("#Delta Bend");
  tpbendL1->GetYaxis()->SetRangeUser(-10,10);
  tpbendL1->Draw();
  tpbendL1m->Draw("SAME");
  tpbendL1m->SetMarkerColor(kRed);
  tpbendL1->Write();
  c.SaveAs(type+"_tp_dbendvsbend_L1.png");

  tpbendL2->GetXaxis()->SetTitle("Bend");
  tpbendL2->GetYaxis()->SetTitle("#Delta Bend");
  tpbendL2->GetYaxis()->SetRangeUser(-10,10);
  tpbendL2->Draw();
  tpbendL2m->Draw("SAME");
  tpbendL2m->SetMarkerColor(kRed);
  tpbendL2->Write();
  c.SaveAs(type+"_tp_dbendvsbend_L2.png");

  tpbendL3->GetXaxis()->SetTitle("Bend");
  tpbendL3->GetYaxis()->SetTitle("#Delta Bend");
  tpbendL3->GetYaxis()->SetRangeUser(-10,10);
  tpbendL3->Draw();
  tpbendL3m->Draw("SAME");
  tpbendL3m->SetMarkerColor(kRed);
  tpbendL3->Write();
  c.SaveAs(type+"_tp_dbendvsbend_L3.png");

  tpbendL4->GetXaxis()->SetTitle("Bend");
  tpbendL4->GetYaxis()->SetTitle("#Delta Bend");
  tpbendL4->GetYaxis()->SetRangeUser(-10,10);
  tpbendL4->Draw();
  tpbendL4m->Draw("SAME");
  tpbendL4m->SetMarkerColor(kRed);
  tpbendL4->Write();
  c.SaveAs(type+"_tp_dbendvsbend_L4.png");

  tpbendL5->GetXaxis()->SetTitle("Bend");
  tpbendL5->GetYaxis()->SetTitle("#Delta Bend");
  tpbendL5->GetYaxis()->SetRangeUser(-10,10);
  tpbendL5->Draw();
  tpbendL5m->Draw("SAME");
  tpbendL5m->SetMarkerColor(kRed);
  tpbendL5->Write();
  c.SaveAs(type+"_tp_dbendvsbend_L5.png");

  tpbendL6->GetXaxis()->SetTitle("Bend");
  tpbendL6->GetYaxis()->SetTitle("#Delta Bend");
  tpbendL6->GetYaxis()->SetRangeUser(-10,10);
  tpbendL6->Draw();
  tpbendL6m->Draw("SAME");
  tpbendL6m->SetMarkerColor(kRed);
  tpbendL6->Write();
  c.SaveAs(type+"_tp_dbendvsbend_L6.png");
  
  tp2dL1->GetXaxis()->SetTitle("p_{T} [GeV]");
  tp2dL1->GetYaxis()->SetTitle("#eta");
  tp2dL1->GetZaxis()->SetRangeUser(-0.1,0.1);
  tp2dL1->Draw("COLZ");
  tp2dL1->Write();
  c.SaveAs(type+"_ptvsEtavsdBend_L1.png");

  tp2dL2->GetXaxis()->SetTitle("p_{T} [GeV]");
  tp2dL2->GetYaxis()->SetTitle("#eta");
  tp2dL2->GetZaxis()->SetRangeUser(-0.1,0.1);
  tp2dL2->Draw("COLZ");
  tp2dL2->Write();
  c.SaveAs(type+"_ptvsEtavsdBend_L2.png");

  tp2dL3->GetXaxis()->SetTitle("p_{T} [GeV]");
  tp2dL3->GetYaxis()->SetTitle("#eta");
  tp2dL3->GetZaxis()->SetRangeUser(-0.2,0.2);
  tp2dL3->Draw("COLZ");
  tp2dL3->Write();
  c.SaveAs(type+"_ptvsEtavsdBend_L3.png");

  tp2dL4->GetXaxis()->SetTitle("p_{T} [GeV]");
  tp2dL4->GetYaxis()->SetTitle("#eta");
  tp2dL4->GetZaxis()->SetRangeUser(-0.5,0.5);
  tp2dL4->Draw("COLZ");
  tp2dL4->Write();
  c.SaveAs(type+"_ptvsEtavsdBend_L4.png");

  tp2dL5->GetXaxis()->SetTitle("p_{T} [GeV]");
  tp2dL5->GetYaxis()->SetTitle("#eta");
  tp2dL5->GetZaxis()->SetRangeUser(-1.0,1.0);
  tp2dL5->Draw("COLZ");
  tp2dL5->Write();
  c.SaveAs(type+"_ptvsEtavsdBend_L5.png");

  tp2dL6->GetXaxis()->SetTitle("p_{T} [GeV]");
  tp2dL6->GetYaxis()->SetTitle("#eta");
  tp2dL6->GetZaxis()->SetRangeUser(-1.0,1.0);
  tp2dL6->Draw("COLZ");
  tp2dL6->Write();
  c.SaveAs(type+"_ptvsEtavsdBend_L6.png");

  fout->Close();

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
  gStyle->SetPadRightMargin(0.12);
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
  gStyle->SetOptStat(0);
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
