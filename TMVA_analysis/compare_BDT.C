#include <iostream>
#include <vector>

#include "TPad.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TSystem.h"
#include "TImage.h"
#include "TKey.h"
#include "TH1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TObjArray.h"

void SetFrameStyle( TH1* frame, Float_t scale = 1.0 )
   {
      frame->SetLabelOffset( 0.012, "X" );// label offset on x axis
      frame->SetLabelOffset( 0.012, "Y" );// label offset on x axis
      frame->GetXaxis()->SetTitleOffset( 1.25 );
      frame->GetYaxis()->SetTitleOffset( 1.22 );
      frame->GetXaxis()->SetTitleSize( 0.045*scale );
      frame->GetYaxis()->SetTitleSize( 0.045*scale );
      Float_t labelSize = 0.04*scale;
      frame->GetXaxis()->SetLabelSize( labelSize );
      frame->GetYaxis()->SetLabelSize( labelSize );

      gPad->SetTicks();
      gPad->SetLeftMargin  ( 0.108*scale );
      gPad->SetRightMargin ( 0.050*scale );
      gPad->SetBottomMargin( 0.120*scale  );
   }

void SetTMVAStyle() {

   static Int_t c_Canvas         = TColor::GetColor( "#f0f0f0" );
   static Int_t c_FrameFill      = TColor::GetColor( "#fffffd" );
   static Int_t c_TitleBox       = TColor::GetColor( "#5D6B7D" );
   static Int_t c_TitleBorder    = TColor::GetColor( "#7D8B9D" );
   static Int_t c_TitleText      = TColor::GetColor( "#FFFFFF" );
   static Int_t c_SignalLine     = TColor::GetColor( "#0000ee" );
   static Int_t c_SignalFill     = TColor::GetColor( "#7d99d1" );
   static Int_t c_BackgroundLine = TColor::GetColor( "#ff0000" );
   static Int_t c_BackgroundFill = TColor::GetColor( "#ff0000" );
   static Int_t c_NovelBlue      = TColor::GetColor( "#2244a5" );

   static Bool_t UsePaperStyle = 0;

      TStyle *TMVAStyle;
      TMVAStyle = new TStyle(*gROOT->GetStyle("Plain")); // our style is based on Plain
      TMVAStyle->SetName("TMVA");
      TMVAStyle->SetTitle("TMVA style based on \"Plain\" with modifications defined in tmvaglob.C");
      gROOT->GetListOfStyles()->Add(TMVAStyle);
      gROOT->SetStyle("TMVA");

      TMVAStyle->SetLineStyleString( 5, "[52 12]" );
      TMVAStyle->SetLineStyleString( 6, "[22 12]" );
      TMVAStyle->SetLineStyleString( 7, "[22 10 7 10]" );

      TMVAStyle->SetPalette((UsePaperStyle ? 18 : 1),0);

      TMVAStyle->SetFrameBorderMode(0);
      TMVAStyle->SetCanvasBorderMode(0);
      TMVAStyle->SetPadBorderMode(0);
      TMVAStyle->SetPadColor(0);
      TMVAStyle->SetFillStyle(0);

      TMVAStyle->SetLegendBorderSize(0);

      TMVAStyle->SetTitleFillColor( c_TitleBox );
      TMVAStyle->SetTitleTextColor( c_TitleText );
      TMVAStyle->SetTitleBorderSize( 1 );
      TMVAStyle->SetLineColor( c_TitleBorder );
      if (!UsePaperStyle) {
         TMVAStyle->SetFrameFillColor( c_FrameFill );
         TMVAStyle->SetCanvasColor( c_Canvas );
      }

      TMVAStyle->SetPaperSize(20,26);
      TMVAStyle->SetPadTopMargin(0.10);
      TMVAStyle->SetPadRightMargin(0.05);
      TMVAStyle->SetPadBottomMargin(0.11);
      TMVAStyle->SetPadLeftMargin(0.12);

      TMVAStyle->SetMarkerStyle(21);
      TMVAStyle->SetMarkerSize(0.3);
      TMVAStyle->SetHistLineWidth(2);
      TMVAStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

      TMVAStyle->SetOptTitle(1);
      TMVAStyle->SetTitleH(0.052);

      TMVAStyle->SetOptStat(0);
      TMVAStyle->SetOptFit(0);

      TMVAStyle->SetPadTickX(1);
      TMVAStyle->SetPadTickY(1);

}

