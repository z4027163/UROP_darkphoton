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
    xrdcp root://xrootd.cmsaf.mit.edu//$line input.root
    root -l -b -q generateTree.C\(\"input.root\"\,\"output_$idx.root\"\,$3\)
    idx=$((idx +1))
    rm input.root
done < list_$1.txt

hadd tree_$1.root output_*.root

tar -xzvf TMVA-v4.2.0.tgz
source TMVA-v4.2.0/test/setup.sh

root -l -b -q add_mva.C\(\"tree_$1.root\"\)
xrdcp -f tree_$1.root root://submit50.mit.edu//store/user/wangzqe/darkphoton/run3/ntuple/$2/
