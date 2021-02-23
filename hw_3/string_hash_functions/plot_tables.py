import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import os

"""
Iterating over .csv data files, counting uniques and saving it in one table.
"""

data_directory = "data"
results_directory = "results"

unique_longest = np.array([], dtype=int)
row_names = []

table_data = {}

for file_name in os.listdir(data_directory):
    if file_name.endswith(".csv") and file_name.startswith("uniform_test"):
        file_path = os.path.join(data_directory, file_name)

        print(f"Processing {file_path}...")

        name = file_name.partition("uniform_test_")[2].partition(".csv")[0]
        row_names += [name]

        data = np.genfromtxt(file_path, delimiter=',', dtype=int)
        unique, counts = np.unique(data, return_counts=True)

        add_unique = unique[np.isin(unique, unique_longest, assume_unique=True, invert=True)]
        unique_longest = np.sort(np.hstack((unique_longest, add_unique)))

        table_data[name] = (unique, counts)


counts_table = pd.DataFrame(index=row_names, columns=unique_longest)

for name in row_names:
    unique, counts = table_data[name]

    for unique_i in range(len(unique)):
        counts_table[unique[unique_i]][name] = counts[unique_i]

fig, ax = plt.subplots(figsize=(9, 3))
ax.axis('off')
ax.set_title("Количество бакетов с заданным количеством элементов (номер столбца)")
the_table = ax.table(cellText=counts_table.values,
                     rowLabels=counts_table.index,
                     colLabels=counts_table.columns, loc='center', cellLoc='center')


fig.tight_layout()
fig.savefig("results/uniform_test.pdf", bbox_inches='tight')
