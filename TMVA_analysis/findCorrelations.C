#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include "TH2D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

using namespace std;

//int num_mass_regions = 12;
//int mass_regions[12] = {0, 2, 4, 6, 8, 10, 15, 20, 25, 30, 40, 50};

int num_mass_regions = 6;
double mass_regions[6] = {3.096-.03, 3.096, 3.096+.03, 9.85-.65, 9.85, 9.85+.65};

void plot_correlations(TH1D* hist[num_mass_regions-1], TString var_name="dummy", float yaxis_scale=1.3) {
	TCanvas *canvas = new TCanvas("canvas", "canvas");
        TLegend *legend = new TLegend(0.67,0.50,.87,0.87);
        canvas->cd();
        for (int i=0; i<num_mass_regions-1; i++) {
            hist[i]->SetTitle(var_name);
            hist[i]->GetXaxis()->SetTitle(var_name);
            hist[i]->GetYaxis()->SetTitle("fraction");
            hist[i]->Scale(1/hist[i]->Integral());
            hist[i]->SetLineColor(i+1);
            if (i==9) hist[i]->SetLineColor(45);
            hist[i]->SetStats(kFALSE);
            hist[i]->SetMaximum(hist[i]->GetMaximum()*yaxis_scale);
            hist[i]->Draw( "HIST SAME");
            legend->AddEntry(hist[i], Form("%d-%d GeV", mass_regions[i], mass_regions[i+1]) , "l");
        }
        legend->Draw();
        TString dir = TString("correlation_plots/");
	TString filetype = TString(".png");
        canvas->SaveAs(dir + var_name + filetype);
}

