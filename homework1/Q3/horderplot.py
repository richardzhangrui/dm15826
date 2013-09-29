#!/usr/bin/env python

from ihorder import ihorder
import matplotlib.pyplot as plt

x , y = [], []

for i in range(0, 128 * 128):
	tmp1, tmp2 = ihorder(i,7)
	x.append(tmp1)
	y.append(tmp2)

fig = plt.plot(x,y)
plt.axis('off')
plt.savefig("horder.png",bbox_inches='tight',dpi=500)

# Draw the plot to the screen
plt.show()
