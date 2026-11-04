#!/usr/bin/env python3
"""Validate deposited QMC3-vs-QMC4 paired result tables (no external dependencies)."""
import csv
from collections import Counter
from pathlib import Path
from statistics import mean

HERE=Path(__file__).resolve().parents[1]
METRICS=('qdist','rf_normalized','mast_fraction','tree_distance_raw')

def read(name):
    with (HERE/'results'/name).open(newline='', encoding='utf-8') as f:
        return list(csv.DictReader(f))

def key(row):
    return tuple(row[x] for x in ('replicate','m','condition','iteration'))

def verify(prefix,expected,expected_conditions):
    acc=read(prefix+'_paired_accuracy.csv')
    tim=read(prefix+'_paired_runtime.csv')
    aa={key(x):x for x in acc}
    tt={key(x):x for x in tim}
    assert len(acc)==len(tim)==len(aa)==len(tt)==expected, f'{prefix}: missing/duplicate rows'
    assert aa.keys()==tt.keys(),f'{prefix}: accuracy/runtime pairing mismatch'
    assert Counter(x['condition'] for x in acc)==Counter(expected_conditions),f'{prefix}: wrong conditions'
    for k,a in aa.items():
        t=tt[k]
        assert a['identical_input_sha256']==t['same_input_sha256'], f'hash mismatch: {k}'
        assert len(a['identical_input_sha256'])==64, f'bad hash: {k}'
        assert t['status']==t['qmc3_status']==t['qmc4_status']=='ok',f'failed run: {k}'
        assert float(t['qmc3_seconds'])>=0 and float(t['qmc4_seconds'])>=0
        for m in METRICS:
            c=float(a['QMC3_'+m]); d=float(a['QMC4_'+m]); g=float(a['QMC4_minus_QMC3_'+m])
            assert abs((d-c)-g)<1e-9, f'delta mismatch {k}/{m}'
    print(f'{prefix}: {expected} matched pairs, all successful, inputs and deltas verified')
    for cond in sorted(set(x['condition'] for x in acc)):
        group=[x for x in acc if x['condition']==cond]
        d_qdist=mean(float(x['QMC4_minus_QMC3_qdist']) for x in group)
        d_rf=mean(-float(x['QMC4_minus_QMC3_rf_normalized']) for x in group)
        d_mast=mean(float(x['QMC4_minus_QMC3_mast_fraction']) for x in group)
        d_td=mean(-float(x['QMC4_minus_QMC3_tree_distance_raw']) for x in group)
        rts=[tt[key(x)] for x in group]
        rt3=mean(float(t['qmc3_seconds']) for t in rts)
        rt4=mean(float(t['qmc4_seconds']) for t in rts)
        print(f'  {cond:<8} n={len(group):>2} mean gains: Qdist={d_qdist:.6f}, RF={d_rf:.6f}, MAST={d_mast:.6f}, TreeDist={d_td:.6f}; mean times QMC3={rt3:.2f}s QMC4={rt4:.2f}s')
    return aa,tt

if __name__=='__main__':
    primary,rt=verify('primary',45,['QMC']*15+['W1']*15+['W1_RW20']*15)
    val,rv=verify('independent_unweighted',15,['QMC']*15)
    assert not ({x['identical_input_sha256'] for x in primary.values()} & {x['identical_input_sha256'] for x in val.values()}), 'primary/validation inputs overlap'
    assert sum(float(x['QMC4_minus_QMC3_qdist'])>0 for x in primary.values() if x['condition']=='W1')==15
    assert sum(float(x['QMC4_minus_QMC3_tree_distance_raw'])<0 for x in primary.values() if x['condition']=='W1')==15
    assert sum(float(x['QMC4_minus_QMC3_qdist'])>0 for x in val.values())==15
    print('ALL CHECKS PASSED')
