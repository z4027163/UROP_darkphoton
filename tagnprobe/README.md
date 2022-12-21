To run fitting code:

```
cmsrel CMSSW_9_4_0_pre3
cd CMSSW_9_4_0_pre3/src/
cmsenv
mkdir -p MuonAnalysis/TagAndProbe
git clone http://www.github.com/cms-analysis/MuonAnalysis-TagAndProbe.git MuonAnalysis/TagAndProbe -b 92X
cmsenv
scram b -j8
git clone https://github.com/fmanteca/TnP_MuonEfficiencies

cd TnP_MuonEfficiencies
cp ../../../fit2Muon_upsilon.py .
cp ../../../fit2Muon_jpsi.py .
```

Then run the fitting code in the `TnP_MuonEfficiencies` folder using:

```
cmsRun fit2Muon_upsilon.py
``` 
