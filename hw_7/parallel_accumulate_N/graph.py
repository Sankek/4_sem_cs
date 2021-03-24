import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv("output.csv", header=None)
fig, ax = plt.subplots(1, 2, figsize=(12, 4))
ax[0].plot(data[0][:16], data[1][:16])
ax[0].set_xlabel("num_threads")
ax[0].set_ylabel("time")
ax[1].plot(data[0], data[1])
ax[1].set_xlabel("num_threads")
ax[1].set_ylabel("time")
plt.savefig("graph.pdf")
