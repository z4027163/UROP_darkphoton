#include "TFileCollection.h"
#include "TChain.h"
#include "TFile.h"
#include <TTreeReader.h>
#include "TH1D.h"
#include <TTreeReaderValue.h>
#include "TLorentzVector.h"

using namespace std;

void read(){
  TFile *f = new TFile("output.root");
  TH1F *h1 = (TH1F*)f->Get("vtxX");
  TH1F *h2 = (TH1F*)f->Get("vtxY");
  cout << "BSx="  << h1->GetMean() << endl;
  cout << "BSy=" << h2->GetMean() << endl;
}
