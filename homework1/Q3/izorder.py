#!/usr/bin/env python

import sys, getopt

def izorder(value, order):
	xvalue, yvalue = 0, 0
	for i in range(0, order):
		ybit = (value >> 1 << 1) ^ value
		value = value >> 1
		xbit = (value >> 1 << 1) ^ value
		value = value >> 1
		xvalue = xvalue + (xbit << i)
		yvalue = yvalue + (ybit << i)
	return xvalue, yvalue
	

def main(argv):
	try: 
		opts, args = getopt.getopt(argv, "hn") 
	except getopt.GetoptError:
		print 'Usage: izorder -n <order-of-curve> <value>\n'
		sys.exit()
	if len(argv) < 3:
		print 'Usage: izorder -n <order-of-curve> <value>\n'
		sys.exit()
	for opt in opts:
		if opt == '-h':
			print 'Usage: izorder -n <order-of-curve> <value>\n'
			sys.exit()

	order = int(float(args[0]))
	value = int(float(args[1]))
	if value >=  pow(2, 2 * order):
		print 'value exceeds the order of curve\n'
		sys.exit()
	
	xvalue, yvalue = izorder(value, order) 
	print xvalue,yvalue
		

if __name__ == "__main__":
	main(sys.argv[1:])

