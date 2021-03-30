import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("output_queue.csv", header=None)

fig, ax = plt.subplots(figsize=(7, 7))
ax.set_title("Queue")
ax.plot(df.iloc[:, 0], df.iloc[:, 1], label="Boost")
ax.plot(df.iloc[:, 0], df.iloc[:, 2], label="Seminar")
ax.legend()
ax.set_xlabel("size")
ax.set_ylabel("time")
ax.set_xscale("log")
ax.set_yscale("log")

plt.savefig("graph_queue.pdf")

df = pd.read_csv("output_stack.csv", header=None)

fig, ax = plt.subplots(figsize=(7, 7))
ax.set_title("Stack")
ax.plot(df.iloc[:, 0], df.iloc[:, 1], label="Boost")
ax.plot(df.iloc[:, 0], df.iloc[:, 2], label="Seminar")
ax.legend()
ax.set_xlabel("size")
ax.set_ylabel("time")
ax.set_xscale("log")
ax.set_yscale("log")

plt.savefig("graph_stack.pdf")
