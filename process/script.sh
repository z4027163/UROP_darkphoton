for i in *.sub;
do
  condor_submit $i
done
