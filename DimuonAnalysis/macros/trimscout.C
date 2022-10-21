#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include "TH2D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

using namespace std;

void trimscout(TString inputfilename = "input.root", const char* outfilename = "./scoutDataMVA_v2.root", bool isMC=false) {

    TFile* outfile = new TFile(outfilename, "RECREATE");
    TTree* outtree = new TTree("tree", "tree");

    TH1F* massforLimitFull = new TH1F("massforLimitFull","massforLimitFull",4000,0., 40.);

    TFile *inputfile = TFile::Open(inputfilename);

    TTreeReader reader("mmtree/tree", inputfile);

    TTreeReaderArray<float>          mucharge  (reader, "Muon_charge"    );
    TTreeReaderArray<float>          mupt  (reader, "Muon_pt"    );
    TTreeReaderArray<float>          mueta  (reader, "Muon_eta"    );
    TTreeReaderArray<float>          muphi  (reader, "Muon_phi"    );
    TTreeReaderArray<float>          muchg (reader, "Muon_charge"    );
    TTreeReaderArray<float>          RelTrackIso  (reader, "Muon_trkiso"    );
    TTreeReaderArray<float>          muTrkChi2  (reader, "Muon_chi2"    );
    TTreeReaderArray<float>          nPixelHits (reader, "Muon_validpixelhits"     );
    TTreeReaderArray<float>          nTrkLayers (reader, "Muon_nvalidstriphits"      );
    TTreeReaderValue<unsigned int>          nmuon  (reader, "n_mu"    );

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
        for (int i = 0; i < *nmuon; i++) {
            if(mupt[i]<4 || abs(mueta[i])>1.9) continue;
            //if(nPixelHits[i]==0) continue;
            //if(nTrkLayers[i]<=5) continue;
            //if(muTrkChi2[i]>=10) continue;
            //if(RelTrackIso[i]>=.15) continue;
            goodmuons.push_back(i);
        }
        
        //cout << "Good Muons: " << goodmuons.size() << endl;
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

        goodmuons.clear();
    }
    
    cout << "final showdown " << "count1=" << count[0] << " count2=" << count[1] << " count3=" << count[2] << " count4=" << count[3] << endl;

    outfile->cd();
    massforLimitFull->Write();
    outfile->Close();
}
