#!/usr/bin/env python

from izorder import izorder
import matplotlib.pyplot as plt

x , y = [], []

for i in range(0, 128 * 128):
	tmp1, tmp2 = izorder(i,7)
	x.append(tmp1)
	y.append(tmp2)

fig = plt.plot(x,y)
plt.axis('off')
plt.savefig("zorder.png",bbox_inches='tight',dpi=500)

# Draw the plot to the screen
plt.show()
