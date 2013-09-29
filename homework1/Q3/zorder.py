#!/usr/bin/env python

import sys, getopt

def zorder(argv):
	try: 
		opts, args = getopt.getopt(argv, "hn") 
	except getopt.GetoptError:
		print 'Usage: zorder -n <order-of-curve> <xvalue> <yvalue>\n'
		sys.exit()
	if len(argv) < 4:
		print 'Usage: zorder -n <order-of-curve> <xvalue> <yvalue>\n'
		sys.exit()
	for opt in opts:
		if opt == '-h':
			print 'Usage: zorder -n <order-of-curve> <xvalue> <yvalue>\n'
			sys.exit()

	order = int(float(args[0]))
	xvalue = int(float(args[1]))
	yvalue = int(float(args[2]))
	if xvalue >=  pow(2, order) or yvalue >= pow(2, order):
		print 'xvalue or yvalue exceeds the order of curve\n'
		sys.exit()
	
	value = 0
	for i in range(0, order):
		ybit = (yvalue >> 1 << 1) ^ yvalue
		xbit = (xvalue >> 1 << 1) ^ xvalue
		value = value + (ybit << (2 * i )) + (xbit << (2 * i + 1))
		xvalue = xvalue >> 1
		yvalue = yvalue >> 1
	print value
		

if __name__ == "__main__":
	zorder(sys.argv[1:])

