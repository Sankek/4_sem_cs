import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("results.csv", index_col=0)

sizes = [1000, 10000, 100000, 1000000, 10000000]
containers = ["std::vector", "std::list", "std::forward_list", "std::deque", "std::array"]
colors = ["tab:blue", "tab:orange", "tab:green", "tab:red", "tab:purple"]

fig, ax = plt.subplots(figsize=(6, 6))

for i in range(len(containers)):
    ax.plot(sizes, df.iloc[i].to_numpy(dtype=float), color=colors[i], label=containers[i])

# plt.tight_layout(0.5)

ax.set_xlabel("push_back num when full")
ax.set_xscale("log")
ax.set_ylabel("vector capacity")
ax.set_yscale("log")
ax.grid(True)
plt.legend(loc='best')
plt.savefig("graph.pdf")

print(df.iloc[0].to_numpy(dtype=float))
