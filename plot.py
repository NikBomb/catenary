import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
data = pd.read_csv('res.csv')
x = data['x'].tolist()
y = data['y'].tolist()

len = 0

# Extract x and y values from the CSV data
plt.plot(x,y)
plt.show()