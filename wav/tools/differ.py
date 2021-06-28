import numpy as np
import matplotlib.pyplot as plt

data_r = open("../out/raw.txt", "r")
data_f = open("../out/filtered.txt", "r")

raw_buffer = data_r.read()
raw_buffer = raw_buffer.splitlines()

fil_buffer = data_f.read()
fil_buffer = fil_buffer.splitlines()

data_r.close()
data_f.close()

array1 = []
for s in raw_buffer:
    array1.append(float(s))

array2 = []
for s in fil_buffer:
    array2.append(float(s))

plt.plot([i for i in range(0,len(array1))],array1, color='b', label="original")
plt.plot([i for i in range(0,len(array2))],array2, color='r', label="filtered")
plt.legend(loc="lower right")
plt.show()