from ROOT import TFile, TTree
import numpy as np
from sklearn import metrics
import matplotlib.pyplot as plt

infile = TFile.Open("../systematics/tagnprobe_input_full.root", "UPDATE")
upsilon_tree = infile.Get("tpTree/upsilon_tree")
upsilon_tree_adjusted = infile.Get("tpTree/upsilon_tree_adjusted")
bckg_tree = infile.Get("tpTree/bckg_tree")


adjusted_mvas = []
mvas = []
true_data = []

sig_params = []
bckg_params = []

num_signal = 0
num_bckg = 0

cnter = 0
for event in upsilon_tree:
    cnter+=1
    if cnter>100000:
        break

    mvas.append(event.mva2)
    num_signal += 1

    sig_params.append((event.vtxchi2, event.IP))

cnter = 0
for event in upsilon_tree_adjusted:
    cnter+=1
    if cnter>100000:
        break

    adjusted_mvas.append(event.mva2)

cnter = 0
for event in bckg_tree:
    cnter+=1
    if cnter>100000:
        break

    num_bckg += 1
    mvas.append(event.mva2)
    adjusted_mvas.append(event.mva2)

    bckg_params.append((event.vtxchi2, event.IP))


true_data = np.append(np.ones(num_signal), np.zeros(num_bckg))


IP_bins = np.linspace(0, 10, 21)
vtxchi2_bins = np.linspace(0, 10, 21)
hist2d = np.zeros((20, 20))
classical_tpr = 0
classical_fpr = 0
for i in range(len(IP_bins)-1):
    for j in range(len(vtxchi2_bins)-1):
        s=0
        for event in sig_params:
            if (event[0]<vtxchi2_bins[j] and event[1]<IP_bins[i]):
                s+=1
        b=0
        for event in bckg_params:
            if (event[0]<vtxchi2_bins[j] and event[1]<IP_bins[i]):
                b+=1
        if (vtxchi2_bins[j] == 4 and IP_bins[i] == 3.5):
            classical_tpr = s / num_signal
            classical_fpr = b / num_bckg
        hist2d[i][j] = s / np.sqrt(b)


mva_bins = np.linspace(-.4, .2, 20)
hist = np.zeros(20)
for i in range(len(mva_bins)):
    s = 0
    b = 0
    for j in range(len(mvas)):
        if (true_data[j] == 1 and mvas[j]>mva_bins[i]):
            s += 1
        elif (true_data[j] == 0 and mvas[j]>mva_bins[i]):
            b += 1
    hist[i] = s / np.sqrt(b)

imax = np.nanargmax(hist)
label = "Maximum value of " + str(np.round(hist[imax], 2)) + "\n at mva > " + str(np.round(mva_bins[imax], 2))
plt.text(-.35, 200, label)
plt.plot(mva_bins, hist)
plt.xlabel("MVA")
plt.ylabel("s/sqrt(b)")
#plt.show()
plt.savefig("ROCs/mva_sOsqrtb.png")
plt.close()

fpr, tpr, _ = metrics.roc_curve(true_data, mvas)
fpr_adj, tpr_adj, _ = metrics.roc_curve(true_data, adjusted_mvas)
plt.plot(tpr, 1-fpr, label = "vtxchi2_IP regular")
plt.plot(tpr_adj, 1-fpr_adj, label = "vtxchi2_IP adjusted")
plt.plot(classical_tpr, 1-classical_fpr, marker = "o", markersize = 5, label = "classical cut")
plt.xlabel("Signal Efficiency")
plt.ylabel("Background Rejection")
plt.legend()
plt.grid()
#plt.show()
plt.savefig("ROCs/roc_curve.png")
plt.close()

im = plt.imshow(hist2d)
plt.xlabel("vtxchi2")
plt.ylabel("IP")
plt.colorbar(im, label = "frac{s}{sqrt{b}}")

no_xlabels = 10
step_x = int(vtxchi2_bins.shape[0] / (no_xlabels -1))
x_positions = np.arange(0, vtxchi2_bins.shape[0], step_x)
x_labels = vtxchi2_bins[::step_x]
plt.xticks(x_positions, x_labels)

no_ylabels = 10
step_y = int(IP_bins.shape[0] / (no_xlabels - 1))
y_positions = np.arange(0, IP_bins.shape[0], step_y)
y_labels = IP_bins[::step_y]
plt.yticks(y_positions, y_labels)

max_i = np.unravel_index(np.nanargmax(hist2d), hist2d.shape)
label = "Maximum value of "+ str(np.round(hist2d[max_i[0], max_i[1]], 2))+ "\n at vtxchi2<"+ str(vtxchi2_bins[max_i[0]])+ " and IP<"+ str(IP_bins[max_i[1]])
plt.text(2, 18, label)
plt.savefig("ROCs/vtxchi2_IP_sOsqrtb.png")
plt.close()
#plt.show()
