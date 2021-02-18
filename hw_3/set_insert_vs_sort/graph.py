import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv", index_col=0)

sizes = [float(x) for x in df.columns]
containers = ["std::vector", "std::array", "std::set"]
colors = ["tab:blue", "tab:orange", "tab:green"]

fig, ax = plt.subplots(figsize=(6, 6))

for i in range(len(containers)):
    times = df.iloc[2*i].to_numpy(dtype=float)
    times_std = df.iloc[2*i+1].to_numpy(dtype=float)
    ax.plot(sizes, times, color=colors[i], label=containers[i], zorder=i+2)
    ax.errorbar(sizes, times, yerr=times_std, fmt=' ', color=colors[i], zorder=i+2)

# plt.tight_layout(0.5)

ax.set_xlabel("number of elements")
ax.set_xscale("log")
ax.set_ylabel("measure time (microseconds)")
ax.set_yscale("log")
ax.grid(True)
plt.legend(loc='best')
plt.savefig("graph.pdf")

