#!/usr/bin/env python3
"""Verify 37-taxon unrooted topological equivalence using Python stdlib only.

No QMC4/SVDquest implementation or original sequence alignment is required.
"""
from pathlib import Path
import csv
import re

HERE = Path(__file__).resolve().parent.parent


def parse_newick(s):
    """Minimal Newick parser for the named trees shipped with this case study."""
    s=s.strip()
    pos=0

    def ws():
        nonlocal pos
        while pos<len(s) and s[pos].isspace(): pos+=1

    def branch_length():
        nonlocal pos
        ws()
        if pos<len(s) and s[pos]==':':
            pos+=1
            while pos<len(s) and s[pos] not in ',);': pos+=1

    def node():
        nonlocal pos
        ws()
        if pos<len(s) and s[pos]=='(':
            pos+=1
            children=[node()]
            ws()
            while pos<len(s) and s[pos]==',':
                pos+=1
                children.append(node())
                ws()
            if pos>=len(s) or s[pos]!=')':
                raise ValueError('Unbalanced Newick parentheses')
            pos+=1
            # Optional internal node label (none used here).
            while pos<len(s) and s[pos] not in ',);:': pos+=1
            branch_length()
            return children
        start=pos
        while pos<len(s) and s[pos] not in ',);:': pos+=1
        name=s[start:pos].strip()
        if not name: raise ValueError('Empty taxon label')
        branch_length()
        return name

    tree=node()
    ws()
    if pos>=len(s) or s[pos]!=';': raise ValueError('Missing final semicolon')
    pos+=1
    ws()
    if pos!=len(s): raise ValueError('Trailing Newick content')
    return tree


def splits(tree):
    def leaves(t):
        return {t} if isinstance(t,str) else set().union(*(leaves(c) for c in t))
    universe=leaves(tree)
    if len(universe)!=37: raise ValueError(f'Expected 37 distinct taxa, found {len(universe)}')
    seen=[]
    def visit(t):
        if isinstance(t,str):
            seen.append(t)
            return {t}
        desc=set()
        for child in t: desc.update(visit(child))
        return desc
    # Descendant sets for every child edge, ignoring trivial bipartitions.
    def walk(t):
        if isinstance(t,str): return {t},set()
        desc=set(); parts=set()
        for child in t:
            d, p=walk(child)
            desc.update(d); parts.update(p)
            if 1<len(d)<len(universe)-1:
                complement=universe-d
                canonical=frozenset(d if (len(d),sorted(d)) <= (len(complement),sorted(complement)) else complement)
                parts.add(canonical)
        return desc,parts
    _,parts=walk(tree)
    return universe,parts


def main():
    qmc=HERE/'trees/qmc4_named_corrected.tree'
    svd=HERE/'trees/svdquest_greedy.tree'
    a,sa=splits(parse_newick(qmc.read_text(encoding='utf-8')))
    b,sb=splits(parse_newick(svd.read_text(encoding='utf-8')))
    mapping=list(csv.DictReader((HERE/'data/taxon_mapping.csv').open(encoding='utf-8')))
    assert len(mapping)==37 and {x['taxon'] for x in mapping}==a
    assert all(int(x['qmc_code'])==int(x['nexus_code'])+1 for x in mapping)
    assert a==b
    rf=len(sa-sb)+len(sb-sa)
    print('QMC4 taxa:',len(a),'SVDquest taxa:',len(b))
    print('QMC4 nontrivial unrooted splits:',len(sa))
    print('SVDquest nontrivial unrooted splits:',len(sb))
    print('Shared splits:',len(sa & sb),'Unrooted RF distance:',rf)
    if len(sa)!=34 or len(sb)!=34 or rf!=0:
        raise SystemExit('FAIL: topology mismatch')
    print('STATUS: PASS')

if __name__=='__main__': main()
