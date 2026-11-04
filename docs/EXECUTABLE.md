# Running QMC4 and wQMC4

The `wQMC4` executable runs **both** unweighted QMC4 and weighted wQMC4. Download the Linux executable from [release v1.0.0](https://github.com/shayhakim/Quartet-MaxCut-QMC4-Algorithm/releases/tag/v1.0.0) and save it in the repository directory (next to `README.md`).

## Requirements

- Linux x86-64, GNU C library (glibc 2.34 or newer), and standard mathematical library.
- Mark the downloaded binary executable: `chmod +x wQMC4`.

## Unweighted quartets

Input: [`examples/unweighted/quartets.txt`](../examples/unweighted/quartets.txt). Every line has the form `a,b|c,d`, e.g. `1,2|3,4`.

From the repository root:

```bash
./wQMC4 -dt 4 -index 2 -fid examples/unweighted/quartets.txt -w 0 -NNN 1 -frtN unweighted_result.tree
```

## Weighted quartets

Input: [`examples/weighted/quartets.txt`](../examples/weighted/quartets.txt). Every line has the form `a,b|c,d:weight`, e.g. `1,2|3,4:1.0`.

```bash
./wQMC4 -dt 4 -index 2 -fid examples/weighted/quartets.txt -w 1 -NNN 1 -frtN weighted_result.tree
```

Both five-quartet examples were tested with the supplied release binary. The output is an unrooted tree in Newick format. The example trees are saved in `examples/unweighted/example.tree` and `examples/weighted/example.tree`. For these five-taxon examples, the executable includes an **auxiliary label `0`** in the output; the input taxa are `1` through `5`.

An additional 12-taxon weighted input and observed tree are provided in `examples/weighted/`. In this separate example, `0` **is an input taxon**, and should not be removed from its output tree.

## Main options

| Option | Meaning |
| --- | --- |
| `-dt 4` | Quartet reconstruction mode |
| `-index 2` | Read quartets from an input file |
| `-fid FILE` | Quartet input file |
| `-w 0` | Unweighted quartet input |
| `-w 1` | Weighted quartet input |
| `-NNN 1` | Numeric taxon identifiers |
| `-frtN FILE` | Output tree file (Newick) |
| `-flg FILE` | Optional log-file path |

Always specify the input and output arguments; invoking the binary with no arguments does not display usage help.

The `src/` directory contains selected algorithmic source excerpts; it is **not** an independent build of the full executable.
