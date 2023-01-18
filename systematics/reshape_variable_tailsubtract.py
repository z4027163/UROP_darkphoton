from ROOT import TFile, TTree, TF1
from sklearn.preprocessing import PowerTransformer
from sklearn.preprocessing import QuantileTransformer
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from array import array
from lmfit import Model

#variables_to_reshape = ["trkiso"]
#variables_to_reshape = ["IP", "PVd"]
variables_to_reshape = ["IP"]
tailsubtract = True

infile = TFile.Open("tagnprobe_input_full.root", "UPDATE")
jpsi_tree = infile.Get("tpTree/jpsisig_tree")
upsilon_tree = infile.Get("tpTree/upsilon_tree")

for variable in variables_to_reshape:
    jpsi_tree.SetBranchStatus(variable, 0)
jpsi_tree_adjusted = jpsi_tree.CloneTree()
jpsi_tree_adjusted.SetName("jpsisig_tree_adjusted")
for variable in variables_to_reshape:
    jpsi_tree.SetBranchStatus(variable, 1)

for variable in variables_to_reshape:
    upsilon_tree.SetBranchStatus(variable, 0)
upsilon_tree_adjusted = upsilon_tree.CloneTree()
upsilon_tree_adjusted.SetName("upsilon_tree_adjusted")
for variable in variables_to_reshape:
    upsilon_tree.SetBranchStatus(variable, 1)

