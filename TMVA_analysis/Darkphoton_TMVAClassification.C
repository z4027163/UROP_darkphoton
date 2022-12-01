#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

//#include "TMVAGui.C"

#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#endif

void Darkphoton_TMVAClassification( TString inputfilename = "scout_probe.root" ) {

    TMVA::Tools::Instance();
    
    TString outfileName( "TMVA.root" );
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

    TFile *input            = TFile::Open(inputfilename);
    TTree *background       = (TTree*)input->Get("bckg_tree");
    TTree *jpsi_signal      = (TTree*)input->Get("jpsisig_tree");
    TTree *upsilon_signal   = (TTree*)input->Get("upsilon_tree");

    Double_t signalWeight           = 1;
    Double_t backgroundWeight       = 1;

    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");
    TMVA::DataLoader *dataloader_upsilon = new TMVA::DataLoader("upsilon_dataset");
    TMVA::DataLoader *dataloader_jpsi = new TMVA::DataLoader("jpsi_dataset");
    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                           "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    
    dataloader->AddSignalTree    ( jpsi_signal,     signalWeight     );
    dataloader->AddSignalTree    ( upsilon_signal,  signalWeight     );
    dataloader->AddBackgroundTree( background, backgroundWeight );
    dataloader_jpsi->AddSignalTree    ( jpsi_signal,     signalWeight     );
    dataloader_jpsi->AddBackgroundTree( background, backgroundWeight );
    dataloader_upsilon->AddSignalTree    ( upsilon_signal,  signalWeight     );
    dataloader_upsilon->AddBackgroundTree( background, backgroundWeight );

    dataloader->AddVariable( "ntklayers","ntklayer","", 'I' );
    dataloader->AddVariable( "chi","chi2","", 'F' );
    dataloader->AddVariable( "nphits","nphits","", 'I' );
    dataloader->AddVariable( "trkiso","trkiso","", 'F' );
    dataloader_upsilon->AddVariable( "ntklayers","ntklayer","", 'I' );
    dataloader_upsilon->AddVariable( "chi","chi2","", 'F' );
    dataloader_upsilon->AddVariable( "nphits","nphits","", 'I' );
    dataloader_upsilon->AddVariable( "trkiso","trkiso","", 'F' );
    dataloader_jpsi->AddVariable( "ntklayers","ntklayer","", 'I' );
    dataloader_jpsi->AddVariable( "chi","chi2","", 'F' );
    dataloader_jpsi->AddVariable( "nphits","nphits","", 'I' );
    dataloader_jpsi->AddVariable( "trkiso","trkiso","", 'F' );

    //dataloader->AddVariable( "vtxchi2","vtxchi2","",'F');
    //dataloader->AddVariable( "PVd","PVd","",'F');

    TCut mycuts = "";
    dataloader->PrepareTrainingAndTestTree(mycuts, "nTrain_Signal=30000:nTrain_Background=30000:SplitMode=Random:NormMode=NumEvents:!V" );

    factory->BookMethod(dataloader, TMVA::Types::kCuts, "Cuts",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
    factory->BookMethod(dataloader,  TMVA::Types::kCuts, "CutsPCA",
                           "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT (total trained)",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    
    factory->BookMethod(dataloader, TMVA::Types::kPyKeras, "PyKeras",
                           "H:!V:VarTransform=D,G:FilenameModel=modelClassification.h5:FilenameTrainedModel=trainedModelClassification.h5:NumEpochs=20:BatchSize=32");

    //factory->BookMethod(dataloader_upsilon, TMVA::Types::kBDT, "BDT (upsilon trained)",
    //                       "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    //factory->BookMethod(dataloader_jpsi, TMVA::Types::kBDT, "BDT (jpsi trained)",
    //                       "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    

    //factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT (larger trees)",
    //                       "!H:!V:NTrees=850:MinNodeSize=5%:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    //factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTD",
    //                       "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );
    //factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG",
    //                       "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );
    //factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTB",
    //                       "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20" );
    //factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTMitFisher",
    //                       "!H:!V:NTrees=50:MinNodeSize=2.5%:UseFisherCuts:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:SeparationType=GiniIndex:nCuts=20" );

    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    outputFile->Close();

    delete factory;

    //if (!gROOT->IsBatch()) TMVAGui( outfileName );

}
