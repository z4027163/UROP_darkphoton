from ROOT import TFile, TTree
from sklearn.preprocessing import PowerTransformer
from sklearn.preprocessing import QuantileTransformer
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from array import array

infile = TFile.Open("tagnprobe_input_era5.root", "UPDATE")
jpsi_tree = infile.Get("tpTree/jpsisig_tree")
upsilon_tree = infile.Get("tpTree/upsilon_tree")

jpsi_tree.SetBranchStatus("trkiso", 0)
jpsi_tree_adjusted = jpsi_tree.CloneTree()
jpsi_tree_adjusted.SetName("jpsisig_tree_adjusted")
jpsi_tree.SetBranchStatus("trkiso", 1)

upsilon_tree.SetBranchStatus("trkiso", 0)
upsilon_tree_adjusted = upsilon_tree.CloneTree()
upsilon_tree_adjusted.SetName("upsilon_tree_adjusted")
upsilon_tree.SetBranchStatus("trkiso", 1)

upsilon_mod_trkiso = array('f', [0])
jpsi_mod_trkiso = array('f', [0])
jpsi_mod_trkiso_branch = jpsi_tree_adjusted.Branch("trkiso", jpsi_mod_trkiso, "trkiso/F")
upsilon_mod_trkiso_branch = upsilon_tree_adjusted.Branch("trkiso", upsilon_mod_trkiso, "trkiso/F")

jpsi_events = []
upsilon_events = []

for event in jpsi_tree:
    jpsi_events.append(event.trkiso)

for event in upsilon_tree:
    upsilon_events.append(event.trkiso)


qt1 = QuantileTransformer(
    n_quantiles=500, output_distribution="normal"
)
qt2 = QuantileTransformer(
    n_quantiles=500, output_distribution="normal"
)

jpsi_events = np.reshape(jpsi_events, (len(jpsi_events), 1))
upsilon_events = np.reshape(upsilon_events, (len(upsilon_events), 1))

jpsi_fit = qt1.fit(jpsi_events)
upsilon_fit = qt2.fit(upsilon_events)
upsilon_events_normalized = upsilon_fit.transform(upsilon_events)
upsilon_events_reshaped = jpsi_fit.inverse_transform(upsilon_events_normalized)
jpsi_events_normalized = jpsi_fit.transform(jpsi_events)
jpsi_events_reshaped = upsilon_fit.inverse_transform(jpsi_events_normalized)

cnter = 0
for event in upsilon_tree_adjusted:
    #print("Old iso: ", event.trkiso)
    #print("New iso: ", upsilon_events_reshaped[cnter])
    #print()

    upsilon_mod_trkiso[0] = upsilon_events_reshaped[cnter][0]
    cnter+=1

    upsilon_mod_trkiso_branch.Fill()
    
    #event.trkiso = upsilon_events_reshaped[cnter][0]
    #cnter += 1
    #upsilon_tree_adjusted.Fill()

cnter = 0
for event in jpsi_tree_adjusted:
    jpsi_mod_trkiso[0] = jpsi_events_reshaped[cnter][0]
    cnter += 1

    jpsi_mod_trkiso_branch.Fill()

    #event.trkiso = upsilon_events_reshaped[cnter][0]
    #cnter += 1
    #jpsi_tree_adjusted.Fill()

infile.cd("tpTree")
upsilon_tree_adjusted.Write()
jpsi_tree_adjusted.Write()
infile.Close()
