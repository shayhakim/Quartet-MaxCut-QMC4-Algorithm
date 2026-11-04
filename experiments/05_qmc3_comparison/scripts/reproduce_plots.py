#!/usr/bin/env python3
"""Replot the deposited paired gains, without running either phylogeny algorithm."""
import argparse
import csv
from collections import defaultdict
from pathlib import Path
from statistics import mean, stdev

HERE=Path(__file__).resolve().parents[1]
METRICS=[('qdist','A  Qdist',1),('rf_normalized','B  Normalized RF',-1),('mast_fraction','C  Normalized MAST',1),('tree_distance_raw','D  TreeDistance',-1)]
PLOT_ORDER=['QMC','W1','W1_RW20']
LABELS={'QMC':'Unweighted','W1':'W1 (oracle-weighted)','W1_RW20':'W1 + rewiring'}

def plot(name,out):
    import matplotlib.pyplot as plt
    with (HERE/'results'/name).open(newline='',encoding='utf-8') as f:
        rows=list(csv.DictReader(f))
    groups=defaultdict(list)
    for r in rows:
        groups[r['condition']].append(r)
    fig,axes=plt.subplots(2,2,figsize=(13,9),constrained_layout=True)
    for ax,(metric,title,sign) in zip(axes.flat,METRICS):
        for cond in PLOT_ORDER:
            if cond not in groups:continue
            xx=[3000,10000,50000]
            samples=[[sign*float(r['QMC4_minus_QMC3_'+metric]) for r in groups[cond] if int(r['m'])==m] for m in xx]
            for i,y in enumerate(samples):
                assert len(y)==5,(name,cond,metric,xx[i],len(y))
                ax.scatter([i]*len(y),y,alpha=0.36,s=25)
            avg=[mean(v) for v in samples]
            sd=[stdev(v) for v in samples]
            ax.errorbar(range(3),avg,yerr=sd,marker='o',capsize=4,label=LABELS[cond])
        ax.axhline(0,ls='--',lw=0.9,color='gray')
        ax.set_xticks(range(3),['3,000','10,000','50,000'])
        ax.set_xlabel('Number of input quartets')
        ax.set_ylabel('Paired gain (positive favors QMC4)')
        ax.set_title(title)
        ax.grid(alpha=0.15)
    handles,labels=axes.flat[0].get_legend_handles_labels()
    fig.suptitle('Paired QMC4 accuracy gains: '+('primary benchmark' if name.startswith('primary') else 'independent unweighted validation'),fontsize=16)
    fig.legend(handles,labels,loc='upper center',ncol=3,bbox_to_anchor=(0.5,0.96))
    out.parent.mkdir(parents=True,exist_ok=True)
    fig.savefig(out,dpi=160,bbox_inches='tight')
    plt.close(fig)
    print('Wrote',out)

if __name__=='__main__':
    p=argparse.ArgumentParser()
    p.add_argument('--output-dir',type=Path,default=HERE/'reproduced_figures')
    args=p.parse_args()
    plot('primary_paired_accuracy.csv',args.output_dir/'primary_paired_gains.png')
    plot('independent_unweighted_paired_accuracy.csv',args.output_dir/'independent_unweighted_paired_gains.png')
