#!/bin/bash
xrdfs submit50.mit.edu ls /store/user/wangzqe/darkphoton/run3_ntuple/2022B | grep ".root" >& listB.txt
xrdfs submit50.mit.edu ls /store/user/wangzqe/darkphoton/run3_ntuple/2022C | grep ".root" >& listC.txt
xrdfs submit50.mit.edu ls /store/user/wangzqe/darkphoton/run3_ntuple/2022D | grep ".root" >& listD.txt
xrdfs submit50.mit.edu ls /store/user/wangzqe/darkphoton/run3_ntuple/2022E | grep ".root" >& listE.txt
xrdfs submit50.mit.edu ls /store/user/wangzqe/darkphoton/run3_ntuple/2022F | grep ".root" >& listF.txt
source make_split.sh
#split -l$((`wc -l < list.txt`/40)) list.txt list -da 2
