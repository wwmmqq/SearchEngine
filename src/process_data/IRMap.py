# -*- coding: utf-8 -*-


def readStandard(filename='../11qrels_visit_checksum.txt'):
	'''
	'''
	standard = set([])
	with open(filename) as f:
		for line in f:
			t = line.split(' ')
			if t[3] != '0':
				iterm = (t[0]+t[5]).strip()
			 	standard.add(iterm)
	return standard

def readMyResult(filename='../QQ.txt'):
	'''
	'''
	result = []
	with open(filename) as f:
		for line in f:
			t = line.split(' ')
			iterm = (t[0]+t[2]).strip()
			result.append(iterm)
	return result


def IRMap(N=11, Q=35):
	standard = readStandard()
	myresult = readMyResult()

	SC = []

	qi = 0
	while qi < Q:
		score = 0.0
		index = 0
		ii = 0

		while index < N:
			i = qi*N + index

			if (myresult[i] in standard):
				score += (ii + 1.0)/(index+1)
				ii = ii + 1
			index = index + 1

		if ii != 0:
			score = score / ii
		else:
			print "error: ii == 0, and score is 0.0"

		SC.append(score)
		qi = qi + 1;

	print sum(SC)/len(SC)

def main():
	IRMap()


if __name__ == '__main__':
	main()