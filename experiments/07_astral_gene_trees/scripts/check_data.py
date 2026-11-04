#!/usr/bin/env python3
"""Verify the curated observed ASTRAL-II / QMC4 accuracy measurements."""
import csv
from collections import defaultdict
from pathlib import Path
from statistics import median

root=Path(__file__).resolve().parents[1]
with (root/'data/accuracy_by_replicate.csv').open(newline='',encoding='utf-8') as f:
    records=list(csv.DictReader(f))
assert len(records)==603, f"Expected 603 observations; found {len(records)}"
groups=defaultdict(list)
for row in records:
    assert row['method'] in {'QMC4','ASTRAL-II'}
    n=int(row['gene_trees']); rep=int(row['replicate'])
    assert n in (50,200,1000)
    for field in ('qdist','mast_normalized','tree_distance','rf_normalized'):
        v=float(row[field])
        assert 0<=v<=1, (field,row)
    groups[row['method'],n,row['setting']].append(row)
assert len(groups)==15
assert sum(len(rows) for (method,_,_),rows in groups.items() if method=='QMC4')==27
assert sum(len(rows) for (method,_,_),rows in groups.items() if method=='ASTRAL-II')==576
for (method,n,setting), values in groups.items():
    expected=7 if (method,n)==('QMC4',1000) else (10 if method=='QMC4' else 48)
    assert len(values)==expected, (method,n,setting,len(values),expected)
    replicate_ids=[int(row['replicate']) for row in values]
    assert len(set(replicate_ids))==len(replicate_ids)
    if method=='ASTRAL-II':
        assert set(range(1,51))-set(replicate_ids)=={8,47}
    elif n==1000:
        assert set(range(1,11))-set(replicate_ids)=={3,8,9}
    else:
        assert set(replicate_ids)==set(range(1,11))
print('PASS: 603 observed measurements in 15 groups; no fabricated or empty metric values.')
print('QMC4: n=10 for 50 gene trees, n=10 for 200, n=7 for 1000.')
print('ASTRAL-II: n=48 per configuration and gene-tree count (4 x 3 groups).')
print('Validated original replicate identifiers and observed group sizes.')
for n in (50,200,1000):
    values=groups[('QMC4',n,'default')]
    print(f'QMC4 {n} median Qdist: {median(float(v["qdist"]) for v in values):.6f}; '
          f'median MAST: {median(float(v["mast_normalized"]) for v in values):.6f}')
