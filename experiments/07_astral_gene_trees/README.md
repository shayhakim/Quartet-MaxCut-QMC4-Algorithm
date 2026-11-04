# ASTRAL-II and QMC4 reconstruction from gene-tree-derived quartets

This directory contains a **curated accuracy-results extract** for the
100-taxon simulation reported in the QMC4 manuscript, with 50, 200,
and 1,000 inferred gene trees. It is distinct from the
**SVDquartets + QFM vs SVDquartets + QMC4** experiment in `06_svdquartets_qfm`.

## Contents

- `data/accuracy_by_replicate.csv`: 603 observed rows from the original
  `Results12_2022.xlsx` result table, preserving replicate identifiers,
  ASTRAL-II option labels, and four published accuracy metrics.
- `figures/astral_Qdist_MAST.png`: original manuscript two-metric figure.
- `figures/QMC4_Figure17_Redesigned_2x2.png`: original supplementary
  four-metric figure.
- `scripts/check_data.py`: validates row counts, replicate identifiers and metric values.
- `scripts/plot_results.py`: regenerates **equivalent** grouped boxplots
  from the CSV; not intended to reproduce fonts or pixels exactly.

## Recorded observations

| Method | Gene trees | Observations |
| --- | ---: | ---: |
| QMC4 | 50 | 10 |
| QMC4 | 200 | 10 |
| QMC4 | 1000 | 7 |
| ASTRAL-II, each of four p/typetree settings | 50 | 48 |
| ASTRAL-II, each of four p/typetree settings | 200 | 48 |
| ASTRAL-II, each of four p/typetree settings | 1000 | 48 |

The four ASTRAL-II settings retain the **original option identifiers**:
`p0_halfresolved`, `p0_true-halfresolved`, `p1_halfresolved`, and
`p1_true-halfresolved`. These settings are not four extra QMC4 runs.

The plots use the **observed sample sizes shown above**: 10/10/7 QMC4 results
at 50/200/1,000 gene trees, respectively, and 48 ASTRAL-II results per
configuration and gene-tree count. Values are not imputed; each boxplot
summarizes its own observed group.

Metrics are `qdist` and `mast_normalized` (higher is better), and
`rf_normalized` and `tree_distance` (lower is better).

## Run the included checks or regenerate figures

```bash
python3 experiments/07_astral_gene_trees/scripts/check_data.py
python3 -m pip install matplotlib
python3 experiments/07_astral_gene_trees/scripts/plot_results.py
```

The plotting script writes figures to a `reproduced_figures/` directory.
The CSV alone suffices to replot the reported accuracy distributions.

## Data provenance

The recorded metrics were extracted from the `AstralVsQMC4` worksheet of
`Results12_2022.xlsx` and cross-checked against independent results tables.
The CSV and scripts allow numerical verification and replotting of the observed distributions.
