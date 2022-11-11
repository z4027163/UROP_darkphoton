#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include "TH2D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

using namespace std;

void trimscout(TString inputfilename = "intermediate.root", const char* outfilename = "./output_histogram.root", bool isMC=false) {

    TFile* outfile = new TFile(outfilename, "RECREATE");
    TTree* outtree = new TTree("tree", "tree");
    
    int num_mass_regions = 470;
    float m = 1;

    TH1F* massforLimitFull = new TH1F("massforLimitFull","massforLimitFull",4000,0., 120.);
    TH1D* massforLimit_CatA[num_mass_regions];
    TH1D* massforLimit_CatB[num_mass_regions];

    for(int j=0; j<num_mass_regions; j++){
      m = m+(m*0.01);
      massforLimit_CatA[j] = new TH1D(Form("massforLimit_CatA%d",j),Form("massforLimit_CatA%d",j),100,m-(m*0.013*5.),m+(m*0.013*5.));  massforLimit_CatA[j]->Sumw2();
      massforLimit_CatB[j] = new TH1D(Form("massforLimit_CatB%d",j),Form("massforLimit_CatB%d",j),100,m-(m*0.013*5.),m+(m*0.013*5.));  massforLimit_CatB[j]->Sumw2();
    }

    TFile *inputfile = TFile::Open(inputfilename);
    TH1F *htotal = (TH1F*)inputfile->Get("htotal");    

    TTreeReader reader("tree", inputfile);

    TTreeReaderArray<float>          mupt  (reader, "pt"    );
    TTreeReaderArray<float>          mueta  (reader, "eta"    );
    TTreeReaderArray<float>          muphi  (reader, "phi"    );
    TTreeReaderArray<float>          muchg (reader, "charge"    );
    TTreeReaderArray<float>          RelTrackIso  (reader, "trkiso"    );
    TTreeReaderArray<float>          muTrkChi2  (reader, "chi"    );
    TTreeReaderArray<int>          nPixelHits (reader, "nphits"     );
    TTreeReaderArray<int>          nTrkLayers (reader, "ntklayers"      );
    TTreeReaderValue<int>          nmuon  (reader, "nmuon"    );

    int j=0;
    int count[4]={0};
    while(reader.Next()) {
        count[0]++;
	j++;
	if (j%10000==0){
	   cout << j << "events" <<endl;
	}

        if(*nmuon<2) continue;
        count[1]++;

        std::vector<unsigned> goodmuons;
	//cout << *nmuon << endl;
        for (int i = 0; i < *nmuon; i++) {
            if(mupt[i]<4 || abs(mueta[i])>1.9) continue;
	    //cout << mupt[i] << endl;
            if(nPixelHits[i]==0) continue;
            if(nTrkLayers[i]<=5) continue;
            if(muTrkChi2[i]>=10) continue;
            if(RelTrackIso[i]>=.15) continue;
            goodmuons.push_back(i);
        }
        
        if (goodmuons.size() < 2) continue;
        count[2]++;

        unsigned idx1 = goodmuons[0];
        unsigned idx2 = goodmuons[1];
        if (mupt[idx1] < mupt[idx2]) {
            idx1 = goodmuons[1];
            idx2 = goodmuons[0];
        }

        TLorentzVector mm;
        TLorentzVector m1;
        TLorentzVector m2;

        m1.SetPtEtaPhiM(mupt[idx1], mueta[idx1], muphi[idx1], 0.1057);
        m2.SetPtEtaPhiM(mupt[idx2], mueta[idx2], muphi[idx2], 0.1057);
        mm =  m1+m2;


        float mass,m1pt,m2pt,m1eta,m2eta,m1phi,m2phi,m1id,m2id,m1ch,m2ch,m1iso,m2iso;
        double weight, ptll;

        m1pt   = m1.Pt();
        m1eta  = m1.Eta();
        m1phi  = m1.Phi();
        m1iso  = RelTrackIso[idx1];
        m1ch   = muchg[idx1];

        m2pt   = m2.Pt();
        m2eta  = m2.Eta();
        m2phi  = m2.Phi();
        m2iso  = RelTrackIso[idx2];
        m2ch   = muchg[idx2];

        weight = 1;
        mass   = mm.M();
        ptll   = mm.Pt();

        if (m1ch*m2ch>0) continue;
        count[3]++;

        massforLimitFull->Fill(mass, weight);

	float maxEta=TMath::Max(abs(m1eta),abs(m2eta));
	
	float ma=1;
        for(int j=0; j<num_mass_regions; j++){
            ma = ma+(ma*0.01);
            if(mass > ma-(ma*0.013*5.) && mass < ma+(ma*0.013*5.)) {
		//cout << "filling catAB "<< j<< " with mass " << mass << endl;
		if(maxEta<0.9){ massforLimit_CatA[j]->Fill(mass,weight); }
		if(maxEta>=0.9 && maxEta<1.9 ){ massforLimit_CatB[j]->Fill(mass,weight); }
	    }
	}

        goodmuons.clear();
    }
    
    cout << "final showdown " << "count1=" << count[0] << " count2=" << count[1] << " count3=" << count[2] << " count4=" << count[3] << endl;

    outfile->cd();
    massforLimitFull->Write();
    for(int j=0; j<num_mass_regions;j++){
        massforLimit_CatA[j]->Write();
        massforLimit_CatB[j]->Write();
    }
    htotal->Write();
    outfile->Close();
}
