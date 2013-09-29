#!/usr/bin/env python

import sys, getopt

def rot(order, xvalue, yvalue, xbit, ybit):
	x, y = xvalue, yvalue
	if ybit == 0:
		if xbit == 1:
			x = (1 << order) - 1 - x
			y = (1 << order) - 1 - y
		tmp = x
		x = y
		y = tmp
	return x, y

def ihorder(value, order):
	xvalue, yvalue = 0, 0
	for i in range(0, order):
		xbit = 1 & (value >> 1) 
		ybit = 1 & (xbit ^ value)
		xvalue, yvalue = rot(i, xvalue, yvalue, xbit, ybit)
		xvalue = xvalue + (xbit << i)
		yvalue = yvalue + (ybit << i)
		value = value >> 2
	return xvalue, yvalue
	

def main(argv):
	try: 
		opts, args = getopt.getopt(argv, "hn") 
	except getopt.GetoptError:
		print 'Usage: ihorder -n <order-of-curve> <value>\n'
		sys.exit()
	if len(argv) < 3:
		print 'Usage: ihorder -n <order-of-curve> <value>\n'
		sys.exit()
	for opt in opts:
		if opt == '-h':
			print 'Usage: ihorder -n <order-of-curve> <value>\n'
			sys.exit()

	order = int(float(args[0]))
	value = int(float(args[1]))
	if value >=  pow(2, 2 * order):
		print 'value exceeds the order of curve\n'
		sys.exit()

	xvalue, yvalue = ihorder(value, order)
	
	print xvalue, yvalue
		

if __name__ == "__main__":
	main(sys.argv[1:])

