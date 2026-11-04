# QMC4 examples

These are **small functional demonstrations of the released executable**, not experimental datasets from the paper.

| Example | Input | Verified output |
| --- | --- | --- |
| Unweighted, 5 taxa | [`unweighted/quartets.txt`](unweighted/quartets.txt) | [`unweighted/example.tree`](unweighted/example.tree) |
| Weighted, 5 taxa | [`weighted/quartets.txt`](weighted/quartets.txt) | [`weighted/example.tree`](weighted/example.tree) |
| Weighted, 12 taxa | [`weighted/quartets_12_taxa.txt`](weighted/quartets_12_taxa.txt) | [`weighted/example_12_taxa.tree`](weighted/example_12_taxa.tree) |

All three examples were run with the `wQMC4` binary from release `v1.0.0`. The input format and exact commands appear in [Executable instructions](../docs/EXECUTABLE.md).

The five-taxon examples use labels `1`–`5`; the binary's example output includes an auxiliary `0`. The larger example deliberately uses `0` as a genuine input taxon. When evaluating tree distances, verify the taxon-label convention for each dataset before removing a label.
