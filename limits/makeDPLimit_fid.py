import ROOT
import os,sys
from ROOT import TGraphAsymmErrors
from ROOT import TGraphErrors
from ROOT import TColor
#from ROOT import TGraph
from array import array
from ROOT import *
from operator import truediv
import random
import math
from glob import glob
import re 
import sys
from math import sqrt

year = sys.argv[1]

fff = open("eps2.txt", "w")
#fff2 = open("eps2_obs.txt", "w")


limit1=array('d')
limiteps2=array('d')
limit190=array('d')
limiteps290=array('d')
limit195up=array('d')
limit195down=array('d')
limit168up=array('d')
limit168down=array('d')
limit1Observed=array('d')

limit2=array('d')
limit2eps2=array('d')
limit290=array('d')
limit2eps290=array('d')
limit295up=array('d')
limit295down=array('d')
limit268up=array('d')
limit268down=array('d')
limit2Observed=array('d')

limit3=array('d')
limit3eps2=array('d')
limit390=array('d')
limit3eps290=array('d')
limit395up=array('d')
limit395down=array('d')
limit368up=array('d')
limit368down=array('d')
limit3Observed=array('d')


#for theoretical
limit1Observed_up=array('d')
limit1Observed_dow=array('d')
limit2Observed_up=array('d')
limit2Observed_dow=array('d')
limit3Observed_up=array('d')
limit3Observed_dow=array('d')

limit1Observed_up_rel=array('d')
limit1Observed_dow_rel=array('d')
limit2Observed_up_rel=array('d')
limit2Observed_dow_rel=array('d')
limit3Observed_up_rel=array('d')
limit3Observed_dow_rel=array('d')

mass1=array('d')
mass2=array('d')
mass3=array('d')
masserr1=array('d')
masserr2=array('d')
masserr3=array('d')

if year == "2018":
        lumi = 61.3
 #       lumi = 6.6
 #       lumi = 1.1


if year == "2017":
        lumi = 35.3

if year == "bothYears":
        lumi = 96.6

if year == "CL90":
        lumi = 96.6
if year == "2022":
	lumi = 14.31
#lumi_project = 96.6
lumi_project = 14.31


#THEO FID CROSS SECTION FOR EPS=0.02
m= array('d',[1.1,1.3,1.5,1.7,1.9,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,15,20,25, 30, 35, 40, 45, 50, 55])
xSec= array('d',[386.7, 312.7, 264.1, 227.2, 197.8, 142.3, 92.74, 68.51, 55.09, 48.8, 46.85, 63.94, 78.23, 47.51, 21.52, 10.44, 5.60, 3.44, 3.19, 2.187, 1.502, 1.106, 0.8205])
#xSec = array('d',[90.54,110.3,115.1,113,108.6,88.71,64.29,50.42,41.3,36.64,36.18,70.71,49.75,32.34,5.017])
#theoretical up/down
#xSec_dow= array('d',[14.1,17.52,22.11,26.53,32.19,45.46,47.92,41,35.83,32.54,32.5,35.79,32.88,32.54,5.125])
#xSec_up=array('d',[225.9,194.7,177.3,155.4,140.9,105.4,70.21,51.88,42.92,37.83,36,50.96,63.33,42.5,5.017])

xsecgraph = TGraph(len(m),m,xSec);
#xsecgraph_up = TGraph(len(m),m,xSec_up);
#xsecgraph_dow = TGraph(len(m),m,xSec_dow);

eps2scale = 1.
base_eps = 0.02 #epsilon for which the cross sections are computed

a = (base_eps**2)*eps2scale/sqrt(lumi_project/lumi) # for lumi projection (6.6->100)

files = glob("combine_output/"+year+"/higgsCombineasympMassIndex_*.AsymptoticLimits.mH*.root")

d_m = {}
for fname in files:
        m = float(re.search(r"mH(\d+\.?\d+).root", fname).group(1))
        d = int(re.search(r"Index_(\d+).Asymptotic", fname).group(1))
        d_m[d] = [m, fname]


