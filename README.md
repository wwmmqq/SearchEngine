# This is my search engine

1. use python processe raw data

2. c++ for core search and use c++11

# current performance

	预处理时间: 1380s
	预处理后数据集大小: (原来298MB)
	索引建立时间: 44 s
	索引文件大小:
	检索时间(35 query):
		共: 4.87539 s
		平均:0.139297 s
	p10: 0.485714
	map@1000: 0.3100

	
## main idea
1. 用Trie树存储所有关键字, 查询关键字的复杂度为O(关键字长度)

2. Trie 树节点中的 DOCID 指针指向与此word的相关信息

3. 搜索变成在Trie树种搜索词，并使用BM25算法计算搜索出的文档的得分

4. 排序，输出

# Todo:
1. index compress
2. improve BM25