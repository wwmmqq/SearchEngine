# -*- coding: utf-8 -*-
standard = set([])
result = []


def readStandard(filename='/home/wmq/Desktop/SearchEngine/data/query/11qrels_visit_checksum.txt'):
	'''
	'''
	with open(filename) as f:
		for line in f:
			t = line.split(' ')
			if t[3] != '0':
				iterm = (t[0]+t[5]).strip()
			 	standard.add(iterm)
	return standard

def readMyResult(filename='/home/wmq/Desktop/SearchEngine/data/query/QQ_1000.txt'):
	'''
	'''
	with open(filename) as f:
		for line in f:
			t = line.split(' ')
			iterm = (t[0]+t[2]).strip()
			result.append(iterm)
	return result

def P_10(N=1000, Q=35):
	'''
		N : your answer number
		Q : your query number
	'''
	print "***** p10 ******"
	hit = 0
	qi = 0
	p10 = 0.0
	while qi < Q:
		index = 0
		while index < 10:
			i = qi*N + index
			if result[i] in standard :
				hit += 1
			index += 1
		p10 += hit/10.0
		hit = 0
		qi += 1
	print 'P10 : '+ str(p10 / Q)


def IRMap(N=1000, Q=35):
	print "***** map ******"
	SC = []

	qi = 0
	while qi < Q:
		score = 0.0
		index = 0
		ii = 0

		while index < N:
			i = qi*N + index

			if (result[i] in standard):
				score += (ii + 1.0)/(index+1)
				ii = ii + 1
			index = index + 1

		if ii != 0:
			score = score / ii
		else:
			print "error: ii == 0, and score is 0.0"

		SC.append(score)
		qi += 1;

	zerocnt = 0
	for x in SC:
		if x == 0:
			zerocnt += 1
	print 'map@' + str(N) + ' : ' + str(sum(SC)/(len(SC)-zerocnt))

def main():
	standard = readStandard()
	result = readMyResult()

	P_10(1000, 35)
	IRMap(1000, 35)

if __name__ == '__main__':
	main()