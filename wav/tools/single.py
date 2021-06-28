import numpy as np
import matplotlib.pyplot as plt

data = open("../out/raw.txt", "r")

buffer = data.read()
buffer = buffer.splitlines()

data.close()

array = []
for s in buffer:
    array.append(float(s))

plt.plot([i for i in range(0,len(array))],array, color='b')
plt.show()