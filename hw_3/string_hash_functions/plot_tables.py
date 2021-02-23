import matplotlib.pyplot as plt
import numpy as np
import os
from matplotlib.backends.backend_pdf import PdfPages

data_directory = "data"
results_directory = "results"

for file_name in os.listdir(data_directory):
    if file_name.endswith(".csv") and file_name.startswith("uniform_test"):
        file_path = os.path.join(data_directory, file_name)
        data = np.genfromtxt(file_path, delimiter=',', dtype=int)
        unique, counts = np.unique(data, return_counts=True)

        name = file_name.partition("uniform_test_")[2].partition(".csv")[0]

        fig, ax = plt.subplots(figsize=(5, 5))
        ax.axis('off')
        the_table = ax.table(cellText=counts[np.newaxis, ...],
                             rowLabels=["Количество бакетов с\nзаданным числом элементов"],
                             colLabels=unique, loc='left', cellLoc='center')

        if not os.path.exists(results_directory):
            os.mkdir(results_directory)
        pp = PdfPages(f"{results_directory}/uniform_test_{name}.pdf")
        pp.savefig(fig, bbox_inches='tight')
        pp.close()


