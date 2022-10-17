from CRABClient.UserUtilities import config
config = config()

config.General.requestName = 'ntuple_1GeV_2'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = '../test/scout2018_cfg.py'

config.Data.userInputFiles = open('list1GeV.txt').readlines() 
config.Data.splitting      = 'FileBased'
config.Data.unitsPerJob    = 1
config.Data.outLFNDirBase = '/store/user/wangz/darkphoton/2018/'

#config.User.voRole = 'wangz'
config.Data.outputPrimaryDataset = 'ntuple_1GeV_2'
config.JobType.allowUndistributedCMSSW = True
config.Site.storageSite = 'T3_US_FNALLPC'
config.Site.whitelist = ['T2_CH_CERN']
