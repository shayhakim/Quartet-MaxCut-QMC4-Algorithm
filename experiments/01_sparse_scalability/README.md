# Experiment 01: QMC4 accuracy and runtime on large simulated trees

This directory contains the measurements and figures used for the large-tree QMC4 experiment in the manuscript. The simulation examines model trees with **1,000, 3,000 and 5,000 taxa**, five independent reference trees per reported condition, uniformly sampled distinct quartets and unit weights.

## Figure datasets

| File | Data represented |
| --- | --- |
| `reports/large_original_5rep/large_accuracy.csv` | 65 Qdist measurements: 13 plotted sampling conditions, five reference trees per condition. |
| `reports/large_original_5rep/figures/large_runtime_selected.csv` | 64 runtime observations used in the manuscript runtime boxplot: 25 for 1,000 taxa; 20 for 3,000 taxa; 19 for 5,000 taxa. |
| `reports/large_original_5rep/figures/large_qdist_normalized_selected.png` | Main-text quartet agreement by normalized quartet sample size. |
| `reports/large_original_5rep/figures/large_qdist_absolute_selected.png` | Quartet agreement by absolute quartet count. |
| `reports/large_original_5rep/figures/large_runtime_by_tree_size_linear.png` | Runtime distributions by taxon count. |

For the 1,000-taxon trees, the plotted quartet counts are 3,000, 10,000, 50,000, 100,000 and 200,000. For the 3,000-taxon trees they are 10,000, 50,000, 100,000 and 200,000; for the 5,000-taxon trees they are 20,000, 50,000, 100,000 and 200,000. The data tables correspond to those plotted conditions and observations; they are **figure-level records**, not an inventory of every computation ever performed.

## Regenerate figures

Requires Python 3 and Matplotlib. From this directory:

```bash
python3 scripts/plot_large_qdist_selected.py
python3 scripts/plot_large_runtime_by_tree_size_linear.py
```

The scripts read the deposited CSV measurements and render the saved Qdist and runtime series. The two Qdist figures also have SVG versions alongside the PNGs, as does the runtime figure. The manuscript provides the detailed model and evaluation protocol.
