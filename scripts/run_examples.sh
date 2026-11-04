#!/usr/bin/env bash
# Run validated small examples with the supplied wQMC4 Linux executable.
# Usage: bash scripts/run_examples.sh /path/to/wQMC4
set -u
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="${1:-$ROOT/wQMC4}"
case "$BIN" in /*) ;; *) BIN="$(cd "$(dirname "$BIN")" && pwd)/$(basename "$BIN")" ;; esac
if [ ! -x "$BIN" ]; then
  echo "Executable not found or not executable: $BIN" >&2
  echo "Run 'chmod +x wQMC4' after downloading the release asset." >&2
  exit 2
fi
mkdir -p "$ROOT/example_outputs"
run_case() {
  local label="$1" mode="$2" input="$3"
  local tree="$ROOT/example_outputs/${label}.tree"
  rm -f "$tree"
  (cd "$ROOT" && "$BIN" -dt 4 -index 2 -fid "$input" -w "$mode" -NNN 1 -frtN "$tree" -flg "$ROOT/example_outputs/${label}.log") > "$ROOT/example_outputs/${label}.stdout" 2>&1 || :
  if [ ! -s "$tree" ] || ! grep -q ');' "$tree"; then
    echo "Reconstruction did not produce a Newick tree for $label." >&2
    exit 1
  fi
  echo "$label: wrote $tree"
}
run_case unweighted 0 examples/unweighted/quartets.txt
run_case weighted 1 examples/weighted/quartets.txt
run_case weighted_12_taxa 1 examples/weighted/quartets_12_taxa.txt