void compare_BDT()
{
   SetTMVAStyle();
   int type=2;
   // input:   - Input file (result from TMVA),
   //          - type = 1 --> plot efficiency(B) versus eff(S)
   //                 = 2 --> plot rejection (B) versus efficiency (S)
   //                 = 3 --> plot 1/eff(B) versus efficiency (S)

   Bool_t __PLOT_LOGO__  = kTRUE;
   Bool_t __SAVE_IMAGE__ = kTRUE;

   // the coordinates
   Float_t x1 = 0;
   Float_t x2 = 1;
   Float_t y1 = 0;
   Float_t y2 = 0.8;
    
   // reverse order if "rejection"
   if (type == 2) {
      Float_t z = y1;
      y1 = 1 - y2;
      y2 = 1 - z;
      //      cout << "--- type==2: plot background rejection versus signal efficiency" << endl;
   } else if (type == 3) {
      y1 = 0;
      y2 = -1; // will be set to the max found in the histograms

   } else {
      //  cout << "--- type==1: plot background efficiency versus signal efficiency" << endl;
   }
   // create canvas
   TCanvas* c = new TCanvas( "c", "the canvas", 200, 0, 650, 500 );

   // global style settings
   c->SetGrid();
   c->SetTicks();

   // legend
   Float_t x0L = 0.107,     y0H = 0.899;
   Float_t dxL = 0.457-x0L, dyH = 0.22;
   if (type == 2) {
      x0L = 0.15;
      y0H = 1 - y0H + dyH + 0.07;
   }

   TLegend *legend = new TLegend( x0L, y0H-dyH, x0L+dxL, y0H );
   //legend->SetTextSize( 0.05 );
   legend->SetHeader( "MVA Method:" );
   legend->SetMargin( 0.4 );

   TString xtit = "Signal efficiency";
   TString ytit = "Background efficiency";
   if (type == 2) ytit = "Background rejection";
   if (type == 3) ytit = "1/(Background eff.)";
   TString ftit = ytit + " versus " + xtit;

   TString hNameRef = "effBvsS";
   if (type == 2) hNameRef = "rejBvsS";
   if (type == 3) hNameRef = "invBeffvsSeff";



   // draw empty frame
   TH2F* frame = new TH2F( "frame", ftit, 500, x1, x2, 500, y1, y2 );
   frame->GetXaxis()->SetTitle( xtit );
   frame->GetYaxis()->SetTitle( ytit );
   SetFrameStyle( frame, 1.0 );

   frame->Draw();

   Int_t color = 1;
   Int_t nmva  = 0;

   // loop over all methods

//   TString dir[12]={"upsilon0p03","upsilon0p05","upsilon0p2","upsilon_nocut","upsilon0p03_PVd","upsilon0p05_PVd","upsilon0p2_PVd","upsilon_nocut_PVd","upsilon0p03_IP","upsilon0p05_IP","upsilon0p2_IP","upsilon_nocut_IP"};
//   TString dir[6]={"mc0p03_noIP","mc0p05_noIP","mc0p2_noIP","mc0p03_IP","mc0p05_IP","mc0p2_IP"}; 
//   TString dir[6]={"JPsi0p03_noIP","JPsi0p05_noIP","JPsi0p2_noIP","JPsi0p03_IP","JPsi0p05_IP","JPsi0p2_IP"};
   TString dir[7] = {"dataset_nontklayers", "dataset_nochi", "dataset_nonphits", "dataset_notrklambdaerror", "dataset_notrkqoverperror", "dataset_nodxy", "dataset_notrkiso"};
   for(int i=0; i<7; i++){
      TString color;
      //TFile *f = new TFile(dir[i]+"/TMVA.root");
      //TH1 *h = (TH1*)f->Get("Method_BDT/BDT/MVA_BDT_rejBvsS");
      TFile *f = new TFile("TMVA.root");
      cout <<  dir[i] + "/Method_BDT/BDT/MVA_BDT_rejBvsS" << endl;
      TH1 *h = (TH1*)f->Get(dir[i] + "/Method_BDT/BDT/MVA_BDT_rejBvsS"); 
      cout << h << endl;
      h->SetDirectory(0);
      h->SetLineWidth(3);
      if(i<3) h->SetLineColor(kRed+(i%3));
      else if(i<6) h->SetLineColor(kBlue+(i%3));
      else h->SetLineColor(kYellow+(i%4));
      h->Draw("csame");   
      legend->AddEntry(h,dir[i],"l");
   }
   // redraw axes
   frame->Draw("sameaxis");
   legend->Draw("same");


   c->Update();
   c->SaveAs("comparison_ROC.png");
}


