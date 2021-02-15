import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

df = pd.read_csv("results.csv", index_col=0)

fig, ax = plt.subplots(figsize=(6, 4))
ax.axis('tight')
ax.axis('off')
the_table = ax.table(cellText=df.values, rowLabels=df.index, colLabels=df.columns, loc='center', cellLoc='center')

pp = PdfPages("table.pdf")
pp.savefig(fig, bbox_inches='tight')
pp.close()

