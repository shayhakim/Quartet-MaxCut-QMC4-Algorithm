#!/usr/bin/env python3
import csv
import math
import statistics
from collections import defaultdict
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

HERE = Path(__file__).resolve().parent.parent
SOURCE = HERE / "reports/large_original_5rep/large_accuracy.csv"
OUT = HERE / "reports/large_original_5rep/figures"
OUT.mkdir(parents=True, exist_ok=True)

COLORS = {
    1000: "#2368ae",
    3000: "#d27b29",
    5000: "#27956c",
}

EXPECTED = {
    1000: (3000, 10000, 50000, 100000, 200000),
    3000: (10000, 50000, 100000, 200000),
    5000: (20000, 50000, 100000, 200000),
}

groups = defaultdict(dict)

with SOURCE.open(newline="") as f:
    for row in csv.DictReader(f):
        if row["metric"] != "qdist":
            continue

        n = int(row["n"])
        m = int(row["m"])
        rep = int(row["replicate"])
        value = float(row["value"])

        assert n in EXPECTED
        assert m in EXPECTED[n]
        assert 1 <= rep <= 5
        assert math.isfinite(value) and 0 <= value <= 1
        assert rep not in groups[(n, m)], "Duplicate measurement"

        groups[(n, m)][rep] = value

assert len(groups) == 13, f"Expected 13 selected conditions, got {len(groups)}"

for n, ms in EXPECTED.items():
    for m in ms:
        assert len(groups[(n, m)]) == 5, f"Incomplete n={n}, m={m}"

def make_figure(normalized):
    plt.rcParams.update({
        "font.family": "DejaVu Sans",
        "font.size": 11,
        "axes.spines.top": False,
        "axes.spines.right": False,
        "svg.fonttype": "none",
    })

    fig, ax = plt.subplots(figsize=(10.6, 5.8))

    for n, ms in EXPECTED.items():
        x = []
        means = []
        sds = []

        for m in ms:
            values = list(groups[(n, m)].values())

            x.append(math.log10(m / n) if normalized else m)
            means.append(statistics.mean(values))
            sds.append(statistics.stdev(values))

        ax.errorbar(
            x, means, yerr=sds,
            color=COLORS[n],
            marker="o",
            markersize=5,
            linewidth=2,
            capsize=3,
            elinewidth=1.2,
            label=f"{n:,} taxa",
        )

    ax.set_ylabel("Quartet agreement (Qdist)")
    ax.set_ylim(0.6, 1.0)

    if normalized:
        ax.set_xlabel("log₁₀(Normalized Quartet Count)")
        ax.set_title("QMC Accuracy on Large Trees — Normalized Quartet Count")
        stem = "large_qdist_normalized_selected"
    else:
        ax.set_xlabel("Number of sampled quartets (m)")
        ax.set_title("QMC Accuracy on Large Trees — Absolute Quartet Count")
        ax.ticklabel_format(axis="x", style="plain")
        stem = "large_qdist_absolute_selected"

    ax.grid(alpha=0.18)
    ax.legend(frameon=False)
    fig.tight_layout()

    png = OUT / f"{stem}.png"
    svg = OUT / f"{stem}.svg"

    fig.savefig(png, dpi=220)
    fig.savefig(svg)
    plt.close(fig)

    print("PNG:", png)
    print("SVG:", svg)

make_figure(False)
make_figure(True)

print("SUCCESS: 65 Qdist measurements, 13 conditions, 5 replicates each")
