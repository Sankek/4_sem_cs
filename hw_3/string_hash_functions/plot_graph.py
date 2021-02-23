import pandas as pd
import os
import matplotlib.pyplot as plt

data_directory = "data"
results_directory = "results"

fig, ax = plt.subplots(figsize=(7, 5))


for file_name in os.listdir(data_directory):
    if file_name.endswith(".csv") and file_name.startswith("collisions_test"):
        file_path = os.path.join(data_directory, file_name)

        name = file_name.partition("collisions_test_")[2].partition(".csv")[0]
        data = pd.read_csv(file_path)

        x_data = data.iloc[:, 0].values
        y_data = data.iloc[:, 1].values

        ax.plot(x_data, y_data, label=name, linewidth=1)


ax.set_xlabel("number of elements")
ax.set_ylabel("number of collisions")
ax.grid(True)
ax.legend(loc='best')

if not os.path.exists(results_directory):
    os.mkdir(results_directory)
plt.savefig("results/collisions_test.pdf")






