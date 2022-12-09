#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include "TH2D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

using namespace std;

void findBS(TString inputfilename = "input.root", const char* outfilename = "./output.root", bool isMC=false) {

    TFile* outfile = new TFile(outfilename, "RECREATE");
    TTree* outtree = new TTree("tree", "tree");
    
    int num_mass_regions = 470;
    float m = 1;

    TH1F* h_x = new TH1F("vtxX","vtxX",4000,-2, 2.);
    TH1F* h_y = new TH1F("vtxY","vtxY",4000,-2, 2.);

    TFile *inputfile = TFile::Open(inputfilename);
    TTreeReader reader("tree", inputfile);

    TTreeReaderValue<float>        vtxX (reader, "vtxX");
    TTreeReaderValue<float>        vtxY (reader, "vtxY");

    int j=0;
    while(reader.Next()) {
	j++;
	if (j%10000==0){
	   cout << j << "events" <<endl;
	}

        h_x->Fill(*vtxX);
        h_y->Fill(*vtxY);
    }
    
    outfile->cd();
    h_x->Write();
    h_y->Write();
    outfile->Close();
}
