# -*- coding: utf-8 -*-
import time

import os
import json
import string
import xml.etree.ElementTree as ET
import nltk 

import sys
reload(sys)  
sys.setdefaultencoding('utf-8') 

#raw_data_path = '/home/wmq/Desktop/SearchEngine/data/trec_data1_renameCheckSum'
#processed_data_save_path = '/home/wmq/Desktop/SearchEngine/data/processed'

raw_data_path = '/Users/wmq/Desktop/coding2016/SearchEngine/data/trec_data1_renameCheckSum'
processed_data_save_path = '/Users/wmq/Desktop/coding2016/SearchEngine/data/processed'
mytokenizer = nltk.data.load('tokenizers/punkt/english.pickle')
from nltk.tokenize import word_tokenize
stopw = nltk.corpus.stopwords.words('english')

delEStr = string.punctuation + string.digits
identify = string.maketrans(delEStr, ' '*len(delEStr))

#from nltk.stem.porter import PorterStemmer
#from nltk.stem.lancaster import LancasterStemmer
#from nltk.stem import SnowballStemmer

def mkdir(path):
	path = path.strip()
	path = path.rstrip("/")
	if os.path.exists(path):
		print "exists path"
	else:
		print "creat path: " + path
		os.makedirs(path)
		

def lsdir_xml(dir_file):
	filenames = os.listdir(dir_file)
	filenames = [x for x in filenames if os.path.splitext(x)[1] == '.xml']
	filenames.sort()
	return filenames

def abstract(file_path=''):
	tree = ET.parse(file_path)
	root = tree.getroot()
	#for child in root:
	#	print child.tag

	for subtype in root.iter('downlaod_time'):
		d_time = str(subtype.text)

	for subtype in root.iter('subtype'):
		subtype_text =  str(subtype.text)

	for subtype in root.iter('chief_complaint'):
		chief_complaint_text = str(subtype.text)

	for report in root.iter('report_text'):
		report_text = str(report.text)


	return d_time, subtype_text, chief_complaint_text, report_text


def write_result(result_path, result):

	with open(result_path, 'w') as f:
		#time
		f.write("%s\n"%result[0])

		for items in result[1:]:
			for item in items:
				f.write("%s " % item)
			f.write("\n")

def sentence_token(raw):
	return mytokenizer.tokenize(raw)

def words_token(sent):
	words = []
	try:
		words = nltk.word_tokenize(sent)
	except Exception, e:
		print "err"
	else:
		pass
	finally:
		pass

	return words


def clean_lines(line):
	'''
	'''
	identify = string.maketrans('', '')  
	delEStr = string.punctuation + string.digits  #ASCII 标点符号，数字 
	cleanLine = line.translate(identify, delEStr) #去掉ASCII 标点符号和空格
	return cleanLine

def clean_words(words):
	'''stopwords, 长度小于3, 小写化
	''' 

	cwords = []
	for w in words:
		w = w.lower()
		if w not in stopw and len(w) > 2:
			cwords.append(w)

	return cwords

def stemming(filtered):
	'''Stemming is an attempt to reduce a word to its stem or root form.
	'''
	
	stemmed = set([])
	for f in filtered:
		#stemmed.add(PorterStemmer().stem(f))
		#stemmed.add(LancasterStemmer().stem(f))
		stemmed.add(nltk.stem.SnowballStemmer('english').stem(f))
	return list(stemmed)


def test():
	os.chdir(raw_data_path)
	d_time, subtype_text, chief_complaint_text, report_text = abstract('report100038.xml')
	words_subtype = words_token(report_text)
	words_subtype = clean_words(words_subtype)
	words_subtype = stemming(words_subtype)
	print words_subtype[:10]

def main():
	time1 = time.time()

	mkdir(processed_data_save_path)
	files = lsdir_xml(raw_data_path)
	print len(files)
	return;
	cwd = os.getcwd()
	print "current work dir: " + cwd

	os.chdir(raw_data_path)

	number = 0

	for onefile in files:
		number = number + 1
		d_time, subtype_text, chief_complaint_text, report_text = abstract(onefile)
		words_subtype = []
		words_report = []

		subtype_text = (subtype_text + ' ' + chief_complaint_text).lower()

		if(len(subtype_text) != 0):
			subtype_text = subtype_text.translate(identify, '')
			

			words_subtype = words_token(subtype_text)

			#words_subtype = clean_words(words_subtype)

			#words_subtype = stemming(words_subtype)

		if(len(report_text) != 0):
			report_text = report_text.translate(identify, '')
			words_report = words_token(report_text)

			words_report = clean_words(words_report)

			if (len(words_report) != 0):
				words_report = stemming(words_report)

		file_path = processed_data_save_path+'/'+onefile.split('.')[0] +'.txt';
		write_result(file_path, [d_time, words_subtype, words_report])

		print "Did: "% number

	time2 = time.time()
	print 'cost time' + str( (time2 - time1)/60 ) 
if __name__ == '__main__':
	main()