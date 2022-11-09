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


#1 input, 2 output 3,era 4.era int
xrdcp root://xrootd.cmsaf.mit.edu//$1 input.root

root -l -b -q generateTree.C\(\"input.root\"\,\"tree_$2.root\"\,$4\)

xrdcp -f tree_$2.root root://submit50.mit.edu//store/user/wangzqe/darkphoton/run3/ntuple/$3/
