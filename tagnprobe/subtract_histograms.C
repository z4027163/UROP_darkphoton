#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include "TH2D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

using namespace std;

void subtract_histograms(TString folder1 = "", TString folder2 = "", TString cut = "MVALoose") {

    TFile* infile1 = new TFile("Efficiency/" + folder1 + "/TnP_MC_NUM_"+ cut +"_DEN_tightPVd_PAR_pt_dR.root", "READ");
    TFile* infile2 = new TFile("Efficiency/" + folder2 + "/TnP_MC_NUM_"+ cut +"_DEN_tightPVd_PAR_pt_dR.root", "READ");
    TFile* outfile = new TFile("systematics.root", "UPDATE");
 
    TCanvas *c1, *c2;
    if (cut == "MVALoose") {
        c1 = (TCanvas*)infile1->Get("tpTree/MvaLooseCutid_NUM_MVALoose_DEN_tightPVd_PAR_pt_dR/fit_eff_plots/dR_pt_PLOT_PVd_bin0");
        c2 = (TCanvas*)infile2->Get("tpTree/MvaLooseCutid_NUM_MVALoose_DEN_tightPVd_PAR_pt_dR/fit_eff_plots/dR_pt_PLOT_PVd_bin0");
    } else {
        c1 = (TCanvas*)infile1->Get("tpTree/ClassicalLooseCutid_NUM_ClassicalLoose_DEN_tightPVd_PAR_pt_dR/fit_eff_plots/dR_pt_PLOT_PVd_bin0");
        c2 = (TCanvas*)infile2->Get("tpTree/ClassicalLooseCutid_NUM_ClassicalLoose_DEN_tightPVd_PAR_pt_dR/fit_eff_plots/dR_pt_PLOT_PVd_bin0");
    }

    TH2F *h1 = (TH2F*)c1->GetPrimitive("dR_pt_PLOT_PVd_bin0");
    TH2F *h2 = (TH2F*)c2->GetPrimitive("dR_pt_PLOT_PVd_bin0"); 

    h1->SetDirectory(0);
    h2->SetDirectory(0);

//    h1->Divide(h2);
    TH2F *outhist = (TH2F*)h1->Clone();
    outhist->SetName(folder1 + "__vs__" + folder2);
    
    outhist->SetDirectory(0);

    //h1->Add(h2, -1);
    for(int x=1; x <= h1->GetNbinsX(); x+=1) {
        for(int y=1; y <= h1->GetNbinsY(); y+=1) {
            //cout << x << "," << y << " " << h1->GetBinContent(x,y)<< " " << h2->GetBinContent(x,y) << endl;
            
            outhist->SetBinContent(x,y,abs(h1->GetBinContent(x,y)/h2->GetBinContent(x,y)-1));
        }
    }

    //for(int x=1; x <= h1->GetNbinsX(); x+=1) {
    //    for(int y=1; y <= h1->GetNbinsY(); y+=1) {
    //        cout << x << "," << y << " " << outhist->GetBinContent(x,y)<< endl;
    //    }
   // }

    
    cout << "xnbin=" << h1->GetNbinsX() << " ynbins=" << h1->GetNbinsY() << endl;
    outhist->Write();
    outfile->Close();



} 
