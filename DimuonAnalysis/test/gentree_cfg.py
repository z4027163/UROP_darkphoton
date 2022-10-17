import FWCore.ParameterSet.Config as cms

# Set parameters externally 
from FWCore.ParameterSet.VarParsing import VarParsing
params = VarParsing('analysis')

# Define the process
process = cms.Process("GenRead")

# Parse command line arguments
params.parseArguments()

# Message Logger settings
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.destinations = ['cout', 'cerr']
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# Set the process options -- Display summary at the end, enable unscheduled execution
process.options = cms.untracked.PSet( 
    allowUnscheduled = cms.untracked.bool(True),
    wantSummary      = cms.untracked.bool(False) 
)

# How many events to process
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# Input EDM files
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring([
          'file:/data/submit/wangzqe/darkphoton/AOD/oldDY/aod_1.root'
	])
)

# Load the standard set of configuration modules
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')


# Load the global tag
from Configuration.AlCa.GlobalTag import GlobalTag

# Define the services needed for the treemaker
process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("output.root")
)

# Make tree
process.mmtree = cms.EDAnalyzer('GenTreeMaker',
    	geneventinfo      = cms.InputTag("generator"),
    	gens              = cms.InputTag("genParticles", "", "HLT")
)

# Analysis path
process.p = cms.Path(process.mmtree)

