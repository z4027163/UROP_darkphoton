#include <iostream>
#include <TLegend.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "TH1D.h"
#include "TH2D.h"
#include <THStack.h>
#include "TProfile.h"
#include "TGraph.h"
#include "TFile.h"
#include "TTree.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFractionFitter.h"
#include <string>
#include <vector>
#include <math.h>
#include <TLatex.h>
#include <TLine.h>
#include <TMarker.h>
#include <TPave.h>
#include <TPaveStats.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <TString.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include "TF1.h"
#include "TEfficiency.h"

#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <valarray>

#include <RooAbsPdf.h> 
#include <RooPlot.h>
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooGaussian.h>
#include <RooCBShape.h>
#include <RooPolynomial.h>
#include <RooBernstein.h>
#include <RooRealVar.h> 
#include <RooFormulaVar.h> 
#include <RooWorkspace.h> 
#include <RooMsgService.h> 
#include <RooAddPdf.h> 
#include <TROOT.h> 

//#include "pdfs.h"
//#include <RooDoubleCB.h>
#include "../HiggsAnalysis/CombinedLimit/interface/RooMultiPdf.h"
//#include "/work/submit/juliush/UROP_darkphoton/CMSSW_12_4_3/src/HiggsAnalysis/CombinedLimit/interface/RooMultiPdf.h"
using namespace std;

