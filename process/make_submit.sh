cp list/list*.txt .
sed -e 's/$/ $(Process) 2022B 2/' -i listB.txt
sed -e 's/$/ $(Process) 2022C 3/' -i listC.txt
sed -e 's/$/ $(Process) 2022D 4/' -i listD.txt
sed -e 's/$/ $(Process) 2022E 5/' -i listE.txt
sed -e 's/$/ $(Process) 2022F 6/' -i listF.txt

cat p1.txt listB.txt p2.txt >& condor_B.sub
cat p1.txt listC.txt p2.txt >& condor_C.sub
cat p1.txt listD.txt p2.txt >& condor_D.sub
cat p1.txt listE.txt p2.txt >& condor_E.sub
cat p1.txt listF.txt p2.txt >& condor_F.sub

rm list*.txt
