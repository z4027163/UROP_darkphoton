#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include "TH2D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

using namespace std;

void generateTree_tagprobe(int era=5) {

    TString outfilename = "tagnprobe_input_era";
    TFile* outfile = new TFile(outfilename + era + ".root", "RECREATE");
    TTree* jpsisig_outtree = new TTree("jpsisig_tree", "jpsisig_tree");
    TTree* upsilon_outtree = new TTree("upsilon_tree", "upsilon_tree");
    TTree* sig_outtree = new TTree("sig_tree", "sig_tree");
    TTree* bckg_outtree = new TTree("bckg_tree", "bckg_tree");

    //TFile *inputfile = TFile::Open(inputfilename);
    //TTree *intree = inputfile->Get<TTree>("mmtree/tree");
    //TTreeReader reader("tree", inputfile);
   
    TString inputdirectory = "";
    if(era==2) {inputdirectory = "/data/submit/cms/store/user/wangzqe/darkphoton/run3/ntuple/2022B/";}
    if(era==3) {inputdirectory = "/data/submit/cms/store/user/wangzqe/darkphoton/run3/ntuple/2022C/";}
    if(era==5) {inputdirectory = "/data/submit/cms/store/user/wangzqe/darkphoton/run3/ntuple/2022E/";}
    if(era==6) {inputdirectory = "/data/submit/cms/store/user/wangzqe/darkphoton/run3/ntuple/2022F/";}
    TChain* inputchain = new TChain();
    inputchain->Add(inputdirectory + "*.root/tree");
    TTreeReader reader(inputchain);

    float BSx, BSy, BSz;
    if(era==2){ BSx = 0.1739; BSy=-0.1804;}
    if(era==3){ BSx = .170; BSy = -.1761; BSz = .642;}
    if(era==5){ BSx = 0.1712;BSy=-0.1747;}
    if(era==6){ BSx= 0.170; BSy= -0.178;}


    TTreeReaderArray<float>          mpt  (reader, "pt"    );
    TTreeReaderArray<float>          meta  (reader, "eta"    );
    TTreeReaderArray<float>          mphi  (reader, "phi"    );
    TTreeReaderArray<float>          mcharge (reader, "charge"    );
    TTreeReaderArray<float>          tkiso  (reader, "trkiso"    );
    TTreeReaderArray<float>          chi2  (reader, "chi"    );
    //TTreeReaderArray<float>          ndof (reader, "Muon_ndof" );
    TTreeReaderArray<int>            nphits (reader, "nphits"     );
    TTreeReaderArray<int>            ntklayers (reader, "ntklayers"      );
    TTreeReaderArray<int>            nmhits (reader, "nmhits" );
    TTreeReaderArray<float>          dxy (reader, "dxy" );
    TTreeReaderArray<float>          dz (reader, "dz" );    
    
    //TTreeReaderArray<float>          mecaliso (reader, "ecalIso");
    //TTreeReaderArray<float>          mhcaliso (reader, "hcalIso");
    TTreeReaderArray<float>          mtrkiso (reader, "trkiso");
 
    TTreeReaderValue<int>   nmuon  (reader, "nmuon"    );
    TTreeReaderValue<unsigned int>   run  (reader, "run" );
    TTreeReaderValue<unsigned int>   lumSec (reader, "lumSec" );

    TTreeReaderValue<unsigned int>   nverts (reader, "nvtx");
    //TTreeReaderValue<float>          vtxX (reader, "vtxX");
    //TTreeReaderValue<float>          vtxY (reader, "vtxY");
    //TTreeReaderValue<float>          vtxZ (reader, "vtxZ");
    //TTreeReaderValue<float>          vtxXError (reader, "vtxXError");
    //TTreeReaderValue<float>          vtxYError (reader, "vtxYError");
    //TTreeReaderValue<float>          vtxZError (reader, "vtxZError");
    TTreeReaderValue<float>           VtxChi2 (reader, "vtxchi2");
    TTreeReaderValue<float>           pvd (reader, "PVd");
    TTreeReaderValue<float>           ip (reader, "IP");
    TTreeReaderValue<float>	      vtxerr (reader, "vtxErr");

    TTreeReaderArray<float>           trkqoverp (reader, "trkqoverp");
    TTreeReaderArray<float>           trklambda (reader, "trklambda"); 
    TTreeReaderArray<float>           trkqoverperror (reader, "trkqoverperror");
    TTreeReaderArray<float>           trklambdaerror (reader, "trklambdaerror");  

    float  m1pt    = 0.0;        
    float  m1eta   = 0.0;        
    float  m1phi   = 0.0;        
    float  m1iso   = 0.0;        
    float  m2pt    = 0.0;        
    float  m2eta   = 0.0;        
    float  m2phi   = 0.0;        
    float  m2iso   = 0.0;        
    float  mass    = 0.0;        
    float  mass4   = 0.0;        
    int   m1id    = 0;
    int   m2id    = 0;
    char   m3id    = 0;
    char   m4id    = 0;
    float  m1ch    = 0.; 
    float  m2ch    = 0.;
    int  m2phits    = 0.; 
    int  m2mhits = 0.;
    int  m2layer    = 0.; 
    int  m2stat = 0;
    float  m2chi = 0.;
    float  m2trkiso = 0;
    float dR=-1;
    vector<bool> l1Bools;
    unsigned nvtx  = 0;
    unsigned Run   = 0;
    unsigned LumSec   = 0;
    float Dxy=0;
    float Dz=0;
    float vchi2=0;
    float PVd=0;
    float IP=0;
    float vtxErr=0;
    float m2trkqoverp, m2trklambda, m2trkqoverperror, m2trklambdaerror = 0.0;

    bckg_outtree->Branch("m1pt"  , &m1pt  , "m1pt/F"  );
    bckg_outtree->Branch("m1eta" , &m1eta , "m1eta/F" );
    bckg_outtree->Branch("m1phi" , &m1phi , "m1phi/F" );
    bckg_outtree->Branch("m1id"  , &m1id  , "m1id/I"  );
    bckg_outtree->Branch("pt"  , &m2pt  , "m2pt/F"  );
    bckg_outtree->Branch("eta" , &m2eta , "m2eta/F" );
    bckg_outtree->Branch("phi" , &m2phi , "m2phi/F" );
    bckg_outtree->Branch("trkiso",&m2trkiso,"trkiso/F");
    bckg_outtree->Branch("nphits",&m2phits,"m2phits/I");
    bckg_outtree->Branch("nmhits",&m2mhits,"m2mhits/I");
    bckg_outtree->Branch("chi",&m2chi,"m2chi/F");
    bckg_outtree->Branch("ntklayers",&m2layer,"ntklayers/I");
    bckg_outtree->Branch("nstations",&m2stat,"nstations/I");
    bckg_outtree->Branch("m2id"  , &m2id  , "m2id/I"  );
    bckg_outtree->Branch("mass"  , &mass  , "mass/F"  );
    bckg_outtree->Branch("nvtx"  , &nvtx  , "nvtx/i"  );
    bckg_outtree->Branch("Run"   , &Run   , "Run/i"  );
    bckg_outtree->Branch("LumSec", &LumSec, "LumSec/i"  );
    bckg_outtree->Branch("dR", &dR,"dR/F");

    bckg_outtree->Branch("dxy",&Dxy,"dxy/F");
    bckg_outtree->Branch("dz",&Dz,"dz/F");
    bckg_outtree->Branch("vtxchi2",&vchi2,"vtxchi2/F");
    bckg_outtree->Branch("PVd",&PVd,"PVd/F");
    bckg_outtree->Branch("IP",&IP,"IP/F");
    bckg_outtree->Branch("vtxErr",&vtxErr,"vtxErr/F");
    bckg_outtree->Branch("trkqoverp", &m2trkqoverp, "trkqoverp/F");
    bckg_outtree->Branch("trklambda", &m2trklambda, "trklambda/F");
    bckg_outtree->Branch("trkqoverperror", &m2trkqoverperror, "trkqoverperror/F");
    bckg_outtree->Branch("trklambdaerror", &m2trklambdaerror, "trklambdaerror/F");

    sig_outtree->Branch("m1pt"  , &m1pt  , "m1pt/F"  );
    sig_outtree->Branch("m1eta" , &m1eta , "m1eta/F" );
    sig_outtree->Branch("m1phi" , &m1phi , "m1phi/F" );
    sig_outtree->Branch("m1id"  , &m1id  , "m1id/I"  );
    sig_outtree->Branch("pt"  , &m2pt  , "m2pt/F"  );
    sig_outtree->Branch("eta" , &m2eta , "m2eta/F" );
    sig_outtree->Branch("phi" , &m2phi , "m2phi/F" );
    sig_outtree->Branch("trkiso",&m2trkiso,"trkiso/F");
    sig_outtree->Branch("nphits",&m2phits,"m2phits/I");
    sig_outtree->Branch("nmhits",&m2mhits,"m2mhits/I");
    sig_outtree->Branch("chi",&m2chi,"m2chi/F");
    sig_outtree->Branch("ntklayers",&m2layer,"ntklayers/I");
    sig_outtree->Branch("nstations",&m2stat,"nstations/I");
    sig_outtree->Branch("m2id"  , &m2id  , "m2id/I"  );
    sig_outtree->Branch("mass"  , &mass  , "mass/F"  );
    sig_outtree->Branch("nvtx"  , &nvtx  , "nvtx/i"  );
    sig_outtree->Branch("Run"   , &Run   , "Run/i"  );
    sig_outtree->Branch("LumSec", &LumSec, "LumSec/i"  );
    sig_outtree->Branch("dR", &dR,"dR/F");

    sig_outtree->Branch("dxy",&Dxy,"dxy/F");
    sig_outtree->Branch("dz",&Dz,"dz/F");
    sig_outtree->Branch("vtxchi2",&vchi2,"vtxchi2/F");
    sig_outtree->Branch("PVd",&PVd,"PVd/F");
    sig_outtree->Branch("IP",&IP,"IP/F");
    sig_outtree->Branch("vtxErr",&vtxErr,"vtxErr/F");
    sig_outtree->Branch("trkqoverp", &m2trkqoverp, "trkqoverp/F");
    sig_outtree->Branch("trklambda", &m2trklambda, "trklambda/F");
    sig_outtree->Branch("trkqoverperror", &m2trkqoverperror, "trkqoverperror/F");
    sig_outtree->Branch("trklambdaerror", &m2trklambdaerror, "trklambdaerror/F");

    jpsisig_outtree->Branch("m1pt"  , &m1pt  , "m1pt/F"  );
    jpsisig_outtree->Branch("m1eta" , &m1eta , "m1eta/F" );
    jpsisig_outtree->Branch("m1phi" , &m1phi , "m1phi/F" );
    jpsisig_outtree->Branch("m1id"  , &m1id  , "m1id/I"  );
    jpsisig_outtree->Branch("pt"  , &m2pt  , "m2pt/F"  );
    jpsisig_outtree->Branch("eta" , &m2eta , "m2eta/F" );
    jpsisig_outtree->Branch("phi" , &m2phi , "m2phi/F" );
    jpsisig_outtree->Branch("trkiso",&m2trkiso,"trkiso/F");
    jpsisig_outtree->Branch("nphits",&m2phits,"m2phits/I");
    jpsisig_outtree->Branch("nmhits",&m2mhits,"m2mhits/I");
    jpsisig_outtree->Branch("chi",&m2chi,"m2chi/F");
    jpsisig_outtree->Branch("ntklayers",&m2layer,"ntklayers/I");
    jpsisig_outtree->Branch("nstations",&m2stat,"nstations/I");
    jpsisig_outtree->Branch("m2id"  , &m2id  , "m2id/I"  );
    jpsisig_outtree->Branch("mass"  , &mass  , "mass/F"  );
    jpsisig_outtree->Branch("nvtx"  , &nvtx  , "nvtx/i"  );
    jpsisig_outtree->Branch("Run"   , &Run   , "Run/i"  );
    jpsisig_outtree->Branch("LumSec", &LumSec, "LumSec/i"  );
    jpsisig_outtree->Branch("dR", &dR,"dR/F");
 
    jpsisig_outtree->Branch("dxy",&Dxy,"dxy/F");
    jpsisig_outtree->Branch("dz",&Dz,"dz/F");
    jpsisig_outtree->Branch("vtxchi2",&vchi2,"vtxchi2/F");
    jpsisig_outtree->Branch("PVd",&PVd,"PVd/F");
    jpsisig_outtree->Branch("IP",&IP,"IP/F");
    jpsisig_outtree->Branch("vtxErr",&vtxErr,"vtxErr/F");
    jpsisig_outtree->Branch("trkqoverp", &m2trkqoverp, "trkqoverp/F");
    jpsisig_outtree->Branch("trklambda", &m2trklambda, "trklambda/F");
    jpsisig_outtree->Branch("trkqoverperror", &m2trkqoverperror, "trkqoverperror/F");
    jpsisig_outtree->Branch("trklambdaerror", &m2trklambdaerror, "trklambdaerror/F");

    upsilon_outtree->Branch("m1pt"  , &m1pt  , "m1pt/F"  );
    upsilon_outtree->Branch("m1eta" , &m1eta , "m1eta/F" );
    upsilon_outtree->Branch("m1phi" , &m1phi , "m1phi/F" );
    upsilon_outtree->Branch("m1id"  , &m1id  , "m1id/I"  );
    upsilon_outtree->Branch("pt"  , &m2pt  , "m2pt/F"  );
    upsilon_outtree->Branch("eta" , &m2eta , "m2eta/F" );
    upsilon_outtree->Branch("phi" , &m2phi , "m2phi/F" );
    upsilon_outtree->Branch("trkiso",&m2trkiso,"trkiso/F");
    upsilon_outtree->Branch("nphits",&m2phits,"m2phits/I");
    upsilon_outtree->Branch("nmhits",&m2mhits,"m2mhits/I");
    upsilon_outtree->Branch("chi",&m2chi,"m2chi/F");
    upsilon_outtree->Branch("ntklayers",&m2layer,"ntklayers/I");
    upsilon_outtree->Branch("nstations",&m2stat,"nstations/I");
    upsilon_outtree->Branch("m2id"  , &m2id  , "m2id/I"  );
    upsilon_outtree->Branch("mass"  , &mass  , "mass/F"  );
    upsilon_outtree->Branch("nvtx"  , &nvtx  , "nvtx/i"  );
    upsilon_outtree->Branch("Run"   , &Run   , "Run/i"  );
    upsilon_outtree->Branch("LumSec", &LumSec, "LumSec/i"  );
    upsilon_outtree->Branch("dR", &dR,"dR/F");

    upsilon_outtree->Branch("dxy",&Dxy,"dxy/F");
    upsilon_outtree->Branch("dz",&Dz,"dz/F");
    upsilon_outtree->Branch("vtxchi2",&vchi2,"vtxchi2/F");
    upsilon_outtree->Branch("PVd",&PVd,"PVd/F");
    upsilon_outtree->Branch("IP",&IP,"IP/F");
    upsilon_outtree->Branch("vtxErr",&vtxErr,"vtxErr/F");
    upsilon_outtree->Branch("trkqoverp", &m2trkqoverp, "trkqoverp/F");
    upsilon_outtree->Branch("trklambda", &m2trklambda, "trklambda/F");
    upsilon_outtree->Branch("trkqoverperror", &m2trkqoverperror, "trkqoverperror/F");
    upsilon_outtree->Branch("trklambdaerror", &m2trklambdaerror, "trklambdaerror/F");

    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;
	
    int k = 0;
    while(reader.Next()) {
	if (k%1000000==0) cout << k << " events processed" << endl;
	k=k+1;	

        if (k%20 == 0) continue; //so that we don't use the 5% of the data that was used to train on

	//if (k>1000000) break;

	//check for two muons
	if(*nmuon<2) continue;
	count1 ++;	

	//only check first muon for tag
	if(((mpt)[0])<8 || fabs((meta)[0])>0.9) continue;
        if(((nphits)[0] == 0) || (ntklayers)[0] <= 5 || (chi2)[0] > 10. || (tkiso)[0]>0.15) continue;
	count2 ++;

	//get probe muon (make sure there is only one)
	unsigned idx1 = 0;
	unsigned idx2 = 1;
	bool jpsisignal = false;
	bool upsilonsignal = false;
	bool background = false;
	int nprob = 0;
	for(std::size_t i = 1; i < *nmuon; i++) {
             //cout << mpt[i] << endl;
	     if(((mpt)[i])<3 || fabs((meta)[i])>1.9) continue;
             if((mcharge)[0]*(mcharge)[i]>0) {
		background = true;
	     }
	     nprob++;
             idx2=i;
        }
	//cout << nprob << endl;       
        if(nprob!=1) continue;
	count3 ++;

	//PVd Cut
	if (nvtx > 0) {
            //vchi2=(vtxchi2);
            //PVd=sqrt( ((*vtxX) - BSx)*((*vtxX) - BSx) + ((*vtxY) - BSy)*((*vtxY) - BSy) );
            //double temx=(*vtxX) - BSx;
            //double temy=(*vtxY) - BSy;
            //double temr=sqrt(temx*temx+temy*temy);
            //vtxErr=sqrt(temx*temx*(*vtxXError)*(*vtxXError)+temy*temy*(*vtxYError)*(*vtxYError))/temr;
            //IP=PVd/(vtxErr);
            PVd = *pvd;
	    IP = *ip;
	    vtxErr = *vtxerr;
	    vchi2 = *VtxChi2;	    
            if (*pvd > .2) continue;
        }
	count4 ++;
  
        TLorentzVector mm;
	
        TLorentzVector m1;
        TLorentzVector m2;
	
        m1.SetPtEtaPhiM((mpt)[idx1], (meta)[idx1], (mphi)[idx1], 0.1057);
        m2.SetPtEtaPhiM((mpt)[idx2], (meta)[idx2], (mphi)[idx2], 0.1057);
        dR = m1.DeltaR(m2);
	
	mm += m1;
        mm += m2;
        m1pt   = m1.Pt();
        m1eta  = m1.Eta();
        m1phi  = m1.Phi();
        m1iso  = (tkiso)[idx1];
        m1id=1;
        m1ch   = (mcharge)[idx1];

        m2pt   = m2.Pt();
        m2eta  = m2.Eta();
        m2phi  = m2.Phi();
	m2id=0;
        m2trkiso = (tkiso)[idx2];
        m2phits=(nphits)[idx2];
        m2mhits=(nmhits)[idx2];
	m2layer=(ntklayers)[idx2];
        m2chi=(chi2)[idx2];
        m2trkqoverp = (trkqoverp)[idx2];
	m2trklambda = (trklambda)[idx2];
	m2trkqoverperror = (trkqoverperror)[idx2];
        m2trklambdaerror = (trklambdaerror)[idx2];

	if((tkiso)[idx2]<0.15 && ((nphits)[idx2] > 0) && (ntklayers)[idx2] > 5 && (chi2)[idx2] < 10.) m2id=1;
	m2ch   = (mcharge)[idx2];

	Dxy=(dxy)[idx2];
        Dz=(dz)[idx2];
	
	mass   = mm.M();
	if (mass > 40) continue;
	nvtx = *nverts;

	Run=*run;
	LumSec=*lumSec;

	if (!background && (mass>3.096-.03 && mass<3.096+.03)) jpsisignal=true;
        if (!background && (mass>9.2 && mass < 10.5)) upsilonsignal = true;

	if (jpsisignal) jpsisig_outtree->Fill();
	if (upsilonsignal) upsilon_outtree->Fill();
	//if (jpsisignal || upsilonsignal) sig_outtree->Fill();
	if (background) bckg_outtree->Fill(); else sig_outtree->Fill();
    }
    cout << "Count 1: " << count1 << ", Count 2: " << count2 << ", Count 3: " << count3 << ", Count 4: " << count4 << endl;

    outfile->cd();
    outfile->mkdir("tpTree");
    outfile->cd("tpTree");
    jpsisig_outtree->Write("jpsisig_tree", TObject::kOverwrite);
    upsilon_outtree->Write("upsilon_tree", TObject::kOverwrite);
    sig_outtree->Write("sig_tree", TObject::kOverwrite);
    bckg_outtree->Write("bckg_tree", TObject::kOverwrite);
    outfile->Close();
	
}
