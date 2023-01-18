### runs generate tree for all eras in parallell ###
### note that generateTree_tagprobe.C only uses 5% of events ###

root -l -q generateTree_tagprobe.C\(2\)  &>log_era2&
root -l -q generateTree_tagprobe.C\(3\)  &>log_era3&
root -l -q generateTree_tagprobe.C\(5\)  &>log_era5&
root -l -q generateTree_tagprobe.C\(6\)  &>log_era6&
