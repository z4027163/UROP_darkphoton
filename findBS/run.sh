#!/bin/bash
RELEASE=CMSSW_12_4_3

source /cvmfs/cms.cern.ch/cmsset_default.sh

if [ -r $RELEASE/src ] ; then
 echo release CMSSW_12_4_3 already exists
else
scram p CMSSW $RELEASE
fi


cd $RELEASE/src
eval `scram runtime -sh`
cd ../../


#1 job ID, #2 era, #3 era int
idx=0
while IFS= read -r line
do
    xrdcp root://submit50.mit.edu//$line input.root
    root -l -b -q findBS.C\(\"input.root\"\,\"output_$idx.root\"\)
    idx=$((idx +1))
    rm input.root
done < list_$1.txt

hadd hist_$1.root output_*.root

xrdcp -f hist_$1.root root://submit50.mit.edu//store/user/wangzqe/darkphoton/run3_BS/$2/
