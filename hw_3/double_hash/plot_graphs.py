import pandas as pd
import matplotlib.pyplot as plt
import os

directory = "results"

for file_name in os.listdir(directory):
    if file_name.endswith(".csv"):
        file_path = os.path.join(directory, file_name)
        data = pd.read_csv(file_path, header=None, index_col=0)

        fig, ax = plt.subplots(figsize=(12, 4))

        for i in range(len(data.index)):
            data_y = data.iloc[i].to_numpy()
            plt.bar(data.columns.to_numpy()+0.3*i, data_y, width=0.4, label=data.index[i])
        plt.legend(loc='lower center')
        plt.title(file_name[:-4])
        plt.xlabel('bucket number')
        plt.ylabel('elements in bucket')
        plt.savefig(f'{file_path[:-4]}.pdf')


