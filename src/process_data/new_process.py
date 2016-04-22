# -*- coding:utf-8 -*-
import sys
import os
import re
import time
import string
from nltk.corpus import stopwords
from nltk.stem.lancaster import LancasterStemmer
from tqdm import *
import xml.etree.ElementTree as ET

reload(sys)
sys.setdefaultencoding('utf-8')

query_path = '/home/wmq/Desktop/SearchEngine/data/query'
raw_data_path = '/home/wmq/Desktop/SearchEngine/data/trec_data1_renameCheckSum'
processed_data_path = '/home/wmq/Desktop/SearchEngine/data/processed'

STOPW = stopwords.words('english')

def mkdir(path):
	path = path.strip()
	path = path.rstrip("/")
	if os.path.exists(path):
		print "exists path"
	else:
		print "creat path: " + path
		os.makedirs(path)

def add_stopwords():
	for x in string.lowercase:
		STOPW.append(unicode(x*3))

def file_list(dir):
	filenames = []
	for f in os.listdir(dir):
		if os.path.isfile(dir+'/'+f) and os.path.splitext(f)[1] == '.xml':
			filenames.append(f)
	return filenames

def abstract(file):
	result = []
	tree = ET.parse(file)
	root = tree.getroot()
	#for child in root:
	#	print child.tag
	for mytext in root.iter('subtype'):
		result.append(str(mytext.text))

	for mytext in root.iter('type'):
		result.append(str(mytext.text))

	for mytext in root.iter('chief_complaint'):
		result.append(str(mytext.text))

	for mytext in root.iter('report_text'):
		result.append(str(mytext.text))

	return result


def remove_pd(text):
	r = '[' + string.punctuation + string.digits + ']+'
	return re.sub(r, ' ', text)

def wash_raw(fileList):
	st = LancasterStemmer()
	
	for f in tqdm(fileList):
		with open(processed_data_path + '/' + f,'w') as fw:
			file_text = abstract(raw_data_path + '/' + f)
			for line in file_text:
				line = remove_pd(line).lower()
				words = [unicode(st.stem(x)) for x in line.split() if len(x)!=0 and x not in STOPW]
				if len(words) != 0 :
					for x in words:
						if x != words[-1]:
							fw.write(x + ' ')
					fw.write(words[-1])
				fw.write('\n')

def wash_query():
	st = LancasterStemmer()

	with open(query_path+'/'+'0601_2011.txt', 'r') as fr, open(query_path+'/'+'washedquery.txt','w') as fw:
		for line in fr:
			line = remove_pd(line).lower()
			words = [unicode(st.stem(x)) for x in line.split() if len(x)!=0 and x not in STOPW]
			
			#fw.write(q_id + ' ')
			for x in words:
				if x != words[-1]:
					fw.write(x + ' ')
			fw.write(words[-1])
			fw.write('\n')

def main():
	mkdir(processed_data_path)

	time1 = time.time()
	filenList = file_list(raw_data_path)
	wash_raw(filenList)
	wash_query()
	time2 = time.time()
	print 'cost time' + str( (time2 - time1)/60 ) #cost time 24.1088325977 min

if __name__ == '__main__':
	main()