cp list$1.txt split$1
cp rename.sh split$1
cd split$1
rm split*.txt
split -l 20 list$1.txt split
source rename.sh
cd ..
