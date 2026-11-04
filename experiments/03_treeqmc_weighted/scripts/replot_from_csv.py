#!/usr/bin/env python3
"""Replot the archived 54 complete matched QMC4/TREE-QMC weighted results.
Archived manuscript figures are kept unchanged in figures/. This script
writes independent CSV-derived reproductions to figures/reproduced/.
Requires Python 3 and matplotlib.
"""
import csv, math, statistics
from pathlib import Path
from collections import defaultdict
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import Patch

ROOT=Path(__file__).resolve().parent.parent
RES=ROOT/'results';OUT=ROOT/'figures/reproduced';OUT.mkdir(parents=True,exist_ok=True)
METHODS=[('qmc4','QMC4','#2368ae'),('treeqmc_n0','TREE-QMC n0','#27956c'),('treeqmc_n2','TREE-QMC n2','#d27b29')]
MODES=[('uniform','Unweighted','-'),('w1','W1 weighted','--')]
MS=[31623,100000,316228]

def load(fn):
 with (RES/fn).open(newline='') as f:return list(csv.DictReader(f))
a=load('weighted_accuracy_extended.csv');t=load('weighted_runtime_54runs.csv')
assert len(a)==len(t)==54
assert {(x['replicate'],x['m'],x['tool'],x['mode']) for x in a}=={(x['replicate'],x['m'],x['tool'],x['mode']) for x in t}
METRICS=[('qdist','Qdist'),('rf_normalized','Normalized RF'),('mast_fraction','Normalized MAST'),('tree_distance_raw','TreeDistance')]
fig,axes=plt.subplots(2,2,figsize=(11,8.5))
for ax,(metric,title) in zip(axes.flat,METRICS):
 for method,name,color in METHODS:
  for mode,mlabel,linestyle in MODES:
   mean_vals=[];sds=[]
   for m in MS:
    xs=[float(r[metric]) for r in a if r['tool']==method and r['mode']==mode and int(r['m'])==m]
    assert len(xs)==3
    mean_vals.append(statistics.mean(xs));sds.append(statistics.stdev(xs))
   ax.errorbar([1.5,2.,2.5],mean_vals,yerr=sds,fmt='o',linestyle=linestyle,color=color,capsize=2,linewidth=1.5,markersize=3)
 ax.set_title(title);ax.set_xlabel('log10(m / n)');ax.set_xticks([1.5,2.,2.5]);ax.grid(alpha=.2)
fig.legend(handles=[Patch(facecolor=c,label=n) for _,n,c in METHODS]+[plt.Line2D([0],[0],color='black',linestyle=ls,label=label) for _,label,ls in MODES],loc='lower center',ncol=5)
fig.tight_layout(rect=[0,.07,1,1]);fig.savefig(OUT/'weighted_accuracy_four_metrics.png',dpi=180);plt.close(fig)
G=defaultdict(list)
for row in t:G[(int(row['m']),row['tool'],row['mode'])].append(float(row['runtime_seconds']))
fig,ax=plt.subplots(figsize=(10,5.8))
for i,m in enumerate(MS):
 for j,(method,name,color) in enumerate(METHODS):
  for mode,_,_ in MODES:
   values=G[(m,method,mode)]; assert len(values)==3
   pos=i+(j-1)*.24+(-.055 if mode=='uniform' else .055)
   ax.boxplot(values,positions=[pos],widths=.095,patch_artist=True,showmeans=True,manage_ticks=False,
      boxprops=dict(facecolor=color,edgecolor=color,alpha=.45,hatch='///' if mode=='w1' else ''),
      medianprops=dict(color='#222'),whiskerprops=dict(color=color),capprops=dict(color=color),
      meanprops=dict(marker='D',markersize=4,markerfacecolor='white',markeredgecolor=color))
ax.set_yscale('log');ax.set_xticks(range(3),['1.5','2','2.5']);ax.set_xlabel('log10(m / n)');ax.set_ylabel('Runtime (seconds)');ax.grid(axis='y',alpha=.2)
fig.legend(handles=[Patch(facecolor=c,label=n) for _,n,c in METHODS]+[Patch(facecolor='#777',label='Unweighted'),Patch(facecolor='#777',hatch='///',label='W1 weighted')],loc='lower center',ncol=5)
fig.tight_layout(rect=[0,.07,1,1]);fig.savefig(OUT/'weighted_runtime.png',dpi=180);plt.close(fig)
print('Wrote:', OUT/'weighted_accuracy_four_metrics.png', 'and', OUT/'weighted_runtime.png')
