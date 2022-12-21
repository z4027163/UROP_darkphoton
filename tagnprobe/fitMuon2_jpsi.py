import FWCore.ParameterSet.Config as cms
import sys, os, shutil
from optparse import OptionParser


def FillNumDen(num, den):

    if num == "mvaloose":
        process.TnP_MuonID.Expressions.MvaLooseVar = cms.vstring("MvaLooseVar", "mva2>-0.1", "mva2")
        process.TnP_MuonID.Cuts.MvaLooseCutid  = cms.vstring("MvaLooseCutid", "MvaLooseVar", "0.5")

    if den == "looseid":
        process.TnP_MuonID.Categories.CutBasedIdLoose  = cms.vstring("PassLooseid", "dummy[pass=1,fail=0]")

def FillVariables(par):

    if par == 'pt_dR':
        process.TnP_MuonID.Variables.pt  = cms.vstring("muon p_{T}", "0", "1000", "GeV/c")
        process.TnP_MuonID.Variables.dR  = cms.vstring("deltaR", "0", "2", "")

    if par == 'pt':
        process.TnP_MuonID.Variables.pt  = cms.vstring("muon p_{T}", "0", "1000", "GeV/c")

    if par == 'pair_deltaR':
        process.TnP_MuonID.Variables.pair_deltaR  = cms.vstring("deltaR", "0", "4", "")

def FillBin(par):

    if par == 'pt':
#        DEN.pt = cms.vdouble(20, 25, 30, 40, 50, 60, 120, 200)
        DEN.pt = cms.vdouble( 3.0,4.0, 5.0, 6.0, 10.0, 12.0,15.0, 20.0, 30.0, 100)
    elif par == 'pair_deltaR':
        DEN.pair_deltaR = cms.vdouble(0., 0.4, 0.8, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 5.0)
    elif par == 'pt_dR':
        DEN.pt = cms.vdouble( 3.0, 4.0, 5.0, 6.0, 10.0, 12.0,15.0, 20.0, 30.0, 100)
        DEN.dR = cms.vdouble(0., 0.2,0.35,0.5, 3)
       # DEN.dR = cms.vdouble(0., 0.5, 0.7 , 0.9, 1.2 , 5.0)

    if den == "gentrack": pass
    elif den == "trackermuons": DEN.TM = cms.vstring("pass") #For low pT ID efficiencies
    elif den == "looseid": DEN.CutBasedIdLoose = cms.vstring("pass")
    elif den == "tightPVd": DEN.PVd = cms.vdouble(0, 0.015)
    elif den == "tightIP": DEN.IP = cms.vdouble(0,3.5)

### USAGE: cmsRun fitMuonID.py TEST tight loose mc mc_all

#_*_*_*_*_*_*_*_*_*_*_*_*
#Set the num, den, par, etc.
#_*_*_*_*_*_*_*_*_*_*_*_*
iteration = ''
num = 'mvaloose'
den = 'tightPVd'
par = 'pt_dR'
#par = 'pt'
sample = 'dataid_test'
scenario = 'data_all'
shape = cms.vstring("voigtPlusExpo")

process = cms.Process("TagProbe")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.source = cms.Source("EmptySource")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100000) )


#_*_*_*_*_*_*_*_*_*_*_*_*
#Prepare variables, den, num and fit funct
#_*_*_*_*_*_*_*_*_*_*_*_*

#Set-up the mass lower edge

# For systematic uncertainties


mass_ =" mass"


# nbins variation for systematics
if 'nbins_up' in sample:
    nbins = 50
elif 'nbins_down' in sample:
    nbins = 30
else:
    # nominal case
    nbins = 40

Template = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
                          SplitMode = cms.uint32(500000),
                          NumCPU = cms.uint32(1),
    SaveWorkspace = cms.bool(False),


    Variables = cms.PSet(
        mass = cms.vstring("Tag-muon Mass", "2.8", "3.35", "GeV/c^{2}"),
        mva2 = cms.vstring("mva id", "-0.8","0.4",""),
        PVd = cms.vstring("PVd", "0","0.2",""),
        #essential for all den/num
        IP = cms.vstring("IP","0","10",""),


        ),

    Categories = cms.PSet(),
    Expressions = cms.PSet(),
    Cuts = cms.PSet(), 
    PDFs = cms.PSet(
        voigtPlusExpo = cms.vstring(
            "Voigtian::signal(mass, mean[3.1,3.0,3.2], width[0.05,0.01,0.10], sigma[0.05,0.01,0.1])".replace("mass",mass_),
            "Exponential::backgroundPass(mass, lp[0,-5,5])".replace("mass",mass_),
            "Exponential::backgroundFail(mass, lf[0,-5,5])".replace("mass",mass_),
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        )
    ),
    
    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(nbins),
    saveDistributionsPlot = cms.bool(False),
    Efficiencies = cms.PSet(), # will be filled later
)



