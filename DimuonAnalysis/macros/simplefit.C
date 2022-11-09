#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooCrystalBall.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"
//#include <string>
//#include <format>
using namespace RooFit;
 
void simplefit(){

   bool save_ntuple=true;
   bool make_plot=true;

   TString hname="massforLimitFull";
   TFile *file = TFile::Open("output.root"); 

   double masses[] = {1.019462, 3.096900, 3.68610, 9.46030, 10.02326, 10.3552};
   double masses_err[] = {.000061, .000006, 0.00006, .00026, .00031, .0005};
   double xmin[]={.919, 2.996, 3.586};
   double xmax[]={1.119, 3.196, 3.786};
   double x_init[]={1.019, 3.096, 3.686};
   double sigmin[]={0.01, 0.01, 0.01};
   double sigmax[]={0.05, 0.05, 0.05};
   double sig_init[]={0.012, 0.012, 0.012};
   int    hist_labels[] = {1, 113, 130};
   char   labels[][40] = {"phi_meson_peak.png", "j_psi_meson_peak.png", "psi_prime_meson_peak.png"};
   
   double res[] = {0,0,0,0,0,0};
   double res_errors[] = {0,0,0,0,0,0};
   double means[] = {0,0,0,0,0,0}; 
   double mean_errors[] = {0,0,0,0,0,0};

   RooRealVar alphaR("alphaR", "alphaR", 1, 0, 2.5);
   RooRealVar alphaL("alphaL", "alphaL", 1, 0, 2.5);
   RooRealVar n("n", "n", 1, 0, 2.5);
   RooRealVar frac_gau("frac_gau", "frac_gau", 0.5, 0, 1);
   RooRealVar gau_reso_scale("gau_reso_scale", "gau_reso_scale", 0.5, 0.2, 1.5);

   int idx=0;
   int idx_arr[] = {1,0,2};
   for (int i=0; i<3; i+=1) {
           idx = idx_arr[i];	   

	   //string hh_name = std::format("massforLimit_CatA{}", hist_labels[idx]);
           //std::string hb_name = std::format("massforLimit_CatB{}", hist_labels[idx]);
	   TH1F *hh = (TH1F*)file->Get(Form("massforLimit_CatA%d",hist_labels[idx]));
  	   TH1F *hB = (TH1F*)file->Get(Form("massforLimit_CatB%d",hist_labels[idx]));
  	   hh->Add(hB);
  	   hh->Rebin(4);

	   double x_min=hh->GetXaxis()->GetXmin(); 
	   double x_max=hh->GetXaxis()->GetXmax();
	   cout << "x range: " << x_min << " - " << x_max << endl;
	   RooRealVar x("m2mu", "x",x_min, x_max);
	   RooDataHist dh("dh","dh",x,Import(*hh));
	
	   //////////////////////////////
	   //Adding new variables here//
	   /////////////////////////////
	   RooRealVar mean("mean", "mean", x_init[idx], xmin[idx], xmax[idx]);
	   RooRealVar res_rel("res_rel", "res_rel", 0.01, 0.005, 0.1);
	   RooRealVar bck_frac("background_fraction", "bck_frac", .01, 0., 1.);

	   RooFormulaVar res_CB("res_CB", "mean*res_rel", RooArgList(mean, res_rel));
	   RooFormulaVar res_gau("res_gau", "gau_reso_scale*mean*res_rel", RooArgList(gau_reso_scale, mean, res_rel));
           RooCrystalBall sig_CB("sig_CB", "sig_CB", x, mean, res_CB, alphaL, n, alphaR, n);
           //RooDoubleCB sig_CB("sig_CB", "sig_CB", x, mean, res_CB, alphaL, n, alphaR, n);
	   RooGaussian sig_gau("sig_gau", "sig_gau", x, mean, res_gau);   

	   RooRealVar exp_const("exp_const", "exp_const", .1, 0., 10.);
	   RooExponential bckg("bckg", "bckg", x, exp_const);

	   RooAddPdf sig("sig", "sig", RooArgList(sig_CB, sig_gau), frac_gau);
           RooAddPdf sig_bckg("sig_bckg", "sig_and_bckg", RooArgList(sig, bckg), bck_frac);

           RooPlot *frame = x.frame(Title("peak fit"));
           dh.plotOn(frame); 
           sig_bckg.fitTo(dh);
           sig_bckg.plotOn(frame, Components(sig_CB), LineStyle(kDashed), LineColor(kRed));
           sig_bckg.plotOn(frame, Components(sig_gau), LineStyle(kDashed), LineColor(kMagenta));
           sig_bckg.plotOn(frame, Components(bckg), LineStyle(kDashed), LineColor(kGreen));
           sig_bckg.plotOn(frame, LineColor(kBlue));

	   means[idx] = mean.getValV();
	   mean_errors[idx] = mean.getError();
	   res[idx] = res_rel.getValV();
	   res_errors[idx] = res_rel.getError();

	   if (i==0) {
		alphaL.setConstant(kTRUE);
                alphaR.setConstant(kTRUE);
		n.setConstant(kTRUE);
                frac_gau.setConstant(kTRUE);
                gau_reso_scale.setConstant(kTRUE);
	   }
           //
/*
           RooRealVar width("dkkwidth","width",0.5,0,3);
           RooRealVar exp_const("exp_const", "exp_const", .1, 0., 10.);
	   //RooRealVar mean("dkkmean", "mean", x_init[idx], xmin[idx], xmax[idx]);
           RooConstVar mean("dkkmean", "mean", x_init[idx]);
           RooRealVar sigma("sigma_gauss", "sigma", sig_init[idx], sigmin[idx], sigmax[idx]);
	   RooRealVar sigmaR("sigmaR", "sigmaR", sig_init[idx], sigmin[idx], sigmax[idx]);
   	   RooRealVar sigmaL("sigmaL", "sigmaL", sig_init[idx], sigmin[idx], sigmax[idx]);
           RooRealVar alphaR("alphaR", "alphaR", 5, 0, 100);
           RooRealVar alphaL("alphaL", "alphaL", 5, 0, 100);
	   RooRealVar nR("nR", "nR", 10, 1, 1000);
	   RooRealVar nL("nL", "nL", 10, 1, 1000);

	   RooRealVar sig_frac("signal_fraction", "sig_frac", .01, 0., 1.);
	   RooRealVar bck_frac("background_fraction", "bck_frac", .01, 0., 1.);

	   RooGaussian gauss("gauss", "gauss", x, mean, sigma);
	   RooVoigtian voig("voig","voig",x,mean,width,sigma);
	   RooCrystalBall doubleCB("doubleCB", "doubleCB", x, mean, sigmaL, sigmaR, alphaL, nL, alphaR, nR);	   
	   RooExponential bckg("bckg", "bckg", x, exp_const);

	   RooAddPdf sig("sig", "sig", RooArgList(doubleCB, gauss), sig_frac);
	   RooAddPdf sig_bckg("sig_bckg", "sig_and_bckg", RooArgList(sig, bckg), bck_frac);
 
	   RooPlot *frame = x.frame(Title("peak fit"));
	   dh.plotOn(frame); 
	   sig_bckg.fitTo(dh);
	   //sig.plotOn(frame, LineColor(kRed));
	   //bckg.plotOn(frame, LineColor(kGreen));
	   sig_bckg.plotOn(frame, Components(doubleCB), LineStyle(kDashed), LineColor(kRed));
	   sig_bckg.plotOn(frame, Components(gauss), LineStyle(kDashed), LineColor(kMagenta));
	   sig_bckg.plotOn(frame, Components(bckg), LineStyle(kDashed), LineColor(kGreen));
           sig_bckg.plotOn(frame, LineColor(kBlue));
           //frame->BuildLegend();
*/	   
	   double norm=hh->Integral(1,-1);
	   if(save_ntuple){
	      RooWorkspace dpworkspace("dpworkspace", "");      
	      dpworkspace.import(sig);
	      dpworkspace.writeToFile("dkk_ws_signal_shape.root"); 
	      save_ntuple = false; 
	   }

	   if(make_plot){  
	     cout <<"tot=" << hh->Integral(1,-1) <<  "norm=" << norm << endl;
	     TCanvas *c = new TCanvas("real peak", "real peak", 1000, 800);
	     c->cd();
	     frame->GetYaxis()->SetTitleOffset(1.4);
	     frame->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
	     frame->Draw();
	     c->SaveAs(labels[idx]);
	     //c->SaveAs("test.png");
	   }
   }

   //Upsilon fitting (hist 229)
   //
   TH1F *hh = (TH1F*)file->Get(Form("massforLimit_CatA%d",229));
   TH1F *hB = (TH1F*)file->Get(Form("massforLimit_CatB%d",229));
   hh->Add(hB);
   hh->Rebin(4);

   double x_min=hh->GetXaxis()->GetXmin();
   double x_max=hh->GetXaxis()->GetXmax();
   cout << "x range: " << x_min << " - " << x_max << endl;
   RooRealVar x("m2mu", "x",x_min, x_max);
   RooDataHist dh("dh","dh",x,Import(*hh));

   RooRealVar mean1("mean1", "mean1", 9.460, 9.360, 9.560);
   RooRealVar res_rel1("res_rel1", "res_rel1", 0.01, 0.005, 0.1);
   RooFormulaVar res_CB1("res_CB1", "mean1*res_rel1", RooArgList(mean1, res_rel1));
   RooFormulaVar res_gau1("res_gau1", "gau_reso_scale*mean1*res_rel1", RooArgList(gau_reso_scale, mean1, res_rel1));
   RooCrystalBall sig_CB1("sig_CB1", "sig_CB1", x, mean1, res_CB1, alphaL, n, alphaR, n);
   RooGaussian sig_gau1("sig_gau1", "sig_gau1", x, mean1, res_gau1);

   RooRealVar mean2("mean2", "mean2", 10.023, 9.923, 10.123);
   RooRealVar res_rel2("res_rel2", "res_rel2", 0.01, 0.005, 0.1);
   RooFormulaVar res_CB2("res_CB2", "mean2*res_rel2", RooArgList(mean2, res_rel2));
   RooFormulaVar res_gau2("res_gau2", "gau_reso_scale*mean2*res_rel2", RooArgList(gau_reso_scale, mean2, res_rel2));
   RooCrystalBall sig_CB2("sig_CB2", "sig_CB2", x, mean2, res_CB2, alphaL, n, alphaR, n);
   RooGaussian sig_gau2("sig_gau2", "sig_gau2", x, mean2, res_gau2);

   RooRealVar mean3("mean3", "mean3", 10.355, 10.255, 10.455);
   RooRealVar res_rel3("res_rel3", "res_rel3", 0.01, 0.005, 0.1);
   RooFormulaVar res_CB3("res_CB3", "mean3*res_rel3", RooArgList(mean3, res_rel3));
   RooFormulaVar res_gau3("res_gau3", "gau_reso_scale*mean3*res_rel3", RooArgList(gau_reso_scale, mean3, res_rel3));
   RooCrystalBall sig_CB3("sig_CB3", "sig_CB3", x, mean3, res_CB3, alphaL, n, alphaR, n);
   RooGaussian sig_gau3("sig_gau3", "sig_gau3", x, mean3, res_gau3);

   RooRealVar exp_const("exp_const", "exp_const", .1, 0., 10.);
   RooExponential bckg("bckg", "bckg", x, exp_const);
   
   RooRealVar bck_frac("background_fraction", "bck_frac", .01, 0., 1.);
   RooRealVar peak_ratio1("peak_ratio1", "peak_ratio1", .5, .1, .9);
   RooRealVar peak_ratio2("peak_ratio2", "peak_ratio2", .5, .1, .9);
   RooAddPdf sig1("sig1", "sig1", RooArgList(sig_CB1, sig_gau1), frac_gau);
   RooAddPdf sig2("sig2", "sig2", RooArgList(sig_CB2, sig_gau2), frac_gau);
   RooAddPdf sig3("sig3", "sig3", RooArgList(sig_CB3, sig_gau3), frac_gau);
   RooAddPdf sig("sig", "sig", RooArgList(sig1, sig2, sig3), RooArgList(peak_ratio1, peak_ratio2));
   RooAddPdf sig_bckg("sig_bckg", "sig_and_bckg", RooArgList(sig, bckg), bck_frac);
   
   RooPlot *frame = x.frame(Title("peak fit"));
   dh.plotOn(frame);
   sig_bckg.fitTo(dh);
   sig_bckg.plotOn(frame, Components(sig1), LineStyle(kDashed), LineColor(kRed));
   sig_bckg.plotOn(frame, Components(sig2), LineStyle(kDashed), LineColor(kMagenta));
   sig_bckg.plotOn(frame, Components(sig3), LineStyle(kDashed), LineColor(kPink));
   sig_bckg.plotOn(frame, Components(bckg), LineStyle(kDashed), LineColor(kGreen));
   sig_bckg.plotOn(frame, LineColor(kBlue));

   TCanvas *c = new TCanvas("real peak", "real peak", 1000, 800);
   c->cd();
   frame->GetYaxis()->SetTitleOffset(1.4);
   frame->GetXaxis()->SetTitle("m_{#mu#mu} (GeV)");
   frame->Draw();
   c->SaveAs("upsilon_meson_peak.png");

   means[3] = mean1.getValV();
   mean_errors[3] = mean1.getError();
   res[3] = res_rel1.getValV();
   res_errors[3] = res_rel1.getError(); 
   means[4] = mean2.getValV();
   mean_errors[4] = mean2.getError();
   res[4] = res_rel2.getValV();
   res_errors[4] = res_rel2.getError(); 
   means[5]= mean3.getValV();
   mean_errors[5] = mean3.getError();
   res[5] = res_rel3.getValV();
   res_errors[5] = res_rel3.getError();  

   double mass_diff[] = {0, 0, 0, 0, 0, 0};
   for(int i=0; i<6; i+=1) {
       //ut << labels[i] << endl;
       cout << "mean: " << means[i] << endl; 
       cout << "mean error: " << mean_errors[i] << endl;
       cout << "relative resolution: " << res[i] << endl;
       cout << "relative resolution error: " << res_errors[i] << endl;
       mass_diff[i] = means[i] - masses[i];
       cout << endl;
   }

   TCanvas *c1 = new TCanvas("resolution plot", "resolution plot", 1000, 800);
   c1->cd();
   TGraph *res_graph = new TGraphErrors(6, masses, res, masses_err, res_errors);
   res_graph->SetTitle("Dimuon Resolution Plot;m_{#mu#mu} (GeV); Resolution (GeV)");
   res_graph->SetMarkerColor(4);
   res_graph->SetMarkerStyle(21);
   res_graph->GetYaxis()->SetRangeUser(0,0.03);
   res_graph->Draw();
   c1->SaveAs("resolution_plot.png");

   TCanvas *c2 = new TCanvas("response plot", "response plot", 1000, 800);
   c2->cd();
   TGraph *dif_graph = new TGraph(6, masses, mass_diff);   
   dif_graph->SetTitle("Dimuon Response Plot;m_{#mu#mu} (GeV); Response (GeV)");
   dif_graph->SetMarkerColor(4);
   dif_graph->SetMarkerStyle(21);
   //TAxis *diff_axis = dif_graph->GetYaxis();
   //diff_axis->SetLimits(-.02, .02);
   dif_graph->GetYaxis()->SetRangeUser(-.04,.04);
   dif_graph->Draw();
   c2->SaveAs("response_plot.png");
}
