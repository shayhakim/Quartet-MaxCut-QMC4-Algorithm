# Paired QMC3–QMC4 comparison (Supplemental Material)

This folder contains the **final paired result tables and two figures** used for the QMC3–QMC4 comparison in the manuscript *Accurate Large-Scale Phylogenetic Reconstruction Using Sparse Random Quartet Samples* (Hakim and Snir). The package provides the paired measurements and figures.

## Experimental design

- **Primary paired benchmark:** 5 independently generated birth–death reference trees with 1,000 taxa, 3 quartet counts (`m=3,000`, `10,000`, `50,000`), and 3 conditions: `QMC` (unweighted), `W1` (oracle weights calculated from reference-tree topology), and `W1_RW20` (the same weighting method, followed by weight-dependent rewiring at weighted-threshold parameter `r=0.2`). Thus, `5 × 3 × 3 = 45` matched algorithm pairs.
- **Independent validation:** 5 distinct reference trees from an existing unweighted dataset, evaluated at the same three quartet counts: `5 × 3 = 15` additional matched pairs. It is **not** pooled into the primary figure.
- QMC3 and QMC4 received **identical quartet input within each matched pair**, as checked using input SHA-256 metadata retained in both accuracy and runtime tables.
- Both programs were run once per input. QMC4 used tree-reconstruction mode (`-index 2`). This benchmark contrasts distinct QMC implementations, not two builds of a single shared source tree.
- `W1` weights are **oracle/reference-tree-derived**; they are not estimated from sequence data. The nominal rewiring threshold is not the literal fraction of quartets modified. In the original primary experiment, rewiring affected approximately 44% of total quartet weight and preferentially changed lower-weight quartets while leaving their assigned weights unchanged.

## Files

| File | Contents |
| --- | --- |
| `results/primary_paired_accuracy.csv` | 45 primary pairs; Qdist, normalized RF, normalized MAST, TreeDistance for each algorithm and paired differences. |
| `results/primary_paired_runtime.csv` | 45 primary pairs; wall-clock runtimes, statuses, matched-input SHA-256 and realized rewiring metadata. |
| `results/independent_unweighted_paired_accuracy.csv` | 15 independent unweighted validation pairs; four accuracy metrics. |
| `results/independent_unweighted_paired_runtime.csv` | 15 validation pairs; runtimes and input hashes. |
| `figures/supp_4metrics_paired_gains.png` | Final four-panel primary comparison from the Supplemental Material. |
| `figures/supp_independent_unweighted_validation.png` | Final four-panel independent validation comparison from the Supplemental Material. |
| `scripts/verify_results.py` | Standard-library check of file integrity, pairing, input hashes, metrics, and benchmark counts; prints grouped summaries. |
| `scripts/reproduce_plots.py` | Optional plotting-only script (requires Matplotlib), reproducing the paired gain series and uncertainty summaries from the CSV results. Its rendering may differ cosmetically from the submitted PNG files. |

## Interpretation and metric orientation

- **Qdist** and **normalized MAST**: larger is better. Paired gain = `QMC4 − QMC3`.
- **Normalized RF** and **TreeDistance**: smaller is better. Paired gain = `QMC3 − QMC4` (the CSV's named `QMC4_minus_QMC3_...` fields have the *opposite* sign). The historical column name `tree_distance_raw` denotes the saved numerical value of the TreeDist-derived metric used in the article, not a replacement or newly computed metric.
- **Runtime overhead:** `QMC4 seconds − QMC3 seconds`; positive indicates QMC4 took longer. The Supplemental figures here display **accuracy differences**, not absolute runtimes.

The primary W1 condition favored QMC4 on Qdist and TreeDistance in all 15 pairs. The independent unweighted set favored QMC4 on Qdist in all 15 pairs. The primary unweighted condition averaged about 43 s for QMC4 versus about 19 s for QMC3.

## Verify and explore

Run from this folder:

```bash
python3 scripts/verify_results.py
python3 scripts/reproduce_plots.py --output-dir reproduced_figures
```

The second command is optional and requires `matplotlib`. Plotting and tabular verification use only the deposited result CSVs.

The included scripts verify the deposited paired measurements and regenerate the comparison plots from those measurements. The recorded input hashes support the within-pair input-consistency check.
