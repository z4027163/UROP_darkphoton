import os
import subprocess
import commands
import re
from glob import glob
import sys
from shutil import move
test = False

year = sys.argv[1]
#os.makedirs("combine_output/CL90")

files = glob("output/dpCard_"+year+"_*.txt")
for fname in files:
	m = re.search(r"_m(\d+\.?\d+)_", fname).group(1)
	d = re.search(r"_(\d+).txt", fname).group(1)
	print "ID {0},  m={1}".format(d,m)
	if test and (not int(d)%10==0): continue
	if os.path.isfile(fname):
                os.system("combine -M AsymptoticLimits "+fname+" -m "+m+" -n asympMassIndex_"+d+ " --cminDefaultMinimizerStrategy 0 --rAbsAcc=0.0001 --rRelAcc=0.0001 --setParameters pdf_index_2022=0  --rMin -1 --rMax 1 --cl 0.9")	

outfiles = glob("higgsCombineasympMassIndex_*.root")
for of in outfiles:	
	move(of, "combine_output/CL90/")
