for i in condor_*.sub;
do
  condor_submit $i
done
