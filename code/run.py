import numpy as np
import os

overall = 0
ping = []
for a in range(108, 112):
    for b in range(256):
        with open("166_%03d_%03d.txt" % (a, b), 'r') as fin:
            t = fin.readline()
            s = int(t.split()[0])
            ping.append(float(t.split()[1]))
            overall += s
print(overall, sum(ping) / len(ping))