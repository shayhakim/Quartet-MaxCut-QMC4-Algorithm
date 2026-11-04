#!/usr/bin/env python3
"""Make equivalent (not pixel-identical) plots from the curated CSV.

Requires matplotlib.  Original manuscript figure PNGs are preserved
separately in figures/ without modification.
"""
import csv
from collections import defaultdict
from pathlib import Path
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.patches import Patch

root=Path(__file__).resolve().parents[1]
with (root/'data/accuracy_by_replicate.csv').open(newline='',encoding='utf-8') as f:
    rows=list(csv.DictReader(f))
groups=defaultdict(list)
for r in rows:
    groups[(int(r['gene_trees']),r['method'],r['setting'])].append(r)
series=[('QMC4','default','QMC4'),
        ('ASTRAL-II','p0_halfresolved','ASTRAL-II p0, halfresolved'),
        ('ASTRAL-II','p0_true-halfresolved','ASTRAL-II p0, true halfresolved'),
        ('ASTRAL-II','p1_halfresolved','ASTRAL-II p1, halfresolved'),
        ('ASTRAL-II','p1_true-halfresolved','ASTRAL-II p1, true halfresolved')]
colors=plt.rcParams['axes.prop_cycle'].by_key()['color'][:5]

def panel(ax, metric, title, ylabel='Score'):
    positions=[]; data=[]; color_sequence=[]
    for idx,n in enumerate((50,200,1000)):
        for j,(method,setting,label) in enumerate(series):
            group=groups[(n,method,setting)]
            assert group, (n,method,setting)
            positions.append(idx*7+j)
            data.append([float(v[metric]) for v in group])
            color_sequence.append(colors[j])
    bp=ax.boxplot(data,positions=positions,widths=.75,patch_artist=True,manage_ticks=False,
                  showfliers=True,medianprops=dict(color='black',linewidth=1.5),
                  flierprops=dict(marker='o',markersize=3,markerfacecolor='none'))
    for b,c in zip(bp['boxes'],color_sequence):
        b.set_facecolor(c);b.set_alpha(.65)
    ax.set_xticks([2,9,16],['50','200','1,000'])
    ax.set_xlabel('Number of gene trees');ax.set_ylabel(ylabel)
    ax.set_title(title,fontsize=12,fontweight='bold');ax.grid(axis='y',alpha=.25)
    ax.set_xlim(-1.5,19.5)

legend=[Patch(facecolor=c,alpha=.65,label=label) for c,(_,_,label) in zip(colors,series)]
output=root/'reproduced_figures';output.mkdir(parents=True,exist_ok=True)
fig,axes=plt.subplots(1,2,figsize=(15,5.5))
panel(axes[0],'qdist','Quartet agreement (Qdist)')
panel(axes[1],'mast_normalized','Normalized MAST')
fig.legend(handles=legend,loc='lower center',ncol=3,fontsize=9)
fig.subplots_adjust(left=.06,right=.985,top=.9,bottom=.28,wspace=.2)
fig.savefig(output/'astral_qdist_mast_reproduced.png',dpi=170)
plt.close(fig)
fig,axes=plt.subplots(2,2,figsize=(15,9))
panel(axes[0,0],'qdist','Quartet agreement (Qdist)')
panel(axes[0,1],'mast_normalized','Normalized MAST')
panel(axes[1,0],'tree_distance','TreeDistance')
panel(axes[1,1],'rf_normalized','Normalized RF distance')
fig.legend(handles=legend,loc='lower center',ncol=3,fontsize=9)
fig.subplots_adjust(left=.06,right=.985,top=.94,bottom=.15,wspace=.22,hspace=.45)
fig.savefig(output/'astral_all_four_metrics_reproduced.png',dpi=170)
plt.close(fig)
print('Wrote two regenerated plots to',output)
