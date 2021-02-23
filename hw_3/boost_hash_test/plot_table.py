import matplotlib.pyplot as plt
import numpy as np
from matplotlib.backends.backend_pdf import PdfPages

data = np.genfromtxt("results/uniform_test.csv", delimiter=',', dtype=int)

unique, counts = np.unique(data, return_counts=True)

fig, ax = plt.subplots(figsize=(5, 5))
ax.axis('off')
the_table = ax.table(cellText=counts[np.newaxis, ...],
                     rowLabels=["Количество бакетов с\nзаданным числом элементов"],
                     colLabels=unique, loc='left', cellLoc='center')

pp = PdfPages("results/uniform_test.pdf")
pp.savefig(fig, bbox_inches='tight')
pp.close()


