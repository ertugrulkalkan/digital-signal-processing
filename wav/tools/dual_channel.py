import numpy as np
import math
import matplotlib.pyplot as plt

data = open("../out/raw.txt", "r")

buffer = data.read()
buffer = buffer.splitlines()

data.close()

array = []
for sample in buffer:
    array.append(float(sample))

plt.plot([i for i in range(0,math.ceil(len(array)/2))],array[:math.floor(len(array)/2)], color='b')
plt.plot([i for i in range(0,math.ceil(len(array)/2))],array[math.ceil(len(array)/2):], color='r')
plt.show()