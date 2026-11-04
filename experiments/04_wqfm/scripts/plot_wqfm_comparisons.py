#!/usr/bin/env python3
"""Recreate wQMC4/wQFM summary charts from the included 60-record table.

The plotted aggregation deliberately follows the original R scripts:
- weight plot: average over the recorded rewiring conditions (40 and 60);
- rewiring plot: average over all recorded weight schemes (1, 2, and 3);
- runtime: raw measured seconds, log10 transformed for box plots.

Source-row 17 rewiring label was corrected to 60 at the author's instruction. These are visualization scripts, not the original
algorithm-execution or dataset-generation scripts.
"""
import argparse
import csv
import math
from collections import defaultdict, Counter
from pathlib import Path

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D

METRICS = [
    ('qdist', 'Quartet agreement (Qdist)', 'Higher is better'),
    ('mast_normalized', 'Normalized MAST', 'Higher is better'),
    ('rf_normalized', 'Normalized RF distance', 'Lower is better'),
    ('tree_distance', 'TreeDistance', 'Lower is better'),
]
WEIGHT_COLORS = {1: '#CA76A3', 2: '#55AFE3', 3: '#E69F00'}
REWIRING_COLORS = {40: '#CA76A3', 60: '#55AFE3'}
METHOD_LS = {'wQFM': '-', 'wQMC4': '--'}


def read_rows(path):
    with open(path, newline='', encoding='utf-8') as f:
        rows = list(csv.DictReader(f))
    assert len(rows) == 60, 'Expected 60 source observations'
    for r in rows:
        for k in ('source_sheet_row', 'n_taxa', 'n_quartets', 'weight_scheme', 'rewiring_percent'):
            r[k] = int(r[k])
        for k in ('runtime_seconds', 'qdist', 'mast_normalized', 'tree_distance', 'rf_normalized', 'log10_quartets_per_taxon'):
            r[k] = float(r[k])
    assert all(r['n_taxa'] == 300 for r in rows)
    k = [(r['algorithm'], r['n_quartets'], r['weight_scheme'], r['rewiring_percent']) for r in rows]
    dup = {key:count for key,count in Counter(k).items() if count != 1}
    assert not dup, f'Duplicate condition(s): {dup}'
    assert len(k) == len(set(k)) == 60, 'Incomplete experimental design grid'
    return rows


def aggregate(rows, group_by):
    out = defaultdict(list)
    for r in rows:
        out[(r['algorithm'], r['n_quartets'], r[group_by])].append(r)
    return {
        key: {m: sum(row[m] for row in group)/len(group) for m,_,_ in METRICS}
        for key,group in out.items()
    }


def make_accuracy(rows, mode, n_panels, output):
    key = 'weight_scheme' if mode == 'weights' else 'rewiring_percent'
    colors = WEIGHT_COLORS if mode == 'weights' else REWIRING_COLORS
    categories = sorted(colors)
    means = aggregate(rows,key)
    metrics = METRICS if n_panels == 4 else METRICS[:2]
    if n_panels == 4:
        fig, axs = plt.subplots(2,2, figsize=(12,8), dpi=140)
        axes = axs.flatten()
    else:
        fig, axes = plt.subplots(1,2,figsize=(12,5), dpi=140)
    for j, (metric,title, direction) in enumerate(metrics):
        ax = axes[j]
        for cat in categories:
            for method in ('wQFM','wQMC4'):
                p = sorted((n,metrics[metric]) for (alg,n,c),metrics in means.items() if alg==method and c==cat)
                ax.plot([r[0] for r in p], [r[1] for r in p], lw=2.6, color=colors[cat],linestyle=METHOD_LS[method])
        ax.grid(True, color='#e1e1e1',lw=.8)
        ax.set_axisbelow(True)
        ax.set_title(f"{'ABCD'[j]}  {title}",loc='left',fontsize=13)
        ax.text(1.0,1.055,direction,transform=ax.transAxes,ha='right',fontsize=10,color='#777777')
        ax.set_xticks([0,25000,50000,75000,100000]);ax.set_xticklabels(['0','25k','50k','75k','100k'])
        ax.spines['top'].set_visible(False);ax.spines['right'].set_visible(False)
        ax.set_xlim(-2000,100000)
    if n_panels==4:
        fig.supxlabel('Number of sampled quartets (NumQ)',fontsize=12,y=.15)
    else:
        fig.supxlabel('Number of sampled quartets (NumQ)',fontsize=12,y=.24)
    cats = ([f'Weight {x} ({x})' for x in categories] if mode=='weights'
            else [f'r = {x/100:.1f}' for x in categories])
    labels = [Line2D([0],[0],color=colors[v],lw=3,label=label) for v,label in zip(categories,cats)]
    methods = [Line2D([0],[0],color='#222222',lw=3,ls=METHOD_LS[method],label=method) for method in ('wQFM','wQMC4')]
    fig.legend(handles=labels,loc='lower center',ncol=len(labels),frameon=False,bbox_to_anchor=(.5,.065 if n_panels==4 else .1),fontsize=11)
    fig.legend(handles=methods,loc='lower center',ncol=2,frameon=False,bbox_to_anchor=(.5,.01 if n_panels==4 else .02),fontsize=11)
    fig.subplots_adjust(bottom=.22 if n_panels==4 else .32,top=.9,hspace=.33,wspace=.24)
    fig.savefig(output,bbox_inches='tight',dpi=140)
    plt.close(fig)


def make_runtime(rows,output):
    levels=sorted(set(r['log10_quartets_per_taxon'] for r in rows))
    fig,ax=plt.subplots(figsize=(9,6),dpi=140)
    x_positions=[];values=[];colors=[]
    for x in levels:
        for offset,alg,col in [(-.18,'wQFM','#CA76A3'),(.18,'wQMC4','#55AFE3')]:
            sample=[math.log10(r['runtime_seconds']) for r in rows if r['log10_quartets_per_taxon']==x and r['algorithm']==alg]
            if sample:
                x_positions.append(x+offset); values.append(sample); colors.append(col)
    boxes=ax.boxplot(values,positions=x_positions,widths=.28,patch_artist=True,manage_ticks=False,medianprops={'color':'#262626','linewidth':2})
    for patch,color in zip(boxes['boxes'],colors):patch.set_facecolor(color)
    ax.set_xticks(levels);ax.set_xticklabels([str(x) for x in levels]);ax.set_xlim(min(levels)-.5,max(levels)+.5)
    ax.set_xlabel('Log10(number of quartets / taxa)')
    ax.set_ylabel('Log10 runtime (seconds)')
    ax.set_title('Comparison of Running Time')
    ax.grid(True,axis='y',color='#e2e2e2',linestyle=':')
    ax.legend([boxes['boxes'][0], boxes['boxes'][1]],['wQFM','wQMC4'],frameon=False)
    fig.tight_layout(); fig.savefig(output,dpi=140);plt.close(fig)


def main():
    root=Path(__file__).resolve().parent.parent
    parser=argparse.ArgumentParser()
    parser.add_argument('--data',type=Path,default=root/'data/wqfm_300_taxa_observations.csv')
    parser.add_argument('--out',type=Path,default=root/'reproduced_figures')
    args=parser.parse_args();args.out.mkdir(parents=True,exist_ok=True)
    rows=read_rows(args.data)
    for mode in ('weights','rewiring'):
        for panels in (2,4):
            make_accuracy(rows,mode,panels,args.out/f'{mode}_{panels}panels.png')
    make_runtime(rows,args.out/'runtime.png')
    print('Created 5 plots from',len(rows),'original observations in',args.out)

if __name__=='__main__': main()
