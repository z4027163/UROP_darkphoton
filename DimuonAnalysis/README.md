How to produce limit plots (assuming the user already ran the mass calibration part):

1) compile:
<pre><code>
cmsrel CMSSW_10_3_2   (This needs to be updated) check http://cms-analysis.github.io/HiggsAnalysis-CombinedLimit/ 
cd CMSSW_10_3_2/src/
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cmsenv
scram b
git clone git@github.com:cosbyc/DarkPhotonLimits.git
cd DarkPhotonAnalysisLimits/DimuonAnalysis2017/
make
</code></pre>

2) perform and store the fit to data
<pre><code>
cd limits
./makeCardsAndWS.exe
</code></pre>

3) run the combine tool, computing the mass-dependent limit
<pre><code>
python pyDPLimitsProcessing.py <year>
</code></pre>

4) run the plotting macro
<pre><code>
python makeDPLimit_Xsec.py <year>
</code></pre>
for the limit on the cross section times BR
or

<pre><code>
python makeDPLimit_Xsec.py <year>
</code></pre>
for the limit on eps^2. (ATTENTION: the cross sections used in this macro correspond to a specific model, to be checked!)

