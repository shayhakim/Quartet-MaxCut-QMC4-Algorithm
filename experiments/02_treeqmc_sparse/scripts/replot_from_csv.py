#!/usr/bin/env python3
"""Replot saved final QMC4/TREE-QMC benchmark values; no algorithms run.
Figures in figures/ are the archived manuscript figures. The newly reproduced
plots are written to figures/reproduced/ and may differ in graphical styling.
Requires Python 3 and matplotlib.
"""
import csv
from pathlib import Path
from collections import defaultdict
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

ROOT=Path(__file__).resolve().parent.parent
RES=ROOT/'results'; OUT=ROOT/'figures/reproduced'; OUT.mkdir(parents=True,exist_ok=True)
METHODS=[('qmc4','QMC4','#2368ae'),('treeqmc_n0','TREE-QMC n0','#c66719'),('treeqmc_n2','TREE-QMC n2','#12836d')]
MVALS=[1000,3162,10000,31623,100000]

def load(n):
 with (RES/n).open(newline='') as f:return list(csv.DictReader(f))

q=load('qdist_n1000.csv'); o=load('other_metrics_n1000.csv'); runtimes=load('runtime_successful_runs_n1000.csv')
assert len(q)==193 and len(o)==579 and len(runtimes)==193
metrics=[('qdist','Qdist agreement','higher'),('rf_normalized','Normalized RF','lower'),('mast_fraction','Normalized MAST','higher'),('tree_distance_raw','TreeDistance','lower')]
G=defaultdict(list)
for r in q: G[('qdist',int(r['m']),r['method'])].append(float(r['qdist']))
for r in o: G[(r['metric'],int(r['m']),r['method'])].append(float(r['value']))
fig,axes=plt.subplots(2,2,figsize=(12,8.5))
for ax,(metric,label,_),ix in zip(axes.flat,metrics,range(4)):
 for i,m in enumerate(MVALS):
  for j,(method,name,color) in enumerate(METHODS):
   values=G[(metric,m,method)]
   if not values: continue
   ax.boxplot(values,positions=[i+(j-1)*0.23],widths=0.20,patch_artist=True,manage_ticks=False,showmeans=True,
     boxprops=dict(facecolor=color,edgecolor=color,alpha=.3), medianprops=dict(color=color),
     whiskerprops=dict(color=color),capprops=dict(color=color),meanprops=dict(marker='D',markersize=3,markerfacecolor='white',markeredgecolor=color))
 ax.set_xticks(range(5),['0','0.5','1','1.5','2']);ax.set_title('ABCD'[ix]+'. '+label)
 ax.set_xlabel('log10(m / n)');ax.grid(axis='y',alpha=.2)
from matplotlib.patches import Patch
fig.legend(handles=[Patch(facecolor=c,label=n) for _,n,c in METHODS],loc='lower center',ncol=3)
fig.tight_layout(rect=[0,.05,1,1]);fig.savefig(OUT/'accuracy_four_metrics.png',dpi=180);plt.close(fig)

mapping={'QMC4':'qmc4','TREE-QMC n0':'treeqmc_n0','TREE-QMC n2':'treeqmc_n2'}
T=defaultdict(list)
for r in runtimes:T[(int(r['quartets']),mapping[r['method']])].append(float(r['runtime_seconds']))
fig,ax=plt.subplots(figsize=(10,5.8))
for i,m in enumerate(MVALS):
 for j,(method,name,color) in enumerate(METHODS):
  values=T[(m,method)]
  if not values:continue
  ax.boxplot(values,positions=[i+(j-1)*.23],widths=.19,patch_artist=True,manage_ticks=False,showmeans=True,
      boxprops=dict(facecolor=color,edgecolor=color,alpha=.32),medianprops=dict(color=color),
      whiskerprops=dict(color=color),capprops=dict(color=color),meanprops=dict(marker='D',markersize=4,markerfacecolor='white',markeredgecolor=color))
ax.set_xticks(range(5),['0','0.5','1','1.5','2']);ax.set_yscale('log');ax.set_xlabel('log10(m / n)');ax.set_ylabel('Runtime (seconds)')
ax.grid(axis='y',alpha=.2);ax.legend(handles=[Patch(facecolor=c,label=n) for _,n,c in METHODS],loc='upper center',bbox_to_anchor=(.5,-.15),ncol=3)
fig.tight_layout();fig.savefig(OUT/'runtime.png',dpi=180,bbox_inches='tight');plt.close(fig)
print('Wrote:', OUT/'accuracy_four_metrics.png', 'and', OUT/'runtime.png')