for variable in variables_to_reshape:

    if variable == "IP":
        start_fit = 8
        upperbound = 20
    else:
        start_fit = .08
        upperbound = .2

    upsilon_mod_trkiso = array('f', [0])
    jpsi_mod_trkiso = array('f', [0])
    jpsi_mod_trkiso_branch = jpsi_tree_adjusted.Branch(variable, jpsi_mod_trkiso, variable + "/F")
    upsilon_mod_trkiso_branch = upsilon_tree_adjusted.Branch(variable, upsilon_mod_trkiso, variable + "/F")

    jpsi_events = []
    upsilon_events = []

    cnter = 0
    for event in jpsi_tree:
        cnter+=1
        #if cnter > 50000:
        #    break
        exec("jpsi_events.append(event." + variable + ")")
        #jpsi_events.append(event.trkiso)

    cnter = 0
    for event in upsilon_tree:
        cnter+=1
        #if cnter > 50000:
        #    break
        exec("upsilon_events.append(event." + variable + ")")
        #upsilon_events.append(event.trkiso)


    qt1 = QuantileTransformer(
        n_quantiles=500, output_distribution="normal"
    )
    qt2 = QuantileTransformer(
        n_quantiles=500, output_distribution="normal"
    )

    jpsi_events = np.reshape(jpsi_events, (len(jpsi_events), 1))
    upsilon_events = np.reshape(upsilon_events, (len(upsilon_events), 1))

    print(jpsi_events)
    print(upsilon_events)

    if tailsubtract:
        jpsi_hist, bin_edges = np.histogram(jpsi_events, bins = np.linspace(0,upperbound, 100))
        nevents = np.sum(jpsi_hist)
        jpsi_hist = jpsi_hist/nevents
        bin_centers = np.array([(bin_edges[i] + bin_edges[i+1])/2 for i in range(len(bin_edges) - 1)])

        def exp_func(x, a, b):
            return np.exp(a + b*x)

        model = Model(exp_func)
        cutoff_index = np.where(bin_centers>=start_fit)[0][0]
        result = model.fit(jpsi_hist[cutoff_index:], x=bin_centers[cutoff_index:], a=0, b=-1)
        print(result.fit_report())
        for key in result.params:
            print(key, "=", result.params[key].value, "+/-", result.params[key].stderr)

        plt.plot(bin_centers, exp_func(bin_centers, result.params['a'], result.params['b']), label='fit')
        plt.plot(bin_centers, jpsi_hist, label='data')
        plt.axvline(x=start_fit, color = 'b', label = 'fit boundary')
        plt.legend()
        plt.yscale('log')
        plt.xlabel(variable)
        plt.show()

        jpsi_hist_tailsubtract = jpsi_hist - exp_func(bin_centers, result.params['a'], result.params['b'])
        jpsi_hist_tailsubtract = np.where(jpsi_hist_tailsubtract>0, jpsi_hist_tailsubtract, 0)
        jpsi_hist_tailsubtract = jpsi_hist_tailsubtract/np.sum(jpsi_hist_tailsubtract)
        print(jpsi_hist_tailsubtract)
        bin_width = (bin_edges[0] - bin_edges[1])/2
        jpsi_events_tailsubtract = np.random.choice(bin_centers, nevents, p=jpsi_hist_tailsubtract) + np.random.uniform(-bin_width/2, bin_width/2)
        
        jpsi_events_tailsubtract_hist, _ = np.histogram(jpsi_events_tailsubtract, bins = bin_edges)
        plt.plot(bin_centers, jpsi_events_tailsubtract_hist, label = 'psuedoexperiment tailsubtracted')
        plt.plot(bin_centers, jpsi_hist*nevents, label = 'original')
        plt.legend()
        plt.xlabel(variable)
        plt.show()

        jpsi_events_tailsubtract = np.reshape(jpsi_events_tailsubtract, (len(jpsi_events_tailsubtract), 1))
    
    if not tailsubtract :
        jpsi_fit = qt1.fit(jpsi_events)
        upsilon_fit = qt2.fit(upsilon_events)
        upsilon_events_normalized = upsilon_fit.transform(upsilon_events)
        upsilon_events_reshaped = jpsi_fit.inverse_transform(upsilon_events_normalized)
    else:
        jpsi_fit = qt1.fit(jpsi_events_tailsubtract)
        upsilon_fit = qt2.fit(upsilon_events)
        upsilon_events_normalized = upsilon_fit.transform(upsilon_events)
        upsilon_events_reshaped = jpsi_fit.inverse_transform(upsilon_events_normalized)

        jpsi_fit_nontailsubtract = qt1.fit(jpsi_events)
        upsilon_fit = qt2.fit(upsilon_events)
        upsilon_events_normalized_nontailsubtract = upsilon_fit.transform(upsilon_events)
        upsilon_events_reshaped_nontailsubtract = jpsi_fit_nontailsubtract.inverse_transform(upsilon_events_normalized_nontailsubtract)
        
    jpsi_fit = qt1.fit(jpsi_events)
    upsilon_fit = qt2.fit(upsilon_events)
    jpsi_events_normalized = jpsi_fit.transform(jpsi_events)
    jpsi_events_reshaped = upsilon_fit.inverse_transform(jpsi_events_normalized)

    plt.hist(jpsi_events_normalized, bins = np.linspace(-5, 5, 50), label = "jpsi normalized")
    plt.hist(upsilon_events_normalized, bins = np.linspace(-5, 5, 50), label = "upsilon_normalized")
    plt.legend()
    plt.show()
  
    plt.hist(jpsi_events, bins = np.linspace(0, upperbound, 100), label = "original jpsi")
    plt.hist(jpsi_events_reshaped, bins = np.linspace(0, upperbound, 100), label = "reshaped jpsi")
    plt.legend()
    plt.xlabel(variable)
    plt.show()

    if not tailsubtract : 
        plt.hist(upsilon_events, bins = np.linspace(0, upperbound, 100), label = "original upsilon")
        plt.hist(upsilon_events_reshaped, bins = np.linspace(0, upperbound, 100), label = "reshaped upsilon")
        plt.legend()
        plt.xlabel(variable)
        plt.show()
    else : 
        plt.hist(upsilon_events, bins = np.linspace(0, upperbound, 100), label = "original upsilon")
        plt.hist(upsilon_events_reshaped, bins = np.linspace(0, upperbound, 100), label = "reshaped upsilon to tailsubtracted")
        plt.hist(upsilon_events_reshaped_nontailsubtract, bins = np.linspace(0, upperbound, 100), label = "reshaped upsilon to non-tailsubtracted")
        plt.legend()
        plt.xlabel(variable)
        plt.show()


    cnter = 0
    for event in upsilon_tree_adjusted:
        ##print("Old iso: ", event.trkiso)
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