d_m = sorted(d_m.items())
print d_m

for d,m_fname in d_m:
    m, fname = m_fname
    #if (m == 1.716 or m ==7.632):
    #        continue
    #file90=glob.glob("higgsCombineIterV9_CL90_ForPress_2018_"+str(d)+".AsymptoticLimits.mH*.root")
    acc=1
    xsec = xsecgraph.Eval(m,0,"S")

    xsec_up=xsec*1.312
    xsec_dow=xsec*(1-0.325)
   
    #xsec_up = xsecgraph_up.Eval(m,0,"S")
    #xsec_dow = xsecgraph_dow.Eval(m,0,"S")
    unc_up=0.312
    unc_dow=0.325

    if (m < 3):
            f=ROOT.TFile.Open(fname)
            tree=f.Get("limit")
            tree.GetEntry(2)
            limit1.append(tree.limit*a/(acc*xsec))
            a_exp=tree.limit*a/(acc*xsec)
             
            tree.GetEntry(0)
            limit195up.append(abs(tree.limit*a/(acc*xsec)-limit1[-1]))
            tree=f.Get("limit")
            tree.GetEntry(4)
            limit195down.append(abs(tree.limit*a/(acc*xsec)-limit1[-1]))
            
            
            tree.GetEntry(1)
            limit168up.append(abs(tree.limit*a/(acc*xsec)-limit1[-1]))
            tree=f.Get("limit")
            tree.GetEntry(3)
            limit168down.append(abs(tree.limit*a/(acc*xsec)-limit1[-1]))

            tree.GetEntry(5)
            limit1Observed.append(abs(tree.limit*a/(acc*xsec)))
            a_obs=tree.limit*a/(acc*xsec)            
           
            limit1Observed_up.append(abs(tree.limit*a/(acc*xsec_up)))
            limit1Observed_dow.append(abs(tree.limit*a/(acc*xsec_dow))) 
            limit1Observed_up_rel.append(abs(tree.limit*a/(acc*xsec)*unc_up))
            limit1Observed_dow_rel.append(abs(tree.limit*a/(acc*xsec)*unc_dow))

            tree=f.Get("limit")
            mass1.append(m)
            masserr1.append(0.)
#            fff.write("{0} {1}\n".format(m, math.sqrt(tree.limit*a)))
            fff.write("{0} {1}\n".format(m, a_exp))
#            fff.write("{0} {1}\n".format(m, a_obs))
            
    if (m > 3 and  m < 9):
            f=ROOT.TFile.Open(fname)
            tree=f.Get("limit")
            tree.GetEntry(2)
            limit2.append(tree.limit*a/(acc*xsec))
            a_exp=tree.limit*a/(acc*xsec)

            tree.GetEntry(0)
            limit295up.append(abs(tree.limit*a/(acc*xsec)-limit2[-1]))
            tree=f.Get("limit")
            tree.GetEntry(4)
            limit295down.append(abs(tree.limit*a/(acc*xsec)-limit2[-1]))
            
            
            tree.GetEntry(1)
            limit268up.append(abs(tree.limit*a/(acc*xsec)-limit2[-1]))
            tree=f.Get("limit")
            tree.GetEntry(3)
            limit268down.append(abs(tree.limit*a/(acc*xsec)-limit2[-1]))
            
            tree.GetEntry(5)
            limit2Observed.append(abs(tree.limit*a/(acc*xsec)))
            a_obs=tree.limit*a/(acc*xsec)           

            limit2Observed_up.append(abs(tree.limit*a/(acc*xsec_up)))
            limit2Observed_dow.append(abs(tree.limit*a/(acc*xsec_dow)))

            limit2Observed_up_rel.append(abs(tree.limit*a/(acc*xsec)*unc_up))
            limit2Observed_dow_rel.append(abs(tree.limit*a/(acc*xsec)*unc_dow))

 
            tree=f.Get("limit")
            mass2.append(m)
            masserr2.append(0.)
#            fff.write("{0} {1}\n".format(m, math.sqrt(tree.limit*a)))
            fff.write("{0} {1}\n".format(m, a_exp))
