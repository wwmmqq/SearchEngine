# -*- coding: utf-8 -*-
import os
import json
import string
import xml.etree.ElementTree as ET
import nltk 

import sys
reload(sys)  
sys.setdefaultencoding('utf-8') 

raw_data_path = '/Users/wmq/Desktop/coding2016/SearchEngine/data/101'
processed_data_save_path = '/Users/wmq/Desktop/coding2016/SearchEngine/data/processed'

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

def abstract(file_path='../../data/101/report100038.xml'):
	tree = ET.parse(file_path)
	root = tree.getroot()
	#for child in root:
	#	print child.tag
	for subtype in root.iter('subtype'):
		subtype_text =  subtype.text

	for report in root.iter('report_text'):
		report_text = report.text
	return (subtype_text, report_text)

def write_result(result_path, result):

	with open(result_path, 'w') as f:
		for items in result:
			for item in items:
				f.write("%s " % item)
			f.write("\n")

def sentence_token(raw):
	mytokenizer = nltk.data.load('tokenizers/punkt/english.pickle')
	return mytokenizer.tokenize(raw)

def words_token(sent):
	words = nltk.word_tokenize(sent)
	return	words

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
	import nltk
	stopw = nltk.corpus.stopwords.words('english')
	identify = string.maketrans('', '')  
	delEStr = string.punctuation + string.digits  


	cwords = []
	for w in words:
		w = w.lower()
		w = w.translate(identify, delEStr)
		if w not in stopw and len(w) > 2:
			cwords.append(w)

	return cwords

def stemming(filtered):
	'''Stemming is an attempt to reduce a word to its stem or root form.

		Depending on the stemming algorithms, we have a slightly different stemming. 
	'''
	from nltk.stem.porter import PorterStemmer
	from nltk.stem.lancaster import LancasterStemmer
	from nltk.stem import SnowballStemmer 
	stemmed = set([])
	for f in filtered:
		#stemmed.add(PorterStemmer().stem(f))
		#stemmed.add(LancasterStemmer().stem(f))
		stemmed.add(SnowballStemmer('english').stem(f))
	return list(stemmed)


def test():
	os.chdir(raw_data_path)
	subtype_text, report_text = abstract('report100038.xml')
	words_subtype = words_token(report_text)
	words_subtype = clean_words(words_subtype)
	words_subtype = stemming(words_subtype)
	print words_subtype[:10]

def main():
	mkdir('/Users/wmq/Desktop/coding2016/SearchEngine/data/processed')
	files = lsdir_xml(raw_data_path)

	cwd = os.getcwd()
	os.chdir(raw_data_path)

	print len(files)
'''
	for onefile in files:
		subtype_text, report_text = abstract(onefile)

		words_subtype = words_token(subtype_text)

		words_subtype = clean_words(words_subtype)

		words_subtype = stemming(words_subtype)

		words_report = words_token(report_text)

		words_report = clean_words(words_report)
		
		words_report = stemming(words_report)

		file_path = processed_data_save_path+'/'+onefile.split('.')[0] +'.txt';
		write_result(file_path, [words_subtype,words_report])
	os.chdir(cwd)
'''
if __name__ == '__main__':
	main()