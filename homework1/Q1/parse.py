#!/usr/bin/env python

with open('movies-short.txt','r') as fin:
	with open('movies-short.csv','w') as fout:

		cnt = 0

		for line in fin:
			p = line.find(':')
			if p != -1:
				cnt = cnt + 1
				if cnt != 5:
					fout.write(line[p+2:-1] + ',')
				else:
					fout.write(line[p+2:-1] + '\n')
					cnt = 0