#if sample == "dataid_test":
#    process.TnP_MuonID = Template.clone(
#       InputFileNames = cms.vstring(
#                '/data/submit/wangzqe/darkphoton/processed/hist/tnp_mva_2018_tree.root'
#            ),
#        InputTreeName = cms.string("fitter_tree"),
#        InputDirectoryName = cms.string("tpTree"),
#        OutputFileName = cms.string("TnP_MuonISO_%s.root" % scenario),
#        Efficiencies = cms.PSet(),
#        )

if sample == "dataid_test":
    process.TnP_MuonID = Template.clone(
       InputFileNames = cms.vstring(
                '/work/submit/juliush/UROP_darkphoton/test/CMSSW_12_4_3/src/UROP_darkphoton/tagnprobe/input.root'
            ),
        InputTreeName = cms.string("sig_tree"),
        InputDirectoryName = cms.string("tpTree"),
        OutputFileName = cms.string("TnP_MuonISO_%s.root" % scenario),
        Efficiencies = cms.PSet(),
        )


print("made it here")
BIN = cms.PSet()

Num_dic = {'looseid':'LooseID', 'mvaIPloose':'MVADrLoose', 'mvaloose':'MVALoose'}
Den_dic = {'gentrack':'genTracks','looseid':'LooseID','tightPVd':'tightPVd','tightIP':'tightIP'}
Sel_dic = {'looseid':'LooseCutid','mvaIPloose':'MvaDrLooseCutid', 'mvaloose':'MvaLooseCutid'}


FillVariables(par)
FillNumDen(num, den)

ID_BINS = [(Sel_dic[num],("NUM_%s_DEN_%s_PAR_%s"%(Num_dic[num],Den_dic[den],par),BIN))]


for ID, ALLBINS in ID_BINS:
    X = ALLBINS[0]
    B = ALLBINS[1]
    _output = os.getcwd() + '/Efficiency' + iteration
    if not os.path.exists(_output):
        print ('Creating', '/Efficiency' + iteration,', the directory where the fits are stored.')
        os.makedirs(_output)
    if scenario == 'data_all':
        _output += '/DATA' + '_' + sample
    if not os.path.exists(_output):
        os.makedirs(_output)
    module = process.TnP_MuonID.clone(OutputFileName = cms.string(_output + "/TnP_MC_%s.root" % (X)))
    #save the fitconfig in the plot directory
    shutil.copyfile(os.getcwd()+'/fitMuon2_jpsi.py',_output+'/fitMuon2_jpsi.py')

    DEN = B.clone(); num_ = ID;
    FillBin(par)
    mass_variable = "mass"


    if scenario == 'data_all':
        print("here")
        if num_.find("Iso4") != -1 or num_.find("Iso3") != -1:
            setattr(module.Efficiencies, ID+"_"+X, cms.PSet(
                EfficiencyCategoryAndState = cms.vstring(num_,"below"),
                UnbinnedVariables = cms.vstring(mass_variable),
                BinnedVariables = DEN,
                BinToPDFmap = shape
                ))
        else:
            setattr(module.Efficiencies, ID+"_"+X, cms.PSet(
                EfficiencyCategoryAndState = cms.vstring(num_,"above"),
                UnbinnedVariables = cms.vstring(mass_variable),
                BinnedVariables = DEN,
                BinToPDFmap = shape
                ))
        setattr(process, "TnP_MuonID_"+ID+"_"+X, module)
        setattr(process, "run_"+ID+"_"+X, cms.Path(module))
        if num_.find("puppiIso") != -1:
            setattr(module.Efficiencies, ID+"_"+X, cms.PSet(
                    EfficiencyCategoryAndState = cms.vstring(num_,"below"),
                    UnbinnedVariables = cms.vstring(mass_variable),
                    BinnedVariables = DEN,
                    BinToPDFmap = shape
                    ))

print("end of file")





