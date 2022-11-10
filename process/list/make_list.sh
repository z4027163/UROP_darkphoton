#!/bin/bash
#xrdfs xrootd.cmsaf.mit.edu ls /store/user/paus/nanodp/F00/ScoutingPFRun3+Run2022A-v1+RAW | grep ".root" >& listA.txt
xrdfs xrootd.cmsaf.mit.edu ls /store/user/paus/nanodp/F00/ScoutingPFRun3+Run2022B-v1+RAW | grep ".root" >& listB.txt
xrdfs xrootd.cmsaf.mit.edu ls /store/user/paus/nanodp/F00/ScoutingPFRun3+Run2022C-v1+RAW | grep ".root" >& listC.txt
xrdfs xrootd.cmsaf.mit.edu ls /store/user/paus/nanodp/F00/ScoutingPFRun3+Run2022D-v1+RAW | grep ".root" >& listD.txt
xrdfs xrootd.cmsaf.mit.edu ls /store/user/paus/nanodp/F00/ScoutingPFRun3+Run2022E-v1+RAW | grep ".root" >& listE.txt
xrdfs xrootd.cmsaf.mit.edu ls /store/user/paus/nanodp/F00/ScoutingPFRun3+Run2022F-v1+RAW | grep ".root" >& listF.txt
#python skim.py >& list.txt
#split -l$((`wc -l < list.txt`/40)) list.txt list -da 2
