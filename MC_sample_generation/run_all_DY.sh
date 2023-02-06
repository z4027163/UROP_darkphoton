#!/bin/bash

if [ -r CMSSW_12_4_3/src ] ; then
 echo release CMSSW_12_4_3 already exists
else
scram p CMSSW CMSSW_12_4_3
fi
cd CMSSW_12_4_3/src
eval `scram runtime -sh`
cd ../../

RAND_SEED=$(($1+1234567))
echo ${RAND_SEED}

OUTPATH=/data/submit/juliush/darkphoton/MC_production
FOUTPUT=scout_$1.root

NEVENTS=400;
NTHREADS=1;

echo "Choose random PU input file."
cp /work/submit/juliush/UROP_darkphoton/test/CMSSW_12_4_3/src/UROP_darkphoton/MC_sample_generation/pulist.txt .
PULIST=($(cat pulist.txt))
PUFILE=${PULIST[$RANDOM % ${#PULIST[@]}]}
echo "Chose PU File: ${PUFILE}"


cp /work/submit/juliush/UROP_darkphoton/test/CMSSW_12_4_3/src/UROP_darkphoton/MC_sample_generation/DY_cfg.py .
sed -i 's/RAND_SEED/'${RAND_SEED}'/g' DY_cfg.py
sed -i 's/NTHREADS/'${NTHREADS}'/g' DY_cfg.py
sed -i 's/NEVENTS/'${NEVENTS}'/g' DY_cfg.py
cmsRun DY_cfg.py


cmsDriver.py \
--python_filename step2_cfg.py \
--eventcontent PREMIXRAW \
--customise Configuration/DataProcessing/Utils.addMonitoring \
--datatier GEN-SIM-RAW \
--fileout file:genSimRaw.root \
--pileup_input $PUFILE \
--conditions 124X_mcRun3_2022_realistic_v12 \
--step DIGI,DATAMIX,L1,DIGI2RAW,HLT:2022v12 \
--procModifiers premix_stage2,siPixelQualityRawToDigi \
--nThreads ${NTHREADS} \
--geometry DB:Extended \
--filein file:DY.root \
--datamix PreMix \
--era Run3 \
--no_exec \
--mc \
-n ${NEVENTS} || exit $? ;

cmsRun step2_cfg.py

cmsDriver.py \
--python_filename step3_cfg.py \
--eventcontent AODSIM \
--customise Configuration/DataProcessing/Utils.addMonitoring \
--datatier AODSIM \
--fileout file:scout_AOD.root \
--conditions 124X_mcRun3_2022_realistic_v12 \
--step RAW2DIGI,L1Reco,RECO,RECOSIM \
--nThreads ${NTHREADS} \
--geometry DB:Extended \
--filein file:genSimRaw.root \
--era Run3 \
--no_exec \
--mc \
-n ${NEVENTS} || exit $? ;

cmsRun step3_cfg.py



cd CMSSW_12_4_3/src/
mkdir darkphoton
cd darkphoton
cp -r /work/submit/juliush/UROP_darkphoton/test/CMSSW_12_4_3/src/UROP_darkphoton/DimuonAnalysis .
cd ..
eval `scram runtime -sh`
scram b -j8
cd darkphoton/DimuonAnalysis/test

cmsRun scout2022_cfg.py

mv scout_output.root ../../../../../output_$1.root

cd ../../../../../



#rm *.py
#rm *.root
