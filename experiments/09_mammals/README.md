# Mammalian 37-taxon case study: QMC4 and SVDquest

This case study uses the established 37-taxon mammalian dataset with 424 retained loci. QMC4 reconstructed an unrooted species tree from 66,045 quartets obtained from the SVDquest workflow. The QMC4 and SVDquest trees have the **same unrooted topology**.

## Main result

- **37 taxa** in each tree.
- **34 nontrivial unrooted splits** in each tree, all 34 shared.
- **Unrooted Robinson-Foulds distance: 0**.
- The numeric QMC4 tree is relabeled using the taxon identifiers in `data/taxon_mapping.csv`. The identifier correspondence was established by independent complete DNA-sequence matching (37 of 37 taxa), with `qmc_code = nexus_code + 1`.
- The QMC4 tree is unchanged topologically by relabeling.

The quartet support calculation from the original experiment reports **54,502 compatible** and **11,543 incompatible** quartets (66,045 total; 82.52% compatible). This is a support statistic for the QMC4 tree, not a direct QMC4-versus-SVDquest accuracy score.

## Files

- `trees/qmc4_indexed.tree`: original numerical QMC4 Newick tree.
- `trees/qmc4_named_corrected.tree`: same QMC4 topology, with independently verified taxon names.
- `trees/svdquest_greedy.tree`: SVDquest greedy Newick tree used for comparison.
- `data/taxon_mapping.csv`: verified mapping between numeric QMC4 and NEXUS identifiers and taxon names.
- `data/quartet_support.csv`: original QMC4 quartet compatibility counts.
- `figures/CompareQMCtoSVDQuest.png`: comparison figure used in the manuscript.
- `scripts/verify_topologies.py`: standalone Python 3 script to recompute the unrooted RF comparison.

## Verify the comparison

From the repository root, run:

```bash
python3 experiments/09_mammals/scripts/verify_topologies.py
```

Expected output includes `Shared splits: 34 Unrooted RF distance: 0` and `STATUS: PASS`. The script uses only Python's standard library.

The supplied trees, identifier mapping and validation script enable independent verification of the reported topology comparison.
