# wQMC4 versus wQFM: 300-taxon comparisons

This folder contains the 300-taxon weighting, rewiring, and runtime comparisons between wQMC4 and wQFM, including the recorded measurements, figures, and a script for regenerating the summaries.

## Dataset

`data/wqfm_300_taxa_observations.csv` contains 60 observations (30 wQFM, 30 wQMC4) with five sampled-quartet counts (949, 3,000, 9,487, 30,000, and 94,868), three weight schemes (a=1, b=2, c=3), and nominal rewiring settings (40%, 60%). Results include Qdist, normalized MAST, normalized RF, TreeDistance, and elapsed seconds. `source_sheet_row` identifies each observation in the original results table.

Each algorithm has one observation for each combination of sampling level, weight scheme, and rewiring setting (2 algorithms x 5 levels x 3 schemes x 2 settings = 60 observations).

## Figures

- `figures/wqfm_weights_Qdist_MAST.png`: main-text weighting comparison, Qdist and MAST.
- `figures/QMC4_Figure6_wQFM_LINEAR_SourceMatched.png`: the corresponding four-metric supplemental figure.
- `figures/wqfm_rewiring_Qdist_MAST.png`: main-text rewiring comparison, Qdist and MAST.
- `figures/QMC4_Figure7_wQFM_LINEAR_SourceMatched.png`: the corresponding four-metric supplemental figure.
- `figures/RunningTimeQFM.png`: runtime comparison based on the recorded elapsed times.

## Summary aggregation and plotting

`python3 scripts/plot_wqfm_comparisons.py` (requires matplotlib) generates five plots under `reproduced_figures/`. The script is a presentation/replotting utility, **not** the original quartet generation or inference pipeline. The summaries follow these aggregation rules:

- Weight comparison: group by algorithm, quartet count and weight scheme, average across both nominal rewiring settings.
- Rewiring comparison: group by algorithm, quartet count and rewiring setting, averaging across weight schemes a, b, and c.
- Runtime comparison: group measured runtime by log10(number of quartets / number of taxa), plot log10(seconds). Runtime values are unmodified.

The plotting script generates summary figures from the recorded measurements and reproduces their numerical means. Plot formatting may differ from the original figures.
