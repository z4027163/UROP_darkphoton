for i in B C D E F
do
    ls split$i/list_*.txt | wc -l
done
