#root -l -q generateTree_tagprobe.C

python3 reshape_variable_tailsubtract.py

root -l -q add_mva_vtx.C\(\"tagnprobe_input_full.root\",\"jpsisig_tree_adjusted\",\"mva2\"\)
root -l -q add_mva_vtx.C\(\"tagnprobe_input_full.root\",\"jpsisig_tree\",\"mva2\"\)

root -l -q add_mva_vtx.C\(\"tagnprobe_input_full.root\",\"upsilon_tree_adjusted\",\"mva2\"\)
root -l -q add_mva_vtx.C\(\"tagnprobe_input_full.root\",\"upsilon_tree\",\"mva2\"\)

root -l -q add_mva_vtx.C\(\"tagnprobe_input_full.root\",\"bckg_tree\",\"mva2\"\)
