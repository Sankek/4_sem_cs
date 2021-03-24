import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("output.csv")

fig, ax = plt.subplots(1, 3, figsize=(18, 5))
ax[0].set_title("for_each")
ax[0].plot(df["size"], df["for_each (seq)"], label="Sequential")
ax[0].plot(df["size"], df["for_each (par)"], label="Parallel")
ax[0].legend()
ax[0].set_xlabel("size")
ax[0].set_ylabel("time")
ax[0].set_xscale("log")
ax[0].set_yscale("log")

ax[1].set_title("partial_sum/inclusive_scan")
ax[1].plot(df["size"], df["partial_sum"], label="Sequential")
ax[1].plot(df["size"], df["inclusive_scan"], label="Parallel")
ax[1].legend()
ax[1].set_xlabel("size")
ax[1].set_ylabel("time")
ax[1].set_xscale("log")
ax[1].set_yscale("log")


ax[2].set_title("inner_product/transform_reduce")
ax[2].plot(df["size"], df["inner_product"], label="Sequential")
ax[2].plot(df["size"], df["transform_reduce"], label="Parallel")
ax[2].legend()
ax[2].set_xlabel("size")
ax[2].set_ylabel("time")
ax[2].set_xscale("log")
ax[2].set_yscale("log")


plt.savefig("graph.pdf")
