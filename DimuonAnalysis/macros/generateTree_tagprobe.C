#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include "TH2D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

using namespace std;

void generateTree_tagprobe(TString inputfilename = "input.root", const char* outfilename = "./scout_probe.root", int era=2) {

    TFile* outfile = new TFile(outfilename, "RECREATE");
    TTree* jpsisig_outtree = new TTree("jpsisig_tree", "jpsisig_tree");
    TTree* bckg_outtree = new TTree("bckg_tree", "bckg_tree");

    //TFile *inputfile = TFile::Open(inputfilename);
    //TTree *intree = inputfile->Get<TTree>("mmtree/tree");
    //TTreeReader reader("tree", inputfile);

    TString inputdirectory = "/data/submit/cms/store/user/wangzqe/darkphoton/run3_ntuple/2022B/";
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
    //TTreeReaderValue<float>          vtxchi2 (reader, "vtxchi2");
    TTreeReaderValue<float>           pvd (reader, "PVd");
    TTreeReaderValue<float>           ip (reader, "IP");
    TTreeReaderValue<float>	      vtxerr (reader, "vtxErr");

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

    int count1 = 0;
    int count2 = 0;
    int count3 = 0;
    int count4 = 0;

    while(reader.Next()) {

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
            if (*pvd > .2) continue;
        }
	count4 ++;
  
        TLorentzVector mm;
	
        TLorentzVector m1;
        TLorentzVector m2;
	
        m1.SetPtEtaPhiM((mpt)[idx1], (meta)[idx1], (mphi)[idx1], 0.1057);
        m2.SetPtEtaPhiM((mpt)[idx2], (meta)[idx2], (mphi)[idx2], 0.1057);
    
	
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

	if((tkiso)[idx2]<0.15 && ((nphits)[idx2] > 0) && (ntklayers)[idx2] > 5 && (chi2)[idx2] < 10.) m2id=1;
	m2ch   = (mcharge)[idx2];

	Dxy=(dxy)[idx2];
        Dz=(dz)[idx2];
	
	mass   = mm.M();
	if (mass > 40) continue;
	nvtx = *nverts;

	Run=*run;
	LumSec=*lumSec;

	if (!background && (mass>3.096-.5 && mass<3.096+.5)) jpsisignal=true;

	if (jpsisignal) jpsisig_outtree->Fill();
	if (background) bckg_outtree->Fill();
    }
    cout << "Count 1: " << count1 << ", Count 2: " << count2 << ", Count 3: " << count3 << ", Count 4: " << count4 << endl;

    outfile->cd();
    jpsisig_outtree->Write("jpsisig_tree", TObject::kOverwrite);
    bckg_outtree->Write("bckg_tree", TObject::kOverwrite);
    outfile->Close();
	
}
