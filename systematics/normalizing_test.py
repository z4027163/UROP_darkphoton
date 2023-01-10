from sklearn.preprocessing import PowerTransformer
from sklearn.preprocessing import QuantileTransformer
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

bc = PowerTransformer(method="box-cox")
yj = PowerTransformer(method='yeo-johnson')
qt1 = QuantileTransformer(
    n_quantiles=500, output_distribution="normal"
)
qt2 = QuantileTransformer(
    n_quantiles=500, output_distribution="normal"
)

def f1(x):
    return np.exp(-2*x)

def f2(x): 
    return np.exp(-4*x)

n_samples1 = 10000
n_samples2 = 12000
n_bins = 20

x = np.linspace(0, 10, 1000)
pdf1 = f1(x)/np.sum(f1(x))
pdf2 = f2(x)/np.sum(f2(x))

y1 = np.random.choice(x, n_samples1, p=pdf1)
y2 = np.random.choice(x, n_samples2, p=pdf2)



hist1, _ = np.histogram(y1, bins = np.linspace(0,5, n_bins))
hist2, bin_edges = np.histogram(y2, bins = np.linspace(0,5, n_bins))
bin_centers = [(bin_edges[i] + bin_edges[i+1])/2 for i in range(len(bin_edges) - 1)]
plt.plot(bin_centers, hist1, label="y1 hist")
plt.plot(bin_centers, hist2, label="y2 hist")
plt.legend()
plt.savefig("raw_data.png")
plt.show()

y1 = np.reshape(y1, (len(y1), 1))
y2 = np.reshape(y2, (len(y2), 1))
y1 = pd.DataFrame(y1)
y2 = pd.DataFrame(y2)
print(y1)

fit1 = qt1.fit(y1)
fit2 = qt2.fit(y2)
y1_normalized = fit1.transform(y1)
y2_normalized = fit2.transform(y2)
y2_reshaped = fit1.inverse_transform(y2_normalized)
#for i in range(len(y2)):
#    print(y2[i])
#    print(y2_reshaped[i])
#    print()

hist1_norm, bin_edges = np.histogram(y1_normalized, bins = np.linspace(-3,3,n_bins))
hist2_norm, bin_edges = np.histogram(y2_normalized, bins = np.linspace(-3,3,n_bins))
bin_centers = [(bin_edges[i] + bin_edges[i+1])/2 for i in range(len(bin_edges) - 1)]
plt.plot(bin_centers, hist1_norm, label="y1 normalized hist")
plt.plot(bin_centers, hist2_norm, label="y2 normalized hist")
plt.legend()
plt.savefig("normalized_data.png")
plt.show()

hist1, _ = np.histogram(y1, bins = np.linspace(0,5, n_bins))
hist2, _ = np.histogram(y2, bins = np.linspace(0,5, n_bins))
hist3, bin_edges = np.histogram(y2_reshaped, bins = np.linspace(0,5, n_bins))
bin_centers = [(bin_edges[i] + bin_edges[i+1])/2 for i in range(len(bin_edges) - 1)]
plt.plot(bin_centers, hist1, label = "y1 hist")
plt.plot(bin_centers, hist2, label = "y2 hist")
plt.plot(bin_centers, hist3, label = "y2 reshaped hist")
plt.legend()
plt.savefig("transformed_data.png")
plt.show()

