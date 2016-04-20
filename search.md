# 使用数据结构

## Trie树
用于存储字典

## 数据结构
	struct Node {
		char chr; // eg: 'a','z'
		bool isleaf;
		node_map m_children; // 存储children
		std::map<unsigned int, doc_ptr> DOCID; // 文档信息指针
		}

	
	struct document {
	
		unsigned int D_ID;
		unsigned int freq; // tf
		char location = 'a'; // a 表示文章标题，b 文章内容
		unsigned int doc_len;// doc 长度
	};
	
## 思路
1. 用Trie树存储所有关键字, 查询关键字的复杂度为O(关键字长度)

2. Trie 树节点中的 DOCID 指针指向与此word的相关信息

3. 搜索变成在Trie树种搜索词，并使用BM25算法计算搜索出的文档的得分

4. 排序，输出
