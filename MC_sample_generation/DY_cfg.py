# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/PPD-Run3Summer22GS-00005-fragment.py --python_filename PPD-Run3Summer22GS-00005_1_cfg.py --eventcontent RAWSIM --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM --fileout file:PPD-Run3Summer22GS-00005.root --conditions 124X_mcRun3_2022_realistic_v12 --beamspot Realistic25ns13p6TeVEarly2022Collision --step GEN,SIM --geometry DB:Extended --era Run3 --no_exec --mc -n 432
import FWCore.ParameterSet.Config as cms
import sys

from Configuration.Eras.Era_Run3_cff import Run3

print("Starting Gen-level production:")
print("------------------------------")
print("Number of events = ", NEVENTS)
print("Number of threads = ", NTHREADS)
print("Random seed = ", RAND_SEED)
print("------------------------------")

process = cms.Process('SIM',Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic25ns13p6TeVEarly2022Collision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(NEVENTS),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

from FWCore.ParameterSet.VarParsing import VarParsing
params = VarParsing('analysis')
params.register('seed',RAND_SEED,VarParsing.multiplicity.singleton,VarParsing.varType.int,'the random seed')
params.parseArguments()
process.RandomNumberGeneratorService.generator.initialSeed = params.seed


# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    accelerators = cms.untracked.vstring('*'),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    deleteNonConsumedUnscheduledModules = cms.untracked.bool(True),
    dumpOptions = cms.untracked.bool(False),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(
            allowAnyLabel_=cms.required.untracked.uint32
        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(0)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(0),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('Configuration/GenProduction/python/PPD-Run3Summer22GS-00005-fragment.py nevts:432'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(1),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(20971520),
    fileName = cms.untracked.string('file:DY.root'),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
if hasattr(process, "XMLFromDBSource"): process.XMLFromDBSource.label="Extended"
if hasattr(process, "DDDetectorESProducerFromDB"): process.DDDetectorESProducerFromDB.label="Extended"
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '124X_mcRun3_2022_realistic_v12', '')

process.generator = cms.EDFilter("Pythia8GeneratorFilter",
    PythiaParameters = cms.PSet(
        parameterSets = cms.vstring(
            'pythia8CommonSettings',
            'pythia8CP5Settings',
            'processParameters'
        ),
        processParameters = cms.vstring(
            #'HardQCD:all = on',
            #'PhaseSpace:pTHatMin = 15  ',
            #'PhaseSpace:pTHatMax = 7000'
            'WeakSingleBoson:ffbar2ffbar(s:gmZ)= on',
             '23:onMode = off',
             '23:onIfAny = 13'
        ),
        pythia8CP5Settings = cms.vstring(
            'Tune:pp 14',
            'Tune:ee 7',
            #'MultipartonInteractions:ecmPow=0.03344',
            #'MultipartonInteractions:bProfile=2',
            #'MultipartonInteractions:pT0Ref=1.41',
            #'MultipartonInteractions:coreRadius=0.7634',
            #'MultipartonInteractions:coreFraction=0.63',
            #'ColourReconnection:range=5.176',
            #'SigmaTotal:zeroAXB=off',
            #'SpaceShower:alphaSorder=2',
            #'SpaceShower:alphaSvalue=0.118',
            #'SigmaProcess:alphaSvalue=0.118',
            #'SigmaProcess:alphaSorder=2',
            #'MultipartonInteractions:alphaSvalue=0.118',
            #'MultipartonInteractions:alphaSorder=2',
            #'TimeShower:alphaSorder=2',
            #'TimeShower:alphaSvalue=0.118',
            #'SigmaTotal:mode = 0',
            #'SigmaTotal:sigmaEl = 22.08',
            #'SigmaTotal:sigmaTot = 101.037',
            #'PDF:pSet=LHAPDF6:NNPDF31_nnlo_as_0118'
        ),
        pythia8CommonSettings = cms.vstring(
            #'Tune:preferLHAPDF = 2',
            #'Main:timesAllowErrors = 10000',
            #'Check:epTolErr = 0.01',
            #'Beams:setProductionScalesFromLHEF = off',
            #'SLHA:minMassSM = 1000.',
            #'ParticleDecays:limitTau0 = on',
            #'ParticleDecays:tau0Max = 10',
            #'ParticleDecays:allowPhotonRadiation = on'


            'Tune:preferLHAPDF = 2',
            'Main:timesAllowErrors = 10000',
            'Check:epTolErr = 0.01',
            'Beams:setProductionScalesFromLHEF = off',
            #'SLHA:keepSM = on',
            'SLHA:minMassSM = 1000.' #new change
            'ParticleDecays:limitTau0 = off',
            'PhaseSpace:mHatMin = 0.05',
            'PhaseSpace:mHatMax = 60',
            'ParticleDecays:allowPhotonRadiation = on'
        )
    ),
    comEnergy = cms.double(13600.0),
    filterEfficiency = cms.untracked.double(0), #changed
    maxEventsToPrint = cms.untracked.int32(0), #changed
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    pythiaPylistVerbosity = cms.untracked.int32(0), #changed
    #reweightGenEmp = cms.PSet(
    #    tune = cms.string('CP5')
    #)
    crossSection = cms.untracked.double(0), #added
)




process.massgenfilter = cms.EDFilter("MCParticlePairFilter",
    Status      = cms.untracked.vint32(1, 1),
    MinPt       = cms.untracked.vdouble(3., 3.),
    MaxEta      = cms.untracked.vdouble(3,3),
    MinEta      = cms.untracked.vdouble(-3, -3),
    ParticleCharge = cms.untracked.int32(-1),
    ParticleID1 = cms.untracked.vint32(13),
    ParticleID2 = cms.untracked.vint32(13),
    MinInvMass = cms.untracked.double(1.0),
    MaxInvMass = cms.untracked.double(50.0),
   # MotherID   = cms.untracked.vint32(23,23)
)


process.ProductionFilterSequence = cms.Sequence(process.generator+process.massgenfilter)



# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)
# filter all path with the production filter sequence
for path in process.paths:
	#getattr(process,path).insert(0, process.generator)
        getattr(process,path).insert(0, process.ProductionFilterSequence)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# End of customisation functions


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