#            fff.write("{0} {1}\n".format(m, a_obs))   

    if (m > 9):
            f=ROOT.TFile.Open(fname)
            tree=f.Get("limit")
            tree.GetEntry(2)
            limit3.append(tree.limit*a/(acc*xsec))
            a_exp=tree.limit*a/(acc*xsec)

            tree.GetEntry(0)
            limit395up.append(abs(tree.limit*a/(acc*xsec)-limit3[-1]))
            tree=f.Get("limit")
            tree.GetEntry(4)
            limit395down.append(abs(tree.limit*a/(acc*xsec)-limit3[-1]))


            tree.GetEntry(1)
            limit368up.append(abs(tree.limit*a/(acc*xsec)-limit3[-1]))
            tree=f.Get("limit")
            tree.GetEntry(3)
            limit368down.append(abs(tree.limit*a/(acc*xsec)-limit3[-1]))

            tree.GetEntry(5)
            limit3Observed.append(abs(tree.limit*a/(acc*xsec)))
            a_obs=tree.limit*a/(acc*xsec)

            limit3Observed_up.append(abs(tree.limit*a/(acc*xsec_up)))
            limit3Observed_dow.append(abs(tree.limit*a/(acc*xsec_dow)))

            limit3Observed_up_rel.append(abs(tree.limit*a/(acc*xsec)*unc_up))
            limit3Observed_dow_rel.append(abs(tree.limit*a/(acc*xsec)*unc_dow))


            tree=f.Get("limit")
            mass3.append(m)
            masserr3.append(0.)
#            fff.write("{0} {1}\n".format(m, math.sqrt(tree.limit*a)))
            fff.write("{0} {1}\n".format(m, a_exp))
#            fff.write("{0} {1}\n".format(m, a_obs))


c1=ROOT.TCanvas("c1","c1",700,500)
#c1.SetGrid()
c1.SetLogy()
c1.SetLogx()

mg=ROOT.TMultiGraph()
mgeps=ROOT.TMultiGraph()
graph_limit1=ROOT.TGraph(len(mass1),mass1,limit1)
graph_limit1.SetTitle("graph_limit1")
graph_limit1.SetMarkerSize(0)
#graph_limit1.SetMarkerStyle(20)
#graph_limit1.SetMarkerColor(kBlack)
#graph_limit1.SetLineWidth(2)
#graph_limit1.SetLineStyle(7)
graph_limit1.SetLineColor(kRed+3)
graph_limit1.SetLineWidth(2)
graph_limit1.SetLineStyle(2)
graph_limit1.GetYaxis().SetRangeUser(0,100)
graph_limit1.GetXaxis().SetRangeUser(10,70)
graph_limit1.GetXaxis().SetMoreLogLabels()
graph_limit1.GetYaxis().SetTitle("#sigma(pp#rightarrow A)#times BR(A#rightarrow #mu#mu)[pb]")
graph_limit1.GetYaxis().SetTitleSize(2)
graph_limit1.GetXaxis().SetTitle("Dark Photon Mass [GeV]")

graph_limit2=ROOT.TGraph(len(mass2),mass2,limit2)
graph_limit2.SetTitle("graph_limit2")
graph_limit2.SetMarkerSize(0)
#graph_limit2.SetMarkerStyle(20)
#graph_limit2.SetMarkerColor(kBlack)
#graph_limit2.SetLineWidth(2)
#graph_limit2.SetLineStyle(7)
graph_limit2.SetLineColor(kRed+3)
graph_limit2.SetLineWidth(2)
graph_limit2.SetLineStyle(2)

graph_limit3=ROOT.TGraph(len(mass3),mass3,limit3)
graph_limit3.SetTitle("graph_limit2")
graph_limit3.SetMarkerSize(0)
#graph_limit3.SetMarkerStyle(20)
#graph_limit3.SetMarkerColor(kBlack)
#graph_limit3.SetLineWidth(2)
#graph_limit3.SetLineStyle(7)
graph_limit3.SetLineColor(kRed+3)
graph_limit3.SetLineWidth(2)
graph_limit3.SetLineStyle(2)

