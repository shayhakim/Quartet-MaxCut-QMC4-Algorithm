# Tree of Life (191 taxa) - wQMC4 quartet sampling

This experiment reconstructs the published 191-taxon Tree of Life reference topology from uniformly sampled, distinct quartets with reference-derived `w1` edge weights. It is an evaluation against a published tree, not an independent sequence-based phylogenetic inference.

## Reported figure

`figures/QMC4_TreeOfLife_n191.png` is the updated figure for the Supplemental Material. The **Tree of Life (n=191)** curve was recomputed from new 191-taxon runs (November 2026). The **Random Tree (n=1000)** curve uses the previously reported comparison series.

The original figure's appearance, series names and log-normalized x-axis have been retained. The x-coordinate is `log10(m/191)`.

## Reconstructed Tree of Life measurements

| Distinct input quartets (m) | Qdist | Taxa in final tree |
| ---: | ---: | ---: |
| 191 | 0.346798 | 191 |
| 428 | 0.702026 | 191 |
| 1,910 | 0.945752 | 191 |
| 9,573 | 0.992101 | 191 |

The metric is the fraction of resolved four-taxon reference splits also displayed in the reconstructed tree. Each value was computed by exact enumeration over all `C(191,4) = 53,727,345` quartets; unresolved quartets in the inferred tree are not counted as matches. The original reference tree has fully resolved topology.

## Data in this directory

- `reference/reference_191.newick`: published reference tree with the original species names and branch lengths.
- `data/tip_index_to_species.tsv`: preserved mapping from integer codes `1..191` to the reference-tip names, in original Newick tip order.
- `data/tree_of_life_191_results.csv`: measured score, counts of agreeing/conflicting/unresolved quartets, seeds, cardinalities and input/output checksums.
- `trees/m_*_191tips.tree`: reconstructed trees with labels `1..191`. The executable's extra auxiliary label `0` has been removed (only that non-biological leaf), before scoring.
- `figures/QMC4_TreeOfLife_n191.png`: updated supplementary figure.

## Experimental protocol and scope

Four **independent** quartet datasets were sampled without replacement within each dataset, with `m = 191, 428, 1910, 9573`. Quartet topologies were extracted from the 191-taxon reference tree, and each quartet was assigned the original `w1` ratio (its induced internal-branch length divided by the sum of all five induced quartet branch lengths). Weights were saved as fixed-point decimal values with 15 fractional digits to conform to the legacy executable's numeric-input parser; values below `1e-15` were represented by `1e-15`. Each run used the weighted reconstruction mode of the QMC4 release binary (`-dt 4 -index 2 -w 1 -NNN 1`). The sample seeds appear in the results CSV. The executable's SHA-256 was `6225edc05ced1d85f47677e517ea4e26af4335b9fa77bdcfd103feb27209f076`. Each point represents one reconstruction (not a replicate mean); algorithmic randomness internal to the executable may cause variation if the same input is run again.

The 191-quartet sample happens to observe 186 of the 191 labels; the final QMC4 output tree nevertheless retains all 191 taxa. All other samples observe all 191 labels. No additional quartets were inserted to force taxon coverage.

The original reference and four inferred trees provide the underlying topologies for direct metric verification.
