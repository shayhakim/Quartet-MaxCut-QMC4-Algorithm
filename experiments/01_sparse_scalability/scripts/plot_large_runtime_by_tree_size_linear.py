import csv
import math
from collections import defaultdict
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

SOURCE = Path(__file__).resolve().parent.parent / "reports/large_original_5rep/figures/large_runtime_selected.csv"
OUT = SOURCE.parent

groups = defaultdict(list)

with SOURCE.open(newline="") as f:
    for row in csv.DictReader(f):
        n = int(row["n"])
        m = int(row["m"])
        seconds = float(row["runtime_seconds"])

        assert math.isfinite(seconds) and seconds > 0
        groups[n].append(seconds)

assert [len(groups[n]) for n in (1000, 3000, 5000)] == [25, 20, 19]

colors = ["#2368ae", "#d27b29", "#27956c"]
sizes = [1000, 3000, 5000]

plt.rcParams.update({
    "font.family": "DejaVu Sans",
    "font.size": 12,
    "axes.spines.top": False,
    "axes.spines.right": False,
    "svg.fonttype": "none",
})

fig, ax = plt.subplots(figsize=(9, 5.8))

bp = ax.boxplot(
    [groups[n] for n in sizes],
    positions=[1, 2, 3],
    widths=0.45,
    patch_artist=True,
    showmeans=True,
    whis=1.5,
    meanprops=dict(
        marker="D",
        markerfacecolor="white",
        markeredgecolor="black",
        markersize=6,
    ),
    medianprops=dict(color="#222222", linewidth=2),
)

for box, color in zip(bp["boxes"], colors):
    box.set_facecolor(color)
    box.set_edgecolor(color)
    box.set_alpha(0.5)

for key in ("whiskers", "caps"):
    for artist in bp[key]:
        artist.set_color("#555555")

ax.set_xticks([1, 2, 3], ["1,000", "3,000", "5,000"])
ax.set_xlabel("Number of taxa (n)")
ax.set_ylabel("Runtime (seconds)")
ax.set_ylim(bottom=0)
ax.set_title("QMC Runtime by Tree Size", pad=15)

ax.grid(axis="y", which="major", alpha=0.20)
ax.grid(axis="y", which="minor", alpha=0.07)

fig.tight_layout()

png = OUT / "large_runtime_by_tree_size_linear.png"
svg = OUT / "large_runtime_by_tree_size_linear.svg"

fig.savefig(png, dpi=220)
fig.savefig(svg)
plt.close(fig)

print("SUCCESS: 64 runtime measurements")
print("Counts:", {n: len(groups[n]) for n in sizes})
print("PNG:", png)
print("SVG:", svg)
