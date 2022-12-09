for i in B C D E F
do
  mkdir split$i
  cp list$i.txt split$i
  cp rename.sh split$i
  cd split$i
  rm split*.txt
  split -l 20 list$i.txt split
  source rename.sh
  cd ..
done
