// Standard C++ includes
#include <memory>
#include <vector>
#include <iostream>

// ROOT includes
#include <TTree.h>
#include <TLorentzVector.h>
#include <TPRegexp.h>

// CMSSW framework includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

// CMSSW data formats
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenLumiInfoHeader.h"

// Other relevant CMSSW includes
#include "CommonTools/UtilAlgos/interface/TFileService.h" 


#include <memory>
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class GenTreeMaker : public edm::EDAnalyzer {
	public:
	explicit GenTreeMaker(const edm::ParameterSet&);
	~GenTreeMaker();
		
	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
	
	
	private:
        virtual void beginJob() override;
        virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
        virtual void endJob() override;

        const edm::EDGetTokenT<std::vector<reco::GenParticle> > gensToken;
        const edm::EDGetTokenT<GenEventInfoProduct>             genEvtInfoToken;


        // 4-vector of genparticles, and their PDG IDs            
        std::vector<float>           m_pt;
        std::vector<float>           m_eta;
        std::vector<float>           m_phi;
        std::vector<int>           m_charge;
        float mass;
 
        // TTree carrying the event weight information
        TTree* tree;

  	//Run and lumisection

};

GenTreeMaker::GenTreeMaker(const edm::ParameterSet& iConfig): 
    gensToken               (consumes<std::vector<reco::GenParticle> >        (iConfig.getParameter<edm::InputTag>("gens"))),
    genEvtInfoToken         (consumes<GenEventInfoProduct>                    (iConfig.getParameter<edm::InputTag>("geneventinfo")))
{
}


GenTreeMaker::~GenTreeMaker() {
}

void GenTreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;
    using namespace std;
    using namespace reco;
    
    
    Handle<GenEventInfoProduct> genEvtInfoH;
    iEvent.getByToken(genEvtInfoToken, genEvtInfoH);
    
    Handle<vector<GenParticle> > gensH;
    iEvent.getByToken(gensToken, gensH);
    
    
    
    // Clear all the vectors for every event
    m_pt.clear(); 
    m_eta.clear();
    m_phi.clear();
    m_charge.clear();
    mass=-1;    
    // Muon information
    // GEN information

    TLorentzVector P1,P2;
    int nlep=0;
    for (auto gens_iter = gensH->begin(); gens_iter != gensH->end(); ++gens_iter) {
        if (abs(gens_iter->pdgId())==13 && gens_iter->status()==1 && abs(gens_iter->mother(0)->pdgId())<100){
                   if(nlep==0) P1.SetPtEtaPhiM(gens_iter->pt(), gens_iter->eta(), gens_iter->phi(), gens_iter->mass());
                   if(nlep==1) P2.SetPtEtaPhiM(gens_iter->pt(), gens_iter->eta(), gens_iter->phi(), gens_iter->mass());
                   nlep++;
                   m_pt.push_back(gens_iter->pt());
                   m_eta.push_back(gens_iter->eta());
                   m_phi.push_back(gens_iter->phi());
                   m_charge.push_back(gens_iter->charge());
         }
    }
   if(nlep!=2) cout << "gen lep=" << nlep << endl;
   mass = (P1+P2).M();

   tree->Fill();	
	
}


void GenTreeMaker::beginJob() {
    // Access the TFileService
    edm::Service<TFileService> fs;


    //book Histos
    //diMuNamesHisto_ = fs_->make<TH1F>("diMuMass", "diMuMass", 500, 0, 100);


    // Create the TTree
    tree = fs->make<TTree>("tree"       , "tree");

    // Event weights

    // Gen info
    tree->Branch("pt"                  , "std::vector<float>"             , &m_pt      , 32000, 0);
    tree->Branch("eta"                , "std::vector<float>"  		 , &m_eta    , 32000, 0);
    tree->Branch("phi"             , "std::vector<float>"             , &m_phi , 32000, 0);
    tree->Branch("charge"           , "std::vector<int>"             , &m_charge , 32000, 0);
    tree->Branch("mass"             ,   &mass,"mass/F");
}

void GenTreeMaker::endJob() {
}

void GenTreeMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}

DEFINE_FWK_MODULE(GenTreeMaker);
