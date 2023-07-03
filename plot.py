import matplotlib.pyplot as plt
import pandas as pd
import os 

" https://stackoverflow.com/questions/37765197/darken-or-lighten-a-color-in-matplotlib "

def adjust_lightness(color, amount=0.5):
    import matplotlib.colors as mc
    import colorsys
    try:
        c = mc.cnames[color]
    except:
        c = color
    c = colorsys.rgb_to_hls(*mc.to_rgb(c))
    return colorsys.hls_to_rgb(c[0], max(0, min(1, amount * c[1])), c[2])

fileList = []
# Read the CSV file
for file in os.listdir("."):
    if file.endswith(".csv"):
        fileList.append(file)
        
fileList.sort()
size = len(fileList)



for idx,file in enumerate(fileList):
    data = pd.read_csv(file)
    x = data['x'].tolist()
    y = data['y'].tolist()

    # Extract x and y values from the CSV data
    plt.plot(x,y,color=adjust_lightness('b', float(idx)/float(size)))

plt.show()