import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("output_queue_M.csv", header=None)

fig, ax = plt.subplots(figsize=(7, 7))
ax.set_title("Queue")
ax.plot(df.iloc[:, 0], df.iloc[:, 1], label="Boost")
ax.plot(df.iloc[:, 0], df.iloc[:, 2], label="Seminar")
ax.legend()
ax.set_xlabel("size")
ax.set_ylabel("time")
ax.set_xscale("log")
ax.set_yscale("log")

plt.savefig("graph_queue_M.pdf")

df = pd.read_csv("output_stack_M.csv", header=None)

fig, ax = plt.subplots(figsize=(7, 7))
ax.set_title("Stack")
ax.plot(df.iloc[:, 0], df.iloc[:, 1], label="Boost")
ax.plot(df.iloc[:, 0], df.iloc[:, 2], label="Seminar")
ax.legend()
ax.set_xlabel("size")
ax.set_ylabel("time")
ax.set_xscale("log")
ax.set_yscale("log")

plt.savefig("graph_stack_M.pdf")

"""######"""

df = pd.read_csv("output_queue_N.csv", header=None)

fig, ax = plt.subplots(figsize=(7, 7))
ax.set_title("Queue")
ax.plot(df.iloc[:, 0], df.iloc[:, 1], label="Boost")
ax.plot(df.iloc[:, 0], df.iloc[:, 2], label="Seminar")
ax.legend()
ax.set_xlabel("threads")
ax.set_ylabel("time")
# ax.set_xscale("log")
# ax.set_yscale("log")

plt.savefig("graph_queue_N.pdf")

df = pd.read_csv("output_stack_N.csv", header=None)

fig, ax = plt.subplots(figsize=(7, 7))
ax.set_title("Stack")
ax.plot(df.iloc[:, 0], df.iloc[:, 1], label="Boost")
ax.plot(df.iloc[:, 0], df.iloc[:, 2], label="Seminar")
ax.legend()
ax.set_xlabel("threads")
ax.set_ylabel("time")
# ax.set_xscale("log")
# ax.set_yscale("log")

plt.savefig("graph_stack_N.pdf")