void findCorrelations(TString inputfilename = "scout_probe_full.root", TString outputfilename = "correlation_outputs.root") {

    TFile *inputfile = TFile::Open(inputfilename);
    TTree *jpsisig_tree = inputfile->Get<TTree>("jpsisig_tree");
    TTree *upsilon_tree = inputfile->Get<TTree>("upsilon_tree");
    TTree *sig_tree = inputfile->Get<TTree>("sig_tree");
    TTree *bckg_tree = inputfile->Get<TTree>("bckg_tree");

    TFile* outfile = new TFile(outputfilename, "RECREATE");

    //TTreeReader reader("bckg_tree", inputfile);
    TTreeReader reader("sig_tree", inputfile);    

    TH1D* chi2_hist[num_mass_regions-1];
    TH1D* nMuonHits_hist[num_mass_regions-1];
    TH1D* nPixelHits_hist[num_mass_regions-1];
    TH1D* vtxchi2_hist[num_mass_regions-1];
    TH1D* ntklayers_hist[num_mass_regions-1];
    TH1D* PVd_hist[num_mass_regions-1];
    TH1D* trkiso_hist[num_mass_regions-1];
    TH1D* IP_hist[num_mass_regions-1];
    TH1D* eta_hist[num_mass_regions-1];
    TH1D* phi_hist[num_mass_regions-1];
    TH1D* pt_hist[num_mass_regions-1];
    TH1D* dxy_hist[num_mass_regions-1];
    TH1D* dz_hist[num_mass_regions-1];
    TH1D* trkqoverp_hist[num_mass_regions-1];
    TH1D* trklambda_hist[num_mass_regions-1];
    TH1D* trkqoverperror_hist[num_mass_regions-1];
    TH1D* trklambdaerror_hist[num_mass_regions-1];

    for (int i=0; i<num_mass_regions-1; i++) {
	chi2_hist[i]        = new TH1D(Form("chi2_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("chi2_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
				100, 0, 10); 
	nMuonHits_hist[i]   = new TH1D(Form("nMuonHits_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("nMuonHits_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                50, 0, 50);
	nPixelHits_hist[i]  = new TH1D(Form("nPixelHits_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("nPixelHits_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                10, 0, 10);
	vtxchi2_hist[i]     = new TH1D(Form("vtxchi2_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("vtxchi2_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, 0, 10);
	ntklayers_hist[i]   = new TH1D(Form("ntklayers_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("ntklayers_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                25, 0, 25);
	PVd_hist[i]         = new TH1D(Form("PVd_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("PVd_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, 0, 0.2);
	trkiso_hist[i]      = new TH1D(Form("trkiso_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("trkiso_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, 0, 6);
	IP_hist[i]       = new TH1D(Form("IP_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("IP_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, 0, 60);
	pt_hist[i]       = new TH1D(Form("pt_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("pt_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, 0, 30);
	eta_hist[i]       = new TH1D(Form("eta_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("eta_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, -2.0, 2.0);
	phi_hist[i]       = new TH1D(Form("phi_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("phi_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, -4, 4);
	dxy_hist[i]       = new TH1D(Form("dxy_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("dxy_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, -.2, .2);
	dz_hist[i]       = new TH1D(Form("dz_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("dz_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, -20, 20);
	trklambda_hist[i]       = new TH1D(Form("trklambda_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("trklambda_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, -1.5, 1.5);
	trkqoverp_hist[i]       = new TH1D(Form("trkqoverp_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("trkqoverp_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, -.3, .3);
	trklambdaerror_hist[i]       = new TH1D(Form("trklambdaerror_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("trklambdaerror_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, 0, 0.02);
        trkqoverperror_hist[i]       = new TH1D(Form("trkqoverperror_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]), Form("trkqoverperror_hist_%d-%d_GeV", mass_regions[i], mass_regions[i+1]),
                                100, 0, 0.02);


    }

    TTreeReaderValue<float> mass (reader, "mass");
    TTreeReaderValue<float> chi2 (reader, "chi");
    TTreeReaderValue<int>   nMuonHits (reader, "nmhits");
    TTreeReaderValue<int>   nPixelHits (reader, "nphits");
    TTreeReaderValue<float> vtxchi2 (reader, "vtxchi2");
    TTreeReaderValue<int>   ntklayers (reader, "ntklayers");
    TTreeReaderValue<float> PVd (reader, "PVd");
    TTreeReaderValue<float> trkiso (reader, "trkiso");
    TTreeReaderValue<float> IP (reader, "IP");
    TTreeReaderValue<float> pt (reader, "pt");
    TTreeReaderValue<float> eta (reader , "eta");
    TTreeReaderValue<float> phi (reader, "phi");
    TTreeReaderValue<float> dxy (reader, "dxy");
    TTreeReaderValue<float> dz  (reader, "dz");
    TTreeReaderValue<float> trklambda (reader, "trklambda");
    TTreeReaderValue<float> trkqoverp (reader, "trkqoverp");
    TTreeReaderValue<float> trklambdaerror (reader, "trklambdaerror");
    TTreeReaderValue<float> trkqoverperror (reader, "trkqoverperror");

    int k = 0;
    while(reader.Next()) {
        if (k%1000000==0) cout << k << " events processed" << endl;
        k=k+1;
        //if (k==10000) break;
        //cout << *ntklayers << endl;

	for (int i=0; i<num_mass_regions-1; i++) {
	     if(*mass>mass_regions[i] && *mass<mass_regions[i+1]) {
		chi2_hist[i]->Fill(*chi2, 1);
		nMuonHits_hist[i]->Fill(*nMuonHits, 1);
		nPixelHits_hist[i]->Fill(*nPixelHits, 1);
		vtxchi2_hist[i]->Fill(*vtxchi2, 1);
		PVd_hist[i]->Fill(*PVd, 1);
		trkiso_hist[i]->Fill(*trkiso, 1);
		IP_hist[i]->Fill(*IP, 1);
		ntklayers_hist[i]->Fill(*ntklayers, 1);
		pt_hist[i]->Fill(*pt,1);
		eta_hist[i]->Fill(*eta, 1);
		phi_hist[i]->Fill(*phi, 1);
		dxy_hist[i]->Fill(*dxy, 1);
		dz_hist[i]->Fill(*dz, 1);
		trklambda_hist[i]->Fill(*trklambda, 1);
		trkqoverp_hist[i]->Fill(*trkqoverp, 1);
		trklambdaerror_hist[i]->Fill(*trklambdaerror, 1);
                trkqoverperror_hist[i]->Fill(*trkqoverperror, 1);
	     }
	}	

    }

    plot_correlations(chi2_hist, "chi2");
    plot_correlations(nMuonHits_hist, "nMuonHits");
    plot_correlations(nPixelHits_hist, "nPixelHits");
    plot_correlations(vtxchi2_hist, "vtxchi2");
    plot_correlations(ntklayers_hist, "ntklayers");
    plot_correlations(PVd_hist, "PVd", 2.0);
    plot_correlations(trkiso_hist, "trkiso");
    plot_correlations(IP_hist, "IP");
    plot_correlations(pt_hist, "pt");
    plot_correlations(eta_hist, "eta");
    plot_correlations(phi_hist, "phi");
    plot_correlations(dxy_hist, "dxy");
    plot_correlations(dz_hist, "dz");
    plot_correlations(trklambda_hist, "trklambda", 2);
    plot_correlations(trkqoverp_hist, "trkqoverp", 2);
    plot_correlations(trklambdaerror_hist, "trklambdaerror");
    plot_correlations(trkqoverperror_hist, "trkqoverperror");

    outfile->cd();
    for (int i=0; i<num_mass_regions-1; i++) {
	chi2_hist[i]->Write();
	nMuonHits_hist[i]->Write();
	nPixelHits_hist[i]->Write();
	vtxchi2_hist[i]->Write();
	PVd_hist[i]->Write();
	trkiso_hist[i]->Write();
	IP_hist[i]->Write();
    }
    outfile->Close();	

}
 
