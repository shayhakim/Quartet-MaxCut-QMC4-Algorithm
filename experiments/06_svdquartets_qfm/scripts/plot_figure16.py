#!/usr/bin/env python3
"""Validate and plot the published 5-replicate, 3-pipeline comparison."""
import csv
from pathlib import Path
from statistics import median

ROOT = Path(__file__).resolve().parent.parent
DATA = ROOT / "data" / "pipeline_metrics.csv"
METHODS = ["ASTRAL-II", "SVDquartets+QFM", "SVDquartets+QMC4"]
METRICS = [
    ("qdist_agreement", "Quartet agreement (Qdist)", "Higher is better"),
    ("rf_normalized", "Normalized RF distance", "Lower is better"),
    ("tree_distance", "TreeDistance", "Lower is better"),
]

def main():
    import matplotlib.pyplot as plt

    with DATA.open(newline="", encoding="utf-8") as f:
        observations = list(csv.DictReader(f))
    assert len(observations) == 15, "Expected 15 observations"
    assert len({(int(x["replicate"]), x["pipeline"]) for x in observations}) == 15
    assert {x["pipeline"] for x in observations} == set(METHODS)
    for method in METHODS:
        assert sorted(int(x["replicate"]) for x in observations if x["pipeline"] == method) == list(range(1, 6))

    fig, axs = plt.subplots(1, 3, figsize=(13, 5.5))
    colors = ["#E86A61", "#19985E", "#4E83CD"]
    tick_names = ["ASTRAL-II", "SVD+QFM", "SVD+QMC4"]
    for ax, (key, title, direction) in zip(axs, METRICS):
        vals = [[float(x[key]) for x in observations if x["pipeline"] == method] for method in METHODS]
        plot = ax.boxplot(vals, patch_artist=True, whis=1.5, widths=0.52)
        for box, color in zip(plot["boxes"], colors):
            box.set(facecolor=color, edgecolor="#465166", linewidth=1.4)
        for obj in plot["medians"]:
            obj.set(color="#273244", linewidth=2.3)
        for obj in plot["whiskers"] + plot["caps"]:
            obj.set(color="#465166", linewidth=1.2)
        for obj in plot["fliers"]:
            obj.set(marker="o", markersize=5, markerfacecolor="#273244", markeredgecolor="#273244")
        ax.set_xticks([1, 2, 3], tick_names, rotation=20, ha="right")
        ax.set_title(title, fontsize=13, fontweight="bold")
        ax.text(0.5, 1.01, direction, color="#64748b", transform=ax.transAxes, ha="center", fontsize=10)
        ax.yaxis.grid(True, alpha=0.22)
        print(title + ": " + ", ".join(f"{m}={median(v):.6f}" for m, v in zip(METHODS, vals)))
    fig.tight_layout()
    dest = ROOT / "figure16_reproduced.png"
    fig.savefig(dest, dpi=200, bbox_inches="tight")
    print(f"Saved {dest}")

if __name__ == "__main__":
    main()
