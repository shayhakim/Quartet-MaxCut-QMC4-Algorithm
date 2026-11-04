# QMC4 — Quartet MaxCut for Large-Scale Phylogenetic Reconstruction

**Executable, selected algorithmic source files, and supporting research results for QMC4 and its weighted extension, wQMC4.**

QMC4 reconstructs unrooted phylogenetic trees from collections of quartet topologies. It refines the recursive Quartet MaxCut (QMC) framework through improved candidate-cut selection, parameter search, randomized partition search, and local partition optimization. The implementation supports both unweighted and weighted quartet evidence.

This repository accompanies the manuscript **“Accurate Large-Scale Phylogenetic Reconstruction Using Sparse Random Quartet Samples”** by **Shay Hakim and Sagi Snir** (University of Haifa).

## Main contributions

- **Sparse random quartet reconstruction.** In model-tree experiments with 1,000, 3,000, and 5,000 taxa, QMC4 reached mean quartet agreement (Qdist) above 0.9 using approximately `10 × n` uniformly sampled quartets, where `n` denotes the number of taxa.
- **Refined Quartet MaxCut optimization.** QMC4 evaluates singleton and pair-cut candidates and refines parameter search, randomized cutting-plane exploration, and local partition improvement while retaining the recursive QMC architecture.
- **Weight-dependent robustness experiments.** The accompanying research introduces a stochastic rewiring mechanism that preferentially perturbs lower-weight quartets, making it possible to evaluate reconstruction accuracy on conflicting quartet inputs.
- **Comparative evaluation.** The manuscript reports experiments against QMC3, TREE-QMC, wQFM, and reconstruction pipelines involving SVDquartets, SVDquest, and ASTRAL-II. Comparisons use Qdist, normalized RF, normalized MAST, and TreeDistance, as applicable to each benchmark.

These are results under the specific experimental protocols reported in the manuscript and Supplemental Material; they are not universal performance guarantees.

## Download and run

Download the Linux x86-64 `wQMC4` executable from [release v1.0.0](https://github.com/shayhakim/Quartet-MaxCut-QMC4-Algorithm/releases/tag/v1.0.0). It supports both **QMC4** (unweighted) and **wQMC4** (weighted) quartet inputs. See [executable instructions](docs/EXECUTABLE.md) for tested commands and input formats.

Quick examples (from the repository root after downloading the binary):

```bash
chmod +x wQMC4
./wQMC4 -dt 4 -index 2 -fid examples/unweighted/quartets.txt -w 0 -NNN 1 -frtN unweighted_result.tree
./wQMC4 -dt 4 -index 2 -fid examples/weighted/quartets.txt -w 1 -NNN 1 -frtN weighted_result.tree
```

Alternatively, execute all included examples with `bash scripts/run_examples.sh ./wQMC4`.

## Repository contents

```text
Quartet-MaxCut-QMC4-Algorithm/
├── LICENSE
├── README.md
├── src/                        selected QMC4 algorithmic source
├── docs/EXECUTABLE.md          binary usage and input format
├── examples/                   verified input/output examples
├── experiments/                nine curated result packages (01-09)
└── scripts/run_examples.sh     example runner
```

The `src/` directory holds selected algorithmic C routines. It is **not** a standalone build of the complete application; required internal headers, types, and helper routines are not distributed as part of these excerpts. The runnable software is supplied as the release binary.

## Reconstruction workflow

At a high level, QMC4:

1. Represents quartet evidence as a graph with *good* edges favoring separation and *bad* edges favoring co-clustering.
2. Searches for a supported nontrivial bipartition, considering singleton/pair candidates and randomized MaxCut-based candidates.
3. Improves candidate partitions using local refinement.
4. Redistributes quartet evidence to the resulting subproblems and recursively assembles the output tree.

The selected source files illustrate parts of this workflow; the release binary provides the executable implementation. The paper and Supplemental Material describe experimental protocols and mathematical details of the separate rewiring mechanism.

## Results and reproducibility

The manuscript evaluates sparse uniform sampling, robustness to quartet perturbations, direct-quartet comparisons with competing amalgamation methods, sequence-/gene-tree-derived quartet inputs, and biological reference datasets. Experimental conditions vary among benchmarks; refer to the manuscript and Supplemental Material for exact sample sizes, weighting schemes, run counts, evaluation measures, and qualifications.

The `experiments/` directory contains curated data tables, saved figures, and benchmark-specific scripts for all nine empirical studies presented in the manuscript: large-tree scaling (01), two TREE-QMC comparisons (02-03), wQFM (04), QMC3 (05), SVDquartets+QFM (06), ASTRAL-II (07), Tree of Life (08), and the mammalian case study (09). See each experiment's README for its protocol, recorded sample sizes, metrics, and available verification or plotting commands.

The supporting result tables and plotting scripts document the figures in the paper. Experiments 08 and 09 additionally include inferred and reference/comparison trees for direct topology checks. The released executable and examples provide a separate functional demonstration.

## Citation

If you use or discuss QMC4, cite:

> Shay Hakim and Sagi Snir. *Accurate Large-Scale Phylogenetic Reconstruction Using Sparse Random Quartet Samples*. Manuscript, 2026.

Please also cite foundational Quartet MaxCut work as appropriate.

## Authors and affiliation

**Shay Hakim · Sagi Snir**  
Department of Evolutionary and Environmental Biology, University of Haifa, Israel.

## License

Use of the repository materials is governed by the terms in [`LICENSE`](LICENSE). Review those terms before copying, modifying, redistributing, or integrating source code. The repository's license does not override third-party rights, if any.
