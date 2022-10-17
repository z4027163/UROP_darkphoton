For Run3 data nTuple production:
```
ssh "you"@lxplus.cern.ch
SCRAM_ARCH=slc7_amd64_gcc700
cmsrel CMSSW_12_4_3
cd CMSSW_12_4_3/src
cmsenv
git clone https://github.com/z4027163/UROP_darkphoton DarkPhotonAnalysis
cd DarkPhotonAnalysis/DimuonAnalysis/
scram b -j 8
cd test
cmsRun scout2022_cfg.py
```
