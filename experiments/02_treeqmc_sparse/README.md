# Independent sparse-quartet comparison: QMC4 vs TREE-QMC

This directory contains the **final figure-level results** of the unweighted, direct-quartet benchmark reported in the main manuscript and Supplemental Material. This benchmark is independent of the QMC4-only large-tree simulation in `01_sparse_scalability`.

## Experimental design

- 1,000 taxa; 75 independent quartet datasets (15 at each of five sampling levels: 1,000, 3,162, 10,000, 31,623, and 100,000 quartets).
- Identical unit-weight quartet inputs were supplied to QMC4, TREE-QMC n0, and TREE-QMC n2 for each dataset.
- TREE-QMC v4.1.5, commit `53b15e5c365ebee1114575c7999eb7660163a385`; direct-quartet mode, with `--norm_atax 0` or `--norm_atax 2`.
- Only outputs with the intended complete taxon set were included in accuracy and successful-run runtime metrics.

## Original measurements and figures

- `results/qdist_n1000.csv`: 193 Qdist observations (75 QMC4; 59 TREE-QMC n0; 59 TREE-QMC n2).
- `results/other_metrics_n1000.csv`: 579 observations (RF, MAST, TreeDistance, 193 per metric).
- `results/runtime_successful_runs_n1000.csv`: 193 completed-run runtimes, excluding data generation and post-processing.
- `figures/runtime_boxplot_n1000.png`: original main-manuscript runtime figure.
- `figures/accuracy_uniform_n1000_consistent.png`: original four-metric Supplemental figure.

The 225 attempted runs produced 193 complete outputs. TREE-QMC had no complete outputs at the lowest sampling level. Accuracy measurements cover complete output trees only; no numerical scores were imputed for unsuccessful runs.

## Replot saved metrics

From this directory, run `python3 scripts/replot_from_csv.py` (requires `matplotlib`). Reproduced plots are written under `figures/reproduced/`; the archived original manuscript PNG files are kept unchanged. Styling may differ while source measurements remain identical.

The supplied measurements and plotting script support validation and reproduction of the figure-level comparison. The simulation and eligibility protocol is specified in the manuscript Supplemental Material.
