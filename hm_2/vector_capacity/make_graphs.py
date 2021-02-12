import numpy as np
import matplotlib.pyplot as plt

data_name = "push_back_capacities"
data = np.genfromtxt(data_name+".csv", delimiter=',')

data_degrees = data[1:]/data[0:-1]

fig, ax = plt.subplots(1, 3, figsize=(14, 4))
ax[0].plot(data)
ax[1].plot(data)
ax[2].plot(data_degrees)

ax[0].set_xlabel("push_back num when full")
ax[1].set_xlabel("push_back num when full")
ax[2].set_xlabel("push_back num when full")

ax[0].set_ylabel("vector capacity")
ax[1].set_ylabel("vector capacity")
ax[2].set_ylabel("multiply factor")
ax[1].set_yscale("log")

ax[0].grid(True)
ax[1].grid(True)
ax[2].grid(True)


plt.tight_layout(0.5)

plt.savefig(data_name+".pdf", pad_inches=0)