#graph_limit=ROOT.TGraph(len(mass),mass,limitObserved)
#graph_limit.Draw("same")
graph_limit195up=ROOT.TGraphAsymmErrors(len(mass1),mass1,limit1,masserr1,masserr1,limit195up,limit195down)
graph_limit195up.SetTitle("graph_limit195up")
graph_limit195up.SetFillColor(ROOT.TColor.GetColor(252,241,15))

graph_limit295up=ROOT.TGraphAsymmErrors(len(mass2),mass2,limit2,masserr2,masserr2,limit295up,limit295down)
graph_limit295up.SetTitle("graph_limit295up")
graph_limit295up.SetFillColor(ROOT.TColor.GetColor(252,241,15))

graph_limit395up=ROOT.TGraphAsymmErrors(len(mass3),mass3,limit3,masserr3,masserr3,limit395up,limit395down)
graph_limit395up.SetTitle("graph_limit395up")
graph_limit395up.SetFillColor(ROOT.TColor.GetColor(252,241,15))



graph_limit168up=ROOT.TGraphAsymmErrors(len(mass1),mass1,limit1,masserr1,masserr1,limit168up,limit168down)
graph_limit168up.SetTitle("graph_limit168up")
graph_limit168up.SetMarkerColor(kGreen)
graph_limit168up.SetFillColor(kGreen);

graph_limit268up=ROOT.TGraphAsymmErrors(len(mass2),mass2,limit2,masserr2,masserr2,limit268up,limit268down)
graph_limit268up.SetTitle("graph_limit268up")
graph_limit268up.SetMarkerColor(kGreen)
graph_limit268up.SetFillColor(kGreen);

graph_limit368up=ROOT.TGraphAsymmErrors(len(mass3),mass3,limit3,masserr3,masserr3,limit368up,limit368down)
graph_limit368up.SetTitle("graph_limit368up")
graph_limit368up.SetMarkerColor(kGreen)
graph_limit368up.SetFillColor(kGreen);

graph_limit1_unc=ROOT.TGraphAsymmErrors(len(mass1),mass1,limit1Observed,masserr1,masserr1,limit1Observed_up_rel,limit1Observed_dow_rel)
graph_limit1_unc.SetTitle("graph_limit1_unc")
graph_limit1_unc.SetFillColorAlpha(15, 0.7);
#graph_limit1_unc.SetLineWidth(-802);
#graph_limit1_unc.SetLineColor(15)

graph_limit2_unc=ROOT.TGraphAsymmErrors(len(mass2),mass2,limit2Observed,masserr2,masserr2,limit2Observed_up_rel,limit2Observed_dow_rel)
graph_limit2_unc.SetTitle("graph_limit2_unc")
#graph_limit2_unc.SetFillColor(ROOT.TColor.GetColor(15))
graph_limit2_unc.SetFillColorAlpha(15, 0.7);
#graph_limit1_unc.SetLineColor(15)

graph_limit3_unc=ROOT.TGraphAsymmErrors(len(mass3),mass3,limit3Observed,masserr3,masserr3,limit3Observed_up_rel,limit3Observed_dow_rel)
graph_limit3_unc.SetTitle("graph_limit3_unc")
#graph_limit3_unc.SetFillColor(ROOT.TColor.GetColor(15))
graph_limit3_unc.SetFillColorAlpha(15, 0.7);
#graph_limit1_unc.SetLineColor(15)

graph_limitObs1=ROOT.TGraph(len(mass1),mass1,limit1Observed)
graph_limitObs1.SetTitle("graph_limitObs1")
graph_limitObs1.SetMarkerSize(1)
graph_limitObs1.SetMarkerStyle(1)
graph_limitObs1.SetMarkerColor(kBlack)
graph_limitObs1.SetLineWidth(2)
graph_limitObs1.SetLineStyle(1)

