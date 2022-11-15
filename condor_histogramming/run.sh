#!/bin/bash
RELEASE=CMSSW_12_4_3

source /cvmfs/cms.cern.ch/cmsset_default.sh

if [ -r $RELEASE/src ] ; then
 echo release CMSSW_12_4_3 already exists
else
scram p CMSSW $RELEASE
fi


cd $RELEASE/src
cmsenv
cd ../../

cp /data/submit/cms/store/user/wangzqe/darkphoton/run3_ntuple/$2/tree_$1.root intermediate.root

root -l -q trimscout.C

oname=$2_$1.root
mv output_histogram.root $oname
rm intermediate.root
#$cp $oname /data/submit/juliush/darkphoton/output_histograms/
