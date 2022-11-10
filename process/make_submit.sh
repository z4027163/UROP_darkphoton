#!/bin/bash
for mark in B C D E F
do
  cp template.sub condor_${mark}.sub
  count=$(ls list/split${mark}/list_*.txt | wc -l)
  sed -i 's/ERA/'${mark}'/g' condor_${mark}.sub
  sed -i 's/NUMBER/'${count}'/g' condor_${mark}.sub
done

sed -i 's/INT/2/g' condor_B.sub
sed -i 's/INT/3/g' condor_C.sub
sed -i 's/INT/4/g' condor_D.sub
sed -i 's/INT/5/g' condor_E.sub
sed -i 's/INT/6/g' condor_F.sub

