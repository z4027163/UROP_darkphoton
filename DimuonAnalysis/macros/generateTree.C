#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include "TH2D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

using namespace std;

void generateTree(TString inputfilename = "input.root", const char* outfilename = "./intermediate.root", int era=6, bool isMC=false) {

    TFile* outfile = new TFile(outfilename, "RECREATE");
    TTree* outtree = new TTree("tree", "tree");

    TFile *inputfile = TFile::Open(inputfilename);
    TH1F *htotal = (TH1F*)inputfile->Get("mmtree/htotal");
    TTree *intree = inputfile->Get<TTree>("mmtree/tree");
    TTreeReader reader("mmtree/tree", inputfile);

    //cout << "Getting beam spot position" << endl;
    //intree->Draw("vtxX >> BSxhist");
    //intree->Draw("vtxY >> BSyhist");
    //intree->Draw("vtxZ >> BSzhist");
    //TH1F BSxhist = (TH1F)gPad->GetPrimitive("BSxhist");
    //TH1F BSyhist = (TH1F)gPad->GetPrimitive("BSyhist");
    //TH1F BSzhist = (TH1F)gPad->GetPrimitive("BSzhist");
    //float BSx = BSxhist.GetMean();
    //float BSy = BSyhist.GetMean();
    //float BSz = BSzhist.GetMean();

    float BSx = .2001;
    float BSy = -.2319;
    float BSz = .642;
    float phi0 = -0.8037;
    
    if(era==2){ BSx = 0.1739; BSy=-0.1804; phi0=-0.8037;}
    if(era==3){ BSx = .170; BSy = -.1761; BSz = .642; phi0=-0.8030;}
    if(era==5){ BSx = 0.1712;BSy=-0.1747; phi0=-0.7955;}
    if(era==6){ BSx= 0.170; BSy= -0.178; phi0=-0.8083;}
     
    cout << "Beam spot position: (" << BSx << "," << BSy << "," << BSz << ")" << endl;

    TTreeReaderArray<float>          mpt  (reader, "Muon_pt"    );
    TTreeReaderArray<float>          meta  (reader, "Muon_eta"    );
    TTreeReaderArray<float>          mphi  (reader, "Muon_phi"    );
    TTreeReaderArray<float>          mcharge (reader, "Muon_charge"    );
    TTreeReaderArray<float>          tkiso  (reader, "Muon_trkiso"    );
    TTreeReaderArray<float>          chi2  (reader, "Muon_chi2"    );
    TTreeReaderArray<float>          ndof (reader, "Muon_ndof" );
    TTreeReaderArray<float>          nphits (reader, "Muon_validpixelhits"     );
    TTreeReaderArray<float>          ntklayers (reader, "Muon_nvalidstriphits"      );
    TTreeReaderArray<float>          nmhits (reader, "Muon_nvalidmuon_hits" );
    TTreeReaderArray<float>          dxy (reader, "Muon_dxy" );
    TTreeReaderArray<float>          dz (reader, "Muon_dz" );    
    
    TTreeReaderArray<float>          mecaliso (reader, "Muon_ecaliso");
    TTreeReaderArray<float>          mhcaliso (reader, "Muon_hcaliso");
    TTreeReaderArray<float>          mtrkiso (reader, "Muon_trkiso");
 
    TTreeReaderValue<unsigned int>   Nmuon  (reader, "n_mu"    );
    TTreeReaderValue<unsigned int>   Run  (reader, "run" );
    TTreeReaderValue<unsigned int>   LumSec (reader, "lumSec" );

    TTreeReaderValue<unsigned int>   Nvtx (reader, "nvtx");
    TTreeReaderArray<float>          VtxX (reader, "vtxX");
    TTreeReaderArray<float>          VtxY (reader, "vtxY");
    TTreeReaderArray<float>          VtxZ (reader, "vtxZ");
    TTreeReaderArray<float>          VtxXError (reader, "vtxXError");
    TTreeReaderArray<float>          VtxYError (reader, "vtxYError");
    TTreeReaderArray<float>          VtxZError (reader, "vtxZError");
    TTreeReaderArray<float>          VtxChi2 (reader, "vtxChi2");

    TTreeReaderArray<float>          Trkqoverp (reader, "Muon_trkqoverp");
    TTreeReaderArray<float>          Trklambda (reader, "Muon_trklambda");
    TTreeReaderArray<float>          Trkqoverperror (reader, "Muon_trkqoverperror");
    TTreeReaderArray<float>          Trklambdaerror (reader, "Muon_trklambdaerror");


    float charge[4]={0};
    float  mupt[4]    = {0};
    float  mueta[4]   = {0};
    float  muphi[4]   = {0};
    int  muphits[4]    = {0};
    int  mumhits[4] = {0};
    int  munlayer[4]    = {0};
    float  muchi[4] = {0.};
    float  mutrkiso[4] = {0};
    float Dxy[4]={-100,-100,-100,-100};
    float Dz[4]={-100,-100,-100,-100};
    float MecalIso[4] = {0};
    float MhcalIso[4] = {0};
    float MtrkIso[4] = {0};
    float trklambda[4] = {0};
    float trkqoverp[4] = {0};
    float trklambdaerror[4] = {0};
    float trkqoverperror[4] = {0};
    float PVd, IP, vtxErr;
    unsigned run, lumSec; 
    unsigned nvtx;
    int n_mu;
    float vtxX, vtxY, vtxZ, vtxXError, vtxYError, vtxZError, vtxchi2;

    outtree->Branch("pt"  , mupt  , "pt[4]/F"  );
    outtree->Branch("eta" , mueta , "eta[4]/F" );
    outtree->Branch("phi" , muphi , "phi[4]/F" );
    outtree->Branch("trkiso",mutrkiso,"trkiso[4]/F");
    outtree->Branch("nphits",muphits,"nphits[4]/I");
    outtree->Branch("nmhits",mumhits,"nmhits[4]/I");
    outtree->Branch("chi",muchi,"chi[4]/F");
    outtree->Branch("charge",charge,"charge[4]/F");
    outtree->Branch("ntklayers",munlayer,"ntklayers[4]/I");
    outtree->Branch("run"   , &run   , "run/i"  );
    outtree->Branch("lumSec", &lumSec);//, "lumSec/i"  );
    outtree->Branch("nmuon", &n_mu);//, "n_mu/i"  );
    outtree->Branch("dxy",Dxy,"dxy[4]/F");
    outtree->Branch("dz",Dz,"dz[4]/F");
    
    outtree->Branch("ecalIso", MecalIso, "mecaliso[4]/F");
    outtree->Branch("hcalIso", MhcalIso, "mhcaliso[4]/F");
    //outtree->Branch("trkIso", MtrkIso, "mtrkiso[4]/F");

    outtree->Branch("nvtx"  , &nvtx  , "nvtx/i"  );   
    outtree->Branch("vtxX", &vtxX, "nvtxX/F");
    outtree->Branch("vtxY", &vtxY, "nvtxY/F");
    outtree->Branch("vtxZ", &vtxZ, "nvtxZ/F");
    outtree->Branch("vtxXError", &vtxXError, "nvtxXError/F");
    outtree->Branch("vtxYError", &vtxYError, "nvtxYError/F");
    outtree->Branch("vtxZError", &vtxZError, "nvtxZError/F");
    outtree->Branch("vtxchi2", &vtxchi2, "vtxchi2/F");
    outtree->Branch("PVd",&PVd,"PVd/F");
    outtree->Branch("IP",&IP,"IP/F");
    outtree->Branch("vtxErr",&vtxErr,"vtxErr/F");

    outtree->Branch("trkqoverp", &trkqoverp, "trkqoverp[4]/F");
    outtree->Branch("trklambda", &trklambda, "trklambda[4]/F");
    outtree->Branch("trkqoverperror", &trkqoverperror, "trkqoverperror[4]/F");
    outtree->Branch("trklambdaerror", &trklambdaerror, "trklambdaerror[4]/F");


    float r0 = sqrt(BSx*BSx+BSy*BSy);

    int j=0;
    while(reader.Next()) {
        if (j%100000==0) cout << j << " events completed" << endl;
	j+=1;
        
	nvtx = *Nvtx;
	if(nvtx<=0) continue;
 
	int idx=0;
	for (std::size_t i=0; i< mpt.GetSize(); i++) {
	    if (idx>=4) continue;
            
	    //INSERT PRE SELECTION HERE
	    if(mpt[i]<4 || abs(meta[i])>1.9) continue;
	    
	    mupt[idx]=(mpt)[i];
	    mueta[idx]=(meta)[i];
	    muphi[idx]=(mphi)[i];
	    mutrkiso[idx]=(tkiso)[i];
	    muphits[idx]=(nphits)[i];
	    mumhits[idx]=(nmhits)[i];
	    muchi[idx]=(chi2)[i]/(ndof)[i];
	    charge[idx]=(mcharge)[i];
	    munlayer[idx]=(ntklayers)[i];
	    
	    Dxy[idx]=(dxy)[i] + r0*sin((mphi)[i]-phi0);
	    Dz[idx]=(dz)[i];

	    MecalIso[idx]=(mecaliso)[i];
	    MhcalIso[idx]=(mhcaliso)[i];
	 
	    trkqoverp[idx] = (Trkqoverp)[i];
	    trklambda[idx] = (Trklambda)[i];
	    trkqoverperror[idx] = (Trkqoverperror)[i];
            trklambdaerror[idx] = (Trklambdaerror)[i];
   
	    idx+=1;
	}

        n_mu = idx;
        run = *Run;
        lumSec = *LumSec;
	
	vtxX = VtxX[0];
	vtxY = VtxY[0];
	vtxZ = VtxZ[0];
	vtxXError = VtxXError[0];
        vtxYError = VtxYError[0];
        vtxZError = VtxZError[0];
	vtxchi2 = VtxChi2[0];
        if (nvtx>0) {
	PVd=sqrt( ((vtxX) - BSx)*((vtxX) - BSx) + ((vtxY) - BSy)*((vtxY) - BSy) );
		double temx=(vtxX) - BSx;
        	double temy=(vtxY) - BSy;
        	double temr=sqrt(temx*temx+temy*temy);
        	vtxErr=sqrt(temx*temx*(vtxXError)*(vtxXError)+temy*temy*(vtxYError)*(vtxYError))/temr;
        	IP=PVd/vtxErr;
	}

	outtree->Fill(); 
   }
     
   outfile->cd();
   htotal->Write();
   outtree->Write();
   outfile->Close();
}




