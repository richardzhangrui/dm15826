#!/usr/bin/env python

import sys, getopt

def rot(order, xvalue, yvalue, xbit, ybit):
	if ybit == 0:
		if xbit == 1:
			xvalue = ~xvalue
			yvalue = ~yvalue
		tmp = xvalue
		xvalue = yvalue
		yvalue = tmp
	return xvalue, yvalue

def horder(argv):
	try: 
		opts, args = getopt.getopt(argv, "hn") 
	except getopt.GetoptError:
		print 'Usage: horder -n <order-of-curve> <xvalue> <yvalue>\n'
		sys.exit()
	if len(argv) < 4:
		print 'Usage: horder -n <order-of-curve> <xvalue> <yvalue>\n'
		sys.exit()
	for opt in opts:
		if opt == '-h':
			print 'Usage: horder -n <order-of-curve> <xvalue> <yvalue>\n'
			sys.exit()

	order = int(float(args[0]))
	xvalue = int(float(args[1]))
	yvalue = int(float(args[2]))
	if xvalue >=  pow(2, order) or yvalue >= pow(2, order):
		print 'xvalue or yvalue exceeds the order of curve\n'
		sys.exit()
	
	value = 0
	for i in range(order - 1, -1, -1):
		ybit = ((1 << i) & yvalue) > 0
		xbit = ((1 << i) & xvalue) > 0
		value = value + (((xbit & 1) ^ ybit) << (2 * i )) + (xbit << (2 * i + 1))
		xvalue, yvalue = rot(i, xvalue, yvalue, xbit, ybit)
	print value
		

if __name__ == "__main__":
	horder(sys.argv[1:])

