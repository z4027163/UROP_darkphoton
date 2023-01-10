//#include "sumwgt.h"
#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLorentzVector.h"

using namespace std;

float iso_c,chi_c,nlayer_c,nmhits_c,nphits_c,PVd_c, vtxchi2_c;
float dxy_c, trkqoverperror_c, trklambdaerror_c;
/*double DELTAPHI( double phi1, double phi2 ){
        if( phi1 > mPI || phi1 < -mPI || phi2 > mPI || phi2 < -mPI) {
          return -999;
        }
        float dp=fabs(phi1-phi2);
        if (dp>mPI) dp-=float(2*mPI);
        return dp;
}*/


void MVAread(TMVA::Reader *reader, int year=2018, int type=1){
   
   reader->AddVariable( "trkiso", &iso_c );
   reader->AddVariable( "ntklayers", &nlayer_c );
   reader->AddVariable( "chi", &chi_c );
   reader->AddVariable( "trklambdaerror", &trklambdaerror_c );
   reader->AddVariable( "nphits", &nphits_c );
   reader->AddVariable( "trkqoverperror", &trkqoverperror_c );
   reader->AddVariable( "dxy", &dxy_c );
//   reader->AddVariable( "nmhits", &nmhits_c );
//   reader->AddVariable( "vtxchi2",&vtxchi2_c);
   //if(type>2) reader->AddVariable("PVd",&PVd_c);
   //if(type<=0 || type>4){ cout << "INPUT TYPE BUG" << endl; return;}
   TString dir="";
   TString methodName = "BDT method";
   TString weightfile = dir + TString("TMVAClassification_BDT.weights.xml");
   reader->BookMVA( methodName, weightfile );
}

void add_mva(string treepath = "tagnprobe_input_full.root", int year=2018) {

   bool debug=false; 
   vector<string> filesToRun;
//   string dirIn="/eos/cms/store/group/phys_exotica/darkPhoton/jakob/newProd/";
//   string dirIn="/eos/user/w/wangz/darkphoton/";
   string dirIn="";
   filesToRun.push_back(dirIn.append(treepath));


     
   for(unsigned int f=0; f<filesToRun.size(); f++){
    cout << filesToRun[f].c_str() << endl;
    TFile *fin = new TFile(filesToRun[f].c_str(),"UPDATE");
    TTree *tree = (TTree*)fin->Get("tpTree/sig_tree");
    int entries = tree->GetEntries();



    float charge;
    float  mupt;
    float  mueta;
    float  muphi;
    int   muid;
    float  much;
    int  muphits;
    int  mumhits;
    int  munlayer;
    float  muchi;
    float  mutrkiso;
    int nmuon;
    float vtxchi2;
    float PVd;
    float mva;
    float mva2;
    tree->SetBranchAddress("pt"  ,   &mupt);
    tree->SetBranchAddress("eta" ,   &mueta);
    tree->SetBranchAddress("phi" ,   &muphi);
    tree->SetBranchAddress("trkiso", &mutrkiso);
    tree->SetBranchAddress("nphits", &muphits);
    tree->SetBranchAddress("nmhits", &mumhits);
    //tree->SetBranchAddress("muid",muid);
    tree->SetBranchAddress("chi",    &muchi);
    //tree->SetBranchAddress("charge", &charge);
    tree->SetBranchAddress("ntklayers", &munlayer);
    //tree->SetBranchAddress("nmuon",  &nmuon);
    tree->SetBranchAddress("vtxchi2",&vtxchi2);
    tree->SetBranchAddress("PVd",    &PVd);
    //TBranch *mumva = tree->Branch("mva",mva,"mva[4]/F"); 
    TBranch *mumva2 = tree->Branch("mva2",&mva2,"mva2/F");
//    tree->Branch("mva",mva); 

    //TMVA::Reader *reader1 = new TMVA::Reader( "!Color:!Silent" );
    //MVAread(reader1,year,4);

    TMVA::Reader *reader2 = new TMVA::Reader( "!Color:!Silent" );
    MVAread(reader2,year,2);
    cout << "total=" << entries << endl;
//    entries=100000;
//    entries=entries/10;
    for(int t=0; t<entries; t++){
      tree->GetEntry(t);
      if(t%10000==0) cout << "entry=" << t << endl;
      //if(t>1000000) break;
      //PVd_c=PVd;
      //vtxchi2_c= vtxchi2; 
      //for(int i=0; i<4;i++){
         //mva[i]=-1;
         //mva2=-1;
      //}
        
      //for (int i = 0; i < nmuon; i++) {
          iso_c=mutrkiso;
          chi_c=muchi;
          nlayer_c=munlayer;
          nmhits_c=mumhits;
          nphits_c=muphits;          
          //mva[i]=reader1->EvaluateMVA( "BDT method");
          mva2=reader2->EvaluateMVA( "BDT method");
          //cout << mva2 << endl;
      //}
     //mumva->Fill();
     mumva2->Fill();
    }
    fin->cd("tpTree");
    tree->Write(0,TObject::kWriteDelete,0);
    fin->Close();
   }
}
