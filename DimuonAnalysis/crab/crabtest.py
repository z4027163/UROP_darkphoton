from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'darkphoton_2018_lpc2'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../test/scout2018_cfg.py'

config.Data.inputDataset   = '/DarkPhoton/gandreas-ZdToMuMu-DY_AOD-6cb0500311fa5ccd686a80db73673208/USER'
#config.Data.inputDataset   = '/DarkPhoton/gandreas-ZdToMuMu-DY_AOD-6cb0500311fa5ccd686a80db73673208/USER'
#'/JpsiToMuMu_JpsiPt8_TuneCP5_13TeV-pythia8/RunIISpring18DRPremix-TkAlJPsiToMuMu_100X_upgrade2018_realistic_v10-v1/AODSIM'
#config.Data.splitting      = 'LumiBased'
config.Data.splitting      = 'FileBased'
config.Data.unitsPerJob    = 100
#config.Data.outLFNDirBase = '/store/user/wangz/ntuple_darkphoton/2018/'
config.Data.outLFNDirBase = '/store/user/wangz/darkphoton/2018/'

config.Data.inputDBS = 'phys03'
#config.Data.inputDBS = 'global'
config.Data.outputDatasetTag = 'darkphoton_2018'

config.JobType.allowUndistributedCMSSW = True
config.Site.storageSite = 'T3_US_FNALLPC'
#config.Site.storageSite    = 'T2_CH_CERNBOX'
