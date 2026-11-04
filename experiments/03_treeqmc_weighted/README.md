# Matched weighted comparison: QMC4 vs TREE-QMC

This directory contains **final figure-level results** of the separate 1,000-taxon weighted and unweighted direct-quartet comparison reported in the manuscript and Supplemental Material.

## Experimental design

- Three independent reference trees generated under a pure-birth (birth-death with death rate zero) model, with patristic branch-length distances.
- For each reference tree, nested uniform samples of 31,623, 100,000, and 316,228 quartet subsets.
- The same quartet topologies were given to QMC4, TREE-QMC n0, and TREE-QMC n2 under unit-weight and oracle reference-tree-derived `w1` weighting.
- 3 reference trees x 3 sample sizes x 3 configurations x 2 weight modes = **54 successful complete reconstructions**.

## Original measurements and figures

- `results/weighted_accuracy_extended.csv`: 54 rows of Qdist agreement, normalized RF, normalized MAST, and TreeDistance.
- `results/weighted_runtime_54runs.csv`: the corresponding 54 successful-run runtimes.
- `figures/weighted_accuracy_4panel_extended.png`: original four-metric Supplemental figure.
- `figures/weighted_runtime_54runs.png`: original runtime Supplemental figure.

## Replot saved metrics

From this directory, run `python3 scripts/replot_from_csv.py` (requires `matplotlib`). Reproduced plots go under `figures/reproduced/`, without modifying the archived manuscript figures. Their graphical styling may differ.

The stored results are the measurements underlying the displayed manuscript figures. The weighting uses reference-tree-derived oracle weights rather than estimates from sequence alignments.