graph_limitObs2=ROOT.TGraph(len(mass2),mass2,limit2Observed)
graph_limitObs2.SetTitle("graph_limitObs2")
graph_limitObs2.SetMarkerSize(1)
graph_limitObs2.SetMarkerStyle(1)
graph_limitObs2.SetMarkerColor(kBlack)
graph_limitObs2.SetLineWidth(2)
graph_limitObs2.SetLineStyle(1)

graph_limitObs3=ROOT.TGraph(len(mass3),mass3,limit3Observed)
graph_limitObs3.SetTitle("graph_limitObs3")
graph_limitObs3.SetMarkerSize(1)
graph_limitObs3.SetMarkerStyle(1)
graph_limitObs3.SetMarkerColor(kBlack)
graph_limitObs3.SetLineWidth(2)
graph_limitObs3.SetLineStyle(1)

#theory up
col=16
tem_t=1;
graph_limitObs1_up=ROOT.TGraph(len(mass1),mass1,limit1Observed_up)
graph_limitObs1_up.SetTitle("graph_limitObs1_up")
graph_limitObs1_up.SetMarkerSize(1)
graph_limitObs1_up.SetMarkerStyle(1)
graph_limitObs1_up.SetLineColor(col)
graph_limitObs1_up.SetMarkerColor(col)
graph_limitObs1_up.SetLineWidth(1)
graph_limitObs1_up.SetLineStyle(tem_t)

graph_limitObs2_up=ROOT.TGraph(len(mass2),mass2,limit2Observed_up)
graph_limitObs2_up.SetTitle("graph_limitObs2_up")
graph_limitObs2_up.SetMarkerSize(1)
graph_limitObs2_up.SetMarkerStyle(1)
graph_limitObs2_up.SetLineColor(col)
graph_limitObs2_up.SetMarkerColor(col)
graph_limitObs2_up.SetLineWidth(1)
graph_limitObs2_up.SetLineStyle(tem_t)

graph_limitObs3_up=ROOT.TGraph(len(mass3),mass3,limit3Observed_up)
graph_limitObs3_up.SetTitle("graph_limitObs3_up")
graph_limitObs3_up.SetMarkerSize(1)
graph_limitObs3_up.SetMarkerStyle(1)
graph_limitObs3_up.SetLineColor(col)
graph_limitObs3_up.SetMarkerColor(col)
graph_limitObs3_up.SetLineWidth(1)
graph_limitObs3_up.SetLineStyle(tem_t)

#theory down
graph_limitObs1_dow=ROOT.TGraph(len(mass1),mass1,limit1Observed_dow)
graph_limitObs1_dow.SetTitle("graph_limitObs1_dow")
graph_limitObs1_dow.SetMarkerSize(1)
graph_limitObs1_dow.SetLineStyle(1)
graph_limitObs1_dow.SetLineColor(col)
graph_limitObs1_dow.SetMarkerColor(col)
graph_limitObs1_dow.SetLineWidth(1)
graph_limitObs1_dow.SetLineStyle(tem_t)

graph_limitObs2_dow=ROOT.TGraph(len(mass2),mass2,limit2Observed_dow)
graph_limitObs2_dow.SetTitle("graph_limitObs2_obs")
graph_limitObs2_dow.SetMarkerSize(1)
graph_limitObs2_dow.SetMarkerStyle(1)
graph_limitObs2_dow.SetLineColor(col)
graph_limitObs2_dow.SetMarkerColor(col)
graph_limitObs2_dow.SetLineWidth(1)
graph_limitObs2_dow.SetLineStyle(tem_t)

graph_limitObs3_dow=ROOT.TGraph(len(mass3),mass3,limit3Observed_dow)
graph_limitObs3_dow.SetTitle("graph_limitObs3_obs")
graph_limitObs3_dow.SetMarkerSize(1)
graph_limitObs3_dow.SetMarkerStyle(1)
graph_limitObs3_dow.SetLineColor(col)
graph_limitObs3_dow.SetMarkerColor(col)
graph_limitObs3_dow.SetLineWidth(1)
graph_limitObs3_dow.SetLineStyle(tem_t)

