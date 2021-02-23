import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy import optimize

df = pd.read_csv("results/collisions_test.csv")


x_data = df.iloc[:, 0].values
y_data = df.iloc[:, 1].values
x2_data = df.iloc[:, 2].values

fit_end = int(len(x_data)/3)
x_to_fit_data = x_data[:fit_end]
y_to_fit_data = y_data[:fit_end]


def test_func(x, a, b, c):
    return a*(x-b)**2+c


# p0 - initial guesses for the parameters
p0 = np.ones(test_func.__code__.co_argcount-1)
params, params_covariance = optimize.curve_fit(test_func, x_to_fit_data, y_to_fit_data, p0=p0)

fig, ax = plt.subplots(figsize=(7, 5))

# Second x-axis:
ax2 = ax.twiny()
ax2.plot(x2_data, y_data, alpha=0)
ax2.set_xlabel("load factor")

fit_x_data = np.linspace(x_data.min(), x_data.max(), num=50, endpoint=True)
fit_y_data = test_func(fit_x_data, *params)
ax.plot(fit_x_data, fit_y_data, label="Fitted quadratic function", color='red', linewidth=1.2)

ax.plot(x_data, y_data, label="Data", linewidth=1)

ax.set_xlabel("number of elements")
ax.set_ylabel("number of collisions")
ax.grid(True)
ax.legend(loc='best')
plt.savefig("results/collisions_test.pdf")

