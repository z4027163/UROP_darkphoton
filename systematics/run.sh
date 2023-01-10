root -l -q generateTree_tagprobe.C

python3 reshape_variable.py

root -l -q add_mva.C\(\"tagnprobe_input_era5.root\",\"jpsisig_tree_adjusted\",\"mva2\"\)
root -l -q add_mva.C\(\"tagnprobe_input_era5.root\",\"jpsisig_tree\",\"mva2\"\)

root -l -q add_mva.C\(\"tagnprobe_input_era5.root\",\"upsilon_tree_adjusted\",\"mva2\"\)
root -l -q add_mva.C\(\"tagnprobe_input_era5.root\",\"upsilon_tree\",\"mva2\"\)