void MultiMakeCardsAndWS(){

  //TString year[2] = {"2017","2018"};
  //for(int y = 0; y < 2; y++){ //year
  

	TFile* file=TFile::Open("../fitting/output.root");

	//LUMINOSITY
	double luminosity = 0; //4000.;//pb-1
	luminosity = 14307;
	//EFFICIENCY
	//get acceptance from hist
	//TFile* eff_file = TFile::Open("l1_corrCuts_eff_Data_newAllTrigLowMass_"+year[y]+"_mll_dR_wieghted.root");
	TFile* eff_file = TFile::Open("modifiedMllEff2022.root");
	//TFile* eff_file = TFile::Open("l1_corrCuts_eff_Data_newAllTrigLowMass_2018_mll_dR_wieghted.root");
	TEfficiency *teff;
        teff = ((TEfficiency*)eff_file->Get("honemll_clone"));

	//cout<<teff<<endl;
	teff->Draw();
	teff->Paint("");
	TGraphAsymmErrors* effgraph = teff->GetPaintedGraph();

	//EFFICIENCY SYSTEMATIC

	effgraph->SaveAs("output/effgraph.root");

	//scale
	double eps2scale = 1;//0.01;//1;//0.1;//0.002; //this scales eps 0.02 -> 

	//----->>>>> nSignal = xsecgraph->Eval(mass)*eff*luminosity*acceptances[i]

	//define unfittable ranges
	//float unfittable_mins[9] = {0,    0.28, 0.5,   0.7,   0.95, 2.75,  3.55,  8.9};

	double unfittable_regions[8][2] = {{0,0.22}, {0.53,0.575}, {0.74,0.85}, {0.97,1.12}, {2.8,3.85}, {9.0,11}};


	//ID EFFICIENCY

   //LOOP OVER MASS INDICES AND MAKE THE CARDS/WORKSPACES
	double mass = -1.;
	double rel_reso=0.013;//temporary
	//TFile* f_ws = TFile::Open(("../mass_calibration/pdfs"+(string)year[y]+".root").c_str(), "READ");
	TFile* f_ws = TFile::Open("../fitting/ws_signal_shape.root", "READ");
	RooWorkspace *w = (RooWorkspace*)f_ws->Get("dpworkspace");
	//w->loadSnapshot("calibrated");
	w->var("alphaL")->setConstant(true);//all this should actually be automatic. Check!!
	//w->var("alphaR")->setConstant(true);
	w->var("n")->setConstant(true);
	w->var("frac_gau")->setConstant(true);
	w->var("gau_reso_scale")->setConstant(true);

	w->Print();


        TGraph* effValues = new TGraph(190);

        //og for(int i=160; i<370; i++) 
	for(int i=1; i<400; i++){
	  	//get the histograms
                //if (i<177) continue;
		//if (i>368) continue;
		//if(i!=161) continue;
	        TH1D* catA;
	        TH1D* catB;
		  catA=(TH1D*)file->Get(Form("massforLimit_CatA%d",i));
		  catB=(TH1D*)file->Get(Form("massforLimit_CatB%d",i));
	  	catA->Add(catB);
	  	delete catB;

		//repeat for both the histograms w/ and w/o the MVA ID
	  	//get the histograms

	  	double massLow  =  catA->GetXaxis()->GetXmin();
		double massHigh =  catA->GetXaxis()->GetXmax();
		double massBinWidth = massHigh-massLow;
                int nbins = catA->GetNbinsX();	  

		//compute mass point and define ROOFit variables
	  	bool dontfit=false;

	  	//reduce the mass bin width to avoid being inside the forbidden regions (resonances)
	  	// for (auto fbin : unfittable_regions){
	  	// 	if ((massLow>fbin[0] && massHigh<fbin[1]) || (massLow<fbin[0] && massHigh>fbin[1])) dontfit=true; //the current bin is completely inside a forbidden region, or viceversa
	  	// 	else if ((massHigh-fbin[0])*(massHigh-fbin[1])<=0){ //high edge of our bin is in a forbidden region
	  	// 		massHigh = fbin[0];
	  	// 	}
	  	// 	else if ((massLow-fbin[0])*(massLow-fbin[1])<=0){ //low edge of our bin is in a forbidden region
	  	// 		massLow = fbin[1];
	  	// 	}
	  	// 	if ((massHigh-massLow)<0.2*massBinWidth) dontfit=true; //skip if the mass bin after this reduction is too small (or negative, which would mean the original mass bin was all inside a forbidden region)
	  	// }
	  	// if (dontfit) continue;

	  	mass = 0.5*(massLow+massHigh);
		if (mass < 1.0) continue;
		if (mass >= 40.) continue;
		if ((mass >= 2.6) && (mass <= 4.16)) continue;
		if ((mass >= 8.4) && (mass <= 11.2)) continue;
	  	for (auto fbin : unfittable_regions){
	  		if ((mass>fbin[0] && mass<fbin[1])) dontfit=true; //the current point is inside a forbidden region
	  		else if ((massHigh-fbin[0])*(massHigh-fbin[1])<=-0.1){ //high edge of our bin is in a forbidden region
			  //massHigh = fbin[0];
			  //massLow = massHigh-massBinWidth;
			  dontfit=true;
	  		}
	  		else if ((massLow-fbin[0])*(massLow-fbin[1])<=-0.1){ //low edge of our bin is in a forbidden region
			  //massLow = fbin[1];
			  //massHigh = massLow+massBinWidth;
			  dontfit=true;
	  		}
			if ((mass-massLow)<4*rel_reso*mass || (massHigh-mass)<4*rel_reso*mass) dontfit=true; //too close to the bin edge
	  	}
	  	if (dontfit) continue;

		double effcuts = 0.82;
		cout << "The ID efficiency is " << effcuts << " at mass " << mass ; 

                effValues->SetPoint(i, mass, effcuts);

		double teff=.7;
		if (mass > 20){
		   teff = .85;
		}
		else if (mass>10){
		   teff = (mass-10)*(.085) + (20-mass)*(.07);
		}

		//Calculate log normal uncertainty for trigger and selection efficiency
		double triggSysVal = 0.10;
		double triggSys = 1.00 + abs(triggSysVal); 
		cout << ".  The value of trigger syst is " <<  triggSys << "\n"; 


		double selSysVal = 0.06;
                double selSys = 1.00 + abs(selSysVal);
                cout << ".  The value of sel syst is " <<  selSys << "\n";

		RooRealVar* m2mu = w->var("m2mu");
		m2mu->setMax(massHigh);
		m2mu->setMin(massLow);

		RooAddPdf* signalModel = (RooAddPdf*)w->pdf("sig");

		//define the signal model
		w->var("mean")->setRange(mass-.1, mass+.1);
		w->var("mean")->setVal(mass);
		w->var("mean")->setConstant(true);
		w->var("res_rel")->setVal(rel_reso);
		w->var("res_rel")->setConstant(true);
                cout << "mass=" << mass <<endl;
                cout << "this is another test 1 mean=" << w->var("mean")->getVal() << endl;
                cout << "reso=" << w->var("res_rel")->getVal() << endl;
		//in pdf.h aggiungi una pdf generica e salvala nel workspace con tutti i param già fissati. poi riprendila da qui, e usa dirett
		// la sua variabile massa osservabile come massa qui, semplicemente cambiandogli il range.

		RooDataHist data_obs("data_obs", "", RooArgList(*m2mu), catA);
		RooRealVar bkg_norm("bkg_norm", "",catA->Integral());


                RooRealVar lar1_2022("lar1_2022", "lar1_2022", 0.0, -10.0, 10.0);
                RooRealVar lar2_2022("lar2_2022", "lar2_2022", 0.0, -10.0, 10.0);
                RooRealVar lar3_2022("lar3_2022", "lar3_2022", 0.0, -10.0, 10.0);
                RooRealVar lar4_2022("lar4_2022", "lar4_2022", 0.0, -10.0, 10.0);
                RooArgList llist_2022(lar1_2022, lar2_2022, lar3_2022, lar4_2022);
                RooPolynomial bkg_model_line4_2022("bkg_model_line_2022", "bkg_model_line_2022", *m2mu, llist_2022, 1);
                //Exponentials
                RooRealVar car1_2022("car1_2022", "car1_2022", -0.5, -10, 10);
                RooExponential bkg_model_exp4_2022("bkg_model_exp4_2022", "bkg_model_exp4_2022", *m2mu, car1_2022);
                //Product of the two
                RooProdPdf bkg_model_pol4xexp_2022("bkg_model_pol4xexp_2022", "bkg_model_pol4xexp_2022", bkg_model_line4_2022, bkg_model_exp4_2022);
                bkg_model_pol4xexp_2022.chi2FitTo(data_obs);


		RooRealVar par1_2022("par1_2022", "par1_2022", 0.2, 0, 10);
		RooRealVar par2_2022("par2_2022", "par2_2022", 1.5, 0, 10);
		RooRealVar par3_2022("par3_2022", "par3_2022", 2.0, 0, 10);
		RooRealVar par4_2022("par4_2022", "par4_2022", 2.0, 0, 10);
		RooArgList alist_2022(par1_2022, par2_2022, par3_2022, par4_2022);
		RooBernstein bkg_model_bern4_2022("bkg_model_bern4_2022", "bkg_model_bern4_2022", *m2mu, alist_2022);
		bkg_model_bern4_2022.fitTo(data_obs);
		
		
		RooRealVar bar1_2022("bar1_2022", "bar1_2022", -0.5, -10, 10);                            
		RooRealVar bf1_2022("bf1_2022","bf1_2022",0.2,0.0,1.0);   
		RooExponential exp1_2022("exp1_2022", "exp1_2022", *m2mu, bar1_2022);
		RooRealVar bar2_2022("bar2_2022", "bar2_2022", -0.5, -10, 10);                                                                                           
		RooRealVar bf2_2022("bf2_2022","bf2_2022",0.2,0.0,1.0);                                                                                                  
		RooExponential exp2_2022("exp2_2022", "exp2_2022", *m2mu, bar2_2022);
		RooRealVar bar3_2022("bar3_2022", "bar3_2022", -0.5, -10, 10);
		RooRealVar bf3_2022("bf3_2022","bf3_2022",0.2,0.0,1.0);
		RooExponential exp3_2022("exp3_2022", "exp3_2022", *m2mu, bar3_2022);
		RooRealVar bar4_2022("bar4_2022", "bar4_2022", -0.5, -10, 10);
		RooRealVar bf4_2022("bf4_2022","bf4_2022",0.2,0.0,1.0);
		RooExponential exp4_2022("exp4_2022", "exp4_2022", *m2mu, bar4_2022);
		RooRealVar bar5_2022("bar5_2022", "bar5_2022", -0.5, -10, 10);
		RooRealVar bf5_2022("bf5_2022","bf5_2022",0.2,0.0,1.0);
		RooExponential exp5_2022("exp5_2022", "exp5_2022", *m2mu, bar5_2022);
                RooRealVar bar6_2022("bar6_2022", "bar6_2022", -0.5, -10, 10);
		RooRealVar bf6_2022("bf6_2022","bf6_2022",0.2,0.0,1.0);
                RooExponential exp6_2022("exp6_2022", "exp6_2022", *m2mu, bar6_2022);
                RooRealVar bar7_2022("bar7_2022", "bar7_2022", -0.5, -10, 10);
                RooExponential exp7_2022("exp7_2022", "exp7_2022", *m2mu, bar7_2022);



		RooArgList explist_2022(exp1_2022,exp2_2022,exp3_2022,exp4_2022,exp5_2022,exp6_2022);
		RooArgList expclist_2022(bf1_2022,bf2_2022,bf3_2022,bf4_2022,bf5_2022);
		RooAddPdf bkg_model_exp7_2022("bkg_model_exp7_2022","bkg_model_exp7_2022",explist_2022,expclist_2022,true);
		bkg_model_exp7_2022.fitTo(data_obs);



                RooRealVar pow_1_2022("pow_1_2022","exponent of power law",0,-10,10);
                RooRealVar pf1_2022("pf1_2022","frac of power law",0.2,0.0,1.0);
                RooGenericPdf plaw1_2022("plaw1_2022","TMath::Power(@0,@1)",RooArgList(*m2mu,pow_1_2022));
                RooRealVar qar1_2022("qar1_2022", "qar1_2022", 0.2, 0, 10);
                RooRealVar qar2_2022("qar2_2022", "qar2_2022", 1.5, 0, 10);
                RooRealVar qar3_2022("qar3_2022", "qar3_2022", 2.0, 0, 10);
                RooArgList qlist_2022(qar1_2022, qar2_2022, qar3_2022);
                RooRealVar bfp1_2022("bfp1_2022","frac of bernstein",0.2,0.0,1.0);
                RooBernstein bern3_2022("bkg_model_bern3_2022", "bkg_model_bern3_2022", *m2mu, qlist_2022);
                RooArgList plawlist1_2022(plaw1_2022, bern3_2022);
                RooArgList plawclist1_2022(pf1_2022, bfp1_2022);
                RooAddPdf bkg_model_bern3p1_2022("bkg_model_bern3p1_2022","bkg_model_bern3p1_2022",plawlist1_2022,plawclist1_2022,true);
                bkg_model_bern3p1_2022.fitTo(data_obs);




                RooCategory  pdf_index_2022("pdf_index_2022","Index of the background PDF which is active");
                RooArgList bkg_pdf_list_2022;		
                bkg_pdf_list_2022.add(bkg_model_bern4_2022);

                bkg_pdf_list_2022.add(bkg_model_pol4xexp_2022);
                bkg_pdf_list_2022.add(bkg_model_exp7_2022);
                bkg_pdf_list_2022.add(bkg_model_bern3p1_2022);

                RooMultiPdf bkg_model_2022("bkg_model_2022", "All Pdfs", pdf_index_2022, bkg_pdf_list_2022);	       
                bkg_model_2022.setCorrectionFactor(0.5);


		//save into ROO workspace
		RooWorkspace dpworkspace("dpworkspace", "");
		dpworkspace.import(data_obs);
		dpworkspace.import(*signalModel);

                cout << "this is a test mean=" << dpworkspace.var("mean")->getVal() << endl;
		  dpworkspace.import(bkg_model_2022); 
		dpworkspace.writeToFile(Form("output/dpWorkspace2022_%d.root",i));

		//write the datacard
		char inputShape[200];
		sprintf(inputShape,"output/dpCard_2022_m%.3f_%d.txt",mass,i);
		ofstream newcardShape;
		newcardShape.open(inputShape);
		newcardShape << Form("imax * number of channels\n");
		newcardShape << Form("jmax * number of background\n");
		newcardShape << Form("kmax * number of nuisance parameters\n");
		newcardShape << Form("shapes data_obs	CatAB output/dpWorkspace2022_%d.root dpworkspace:data_obs\n",i);
		newcardShape << Form("shapes bkg_mass	CatAB output/dpWorkspace2022_%d.root dpworkspace:bkg_model_2022\n",i);
		newcardShape << Form("shapes signalModel_generic	CatAB output/dpWorkspace2022_%d.root dpworkspace:sig\n",i);
		newcardShape << Form("bin		CatAB\n");
		newcardShape << Form("observation 	-1.0\n");
		newcardShape << Form("bin     		CatAB		CatAB		\n");
		newcardShape << Form("process 		signalModel_generic  	bkg_mass	\n");
		newcardShape << Form("process 		0    		1	   	\n");
		newcardShape << Form("rate    		%f  		%f		\n",
				     effcuts*teff*luminosity, catA->Integral());
		newcardShape << Form("lumi13TeV_2022 lnN 	1.026 	-\n");
		newcardShape << Form("id_eff_mva_2022 lnN	%f 	-\n", selSys);
		newcardShape << Form("eff_trig_2022 lnN         %f        -\n", triggSys);
                double eff_cut_unc=1.05; // ID eff entanglement
                if(i<290) eff_cut_unc=1.08;
                newcardShape << Form("eff_cut lnN         %f        -\n",eff_cut_unc );
                newcardShape << Form("pdf_index_2022 discrete \n");
		  newcardShape << Form("bkg_norm_2022 rateParam CatAB bkg_mass 1.0\n");
                newcardShape << Form("res_rel_generic param 0.013  0.0015\n");
		newcardShape << Form("");

		newcardShape.close();

	}
	f_ws->Close();

	
	TCanvas c_fVal("c_fVal", "c_fVal", 950, 1020);
	effValues->GetXaxis()->SetRangeUser(0.8, 9);
	effValues->GetXaxis()->SetTitle("m_{#mu#mu}");
	effValues->SetTitle("Efficiency (Muon MVA + PVd Cut)");
        effValues->Draw("a*");
	TMarker *point = new TMarker(1,0.73,8);
	point->SetMarkerColor(4);
	point->Draw("s");
	TLine *l = new TLine(2,0,2,1);
	//TLine *l = new TLine(2,0,2,c_fVal.GetY1());
	l->SetLineColor(2);
	l->Draw("s");
	auto legend = new TLegend(0.6,0.1,0.9,0.4);
	legend->AddEntry(effValues,"ID Efficiency","p");
	legend->AddEntry(point,"Scalar sample point","p");
	legend->AddEntry(l,"Extrapolation threshold","l");
	
	legend->Draw();
	//c_fVal.SetLogy();
        c_fVal.SaveAs("ID_EffBareDistribution.png");
	
	//for (int j=1; j<=nbins_tsys; j++){
	//  double val = 1.00 + abs(tsys->GetBinContent(j));
	//  cout <<  "Tris sys " << val << "\n"; 
	//}
	
}