#graph_limit268up=ROOT.TGraphAsymmErrors(len(mass2),mass2,limit2,masserr2,masserr2,limit268up,limit268down)

#graph_limit268up.SetTitle("graph_limit268up")
#graph_limit268up.SetFillColor(kGreen);
#graph_limit268up.SetMarkerColor(kGreen)

mg.Add(graph_limit195up,"3")
mg.Add(graph_limit168up,"3")
#mg.Add(graph_limit1_unc,"3")
mg.Add(graph_limit1,"l")
#mg.Add(graph_limitObs1,"l")
mg.Add(graph_limit295up,"3")
mg.Add(graph_limit268up,"3")
#mg.Add(graph_limit2_unc,"3")
mg.Add(graph_limit2,"l")
#mg.Add(graph_limitObs2,"l")
mg.Add(graph_limit395up,"3")
mg.Add(graph_limit368up,"3")
#mg.Add(graph_limit3_unc,"3")
mg.Add(graph_limit3,"l")
#mg.Add(graph_limitObs3,"l")


mg.Draw("APC")
mg.GetXaxis().SetRangeUser(1,40)
mg.GetYaxis().SetRangeUser(5e-8,1e-3)
#mg.GetYaxis().SetTitle("xSec*BR [pb]")
#mg.GetXaxis().SetTitle("Dark Photon Mass [GeV]")
mg.GetYaxis().SetTitle("#epsilon^{2}")
mg.GetYaxis().SetTitleOffset(0.9)
mg.GetYaxis().SetTitleSize(0.05)
mg.GetXaxis().SetTitle("Dark Photon Mass [GeV]")
mg.GetXaxis().SetTitleSize(0.05)
mg.GetXaxis().SetMoreLogLabels()
c1.Update()
legend=ROOT.TLegend(0.5,0.6,0.8,0.9)
cmsTag=ROOT.TLatex(0.13,0.917,"#scale[1.1]{CMS}")
cmsTag.SetNDC()
cmsTag.SetTextAlign(11)
cmsTag.Draw()
cmsTag2=ROOT.TLatex(0.215,0.917,"#scale[0.825]{#bf{#it{Preliminary}}}")
cmsTag2.SetNDC()
cmsTag2.SetTextAlign(11)
#cmsTag.SetTextFont(61)
cmsTag2.Draw()
cmsTag3=ROOT.TLatex(0.90,0.917,"#scale[0.9]{#bf{"+str(14.31)+" fb^{-1} (13.6 TeV)}}")
cmsTag3.SetNDC()
cmsTag3.SetTextAlign(31)
#cmsTag.SetTextFont(61)
cmsTag3.Draw()
leg=ROOT.TLegend(0.65, 0.65,0.85, 0.85)  
leg.SetBorderSize( 0 )
leg.SetFillStyle( 1001 )
leg.SetFillColor(kWhite) 
leg.AddEntry( graph_limitObs1 , "Observed",  "LP" )
leg.AddEntry( graph_limit1 , "Expected",  "L" )
leg.AddEntry( graph_limit168up, "#pm 1#sigma",  "F" ) 
leg.AddEntry( graph_limit195up, "#pm 2#sigma",  "F" ) 
#leg.AddEntry( graph_limit1_unc, "theoretical #pm 1#sigma",  "F" )

leg.Draw("same")
c1.SaveAs("limit"+year+"DarkPhoton_eps2_fidturbo.root")
c1.SaveAs("limit"+year+"DarkPhoton_eps2_fidturbo.pdf")
c1.SaveAs("limit"+year+"DarkPhoton_eps2_fidturbo.png")
c2=ROOT.TCanvas("c2","c2",700,500)
c2.SetLogy()
cmsTag.Draw()
cmsTag2.Draw()
cmsTag3.Draw()
mgeps.Draw("APC")
leg2=ROOT.TLegend(0.65, 0.65,0.8, 0.85)  
leg2.SetBorderSize( 0 )
leg2.SetFillStyle( 1001 )
leg2.SetFillColor(kWhite) 

leg2.Draw("same")
c2.SaveAs("thep.root")

