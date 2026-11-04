# Sequence-derived quartets: SVDquartets + QFM versus SVDquartets + QMC4

## Scope

This folder contains the **15 published observations (5 replicates per pipeline)** underlying the sequence-derived reconstruction comparison (Figure `QMC4_Figure16_Final_CorrectPipelines.png` in the manuscript).

- **SVDquartets + QFM** and **SVDquartets + QMC4** amalgamated the same SVDquartets-inferred quartet sets for each simulation replicate, as described in the manuscript.
- **ASTRAL-II** was evaluated in the same simulation setting, but uses a separate gene-tree-based inference pipeline. Its performance must **not** be interpreted as a head-to-head comparison of quartet amalgamation algorithms on identical quartet inputs.
- The simulation setting described in the manuscript used 100 taxa, SimPhy and 5 replicates.

## Contents

- `data/pipeline_metrics.csv`: one row for each pipeline and replicate, preserving the original metric values.
- `figures/QMC4_Figure16_Final_CorrectPipelines.png`: the final figure selected for the manuscript, reproduced here without editing.
- `scripts/plot_figure16.py`: standalone, optional plotting and validation utility for the CSV; generates a new visualization rather than modifying the supplied manuscript figure.

The table was assembled from the original `QFM_SVD_ASTRAL_QMC.xlsx` metric workbook, and its RF and TreeDistance fields were cross-checked against `output.xlsx`, which identifies the five true-tree replicates. In the original workbook, the ASTRAL-II code is spelled `Astal2`; this has only been renamed for readability, with no changes to metric values. Replicate indices 1--5 are reconstructed from the original row order and cross-check against the independent distance table.

## Summary of medians

| Pipeline | Qdist agreement (higher better) | normalized RF (lower better) | TreeDistance (lower better) |
|---|---:|---:|---:|
| ASTRAL-II | 0.945741 | 0.295918 | 0.167499 |
| SVDquartets + QFM | 0.770891 | 0.576087 | 0.347373 |
| SVDquartets + QMC4 | 0.778073 | 0.577778 | 0.382015 |

Thus QMC4 has slightly higher median quartet agreement than QFM on their matched SVDquartets inputs; the other two measures do not show uniform improvement.

## Reproducing the metric visualization

From this directory (Python 3 + `matplotlib`):

```bash
python3 scripts/plot_figure16.py
```

This produces `figure16_reproduced.png` here. Its data and boxplot definition match the published values, though its graphical layout need not be pixel-identical to the supplied figure.

## Data verification

The supplied table preserves the 15 recorded metric observations, and the included script reproduces the comparison directly from these recorded values.
