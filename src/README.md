# playWithTrieTree

A better example:  
http://songlee24.github.io/2015/05/09/prefix-tree/  
Including other useful stuff, like multithreading, etc

Extra:  
Aho-Corasick automation(AC自动机)  
http://www.cppblog.com/mythit/archive/2009/04/21/80633.html

Extra:
Prefix Tree to Sufix Tree:  
http://taop.marchtea.com/08.06.html

Trie Tree缺点是内存消耗太大。但是，如果使用自动机，就可以在保持Trie树的功能与性能的同时，大大降低内存用量（十几倍，甚至几十倍，几百倍），但是因为这样的自动机实现太困难，工业上的应用很少。不过，现在已经有成熟的实现了：把自动机用作 Key-Value 存储

优化:  
一般都在说 patricia tree, 它的 bitwise版本也不错，叫做crit-bit tree。

这种数据结构进阶路线为:  
字典树-> 后缀树->patricia tree-> crit-bit tree

An idea:  
Use DP theory to implement an advanced Trie Tree, to keep memory as low as possible  
Already exists? http://dongxicheng.org/structure/trietree/


### function pointer(函数指针)  
member function pointer  
If function instance is no in static:  
(1)have to use class name when declare the function void (Trie::*handleNode)(const NodePtr current_node)  
(2)have to use this-> when using this function pointer   
(3)have to use reference when use the function (&Trie::saveWord)  

总结：如果函数实例没有定义成static形式,那么：   
(1)那么函数指针的声明得加类空间名字；  
(2)在函数实例使用者内部得用this->来引用到函数指针   
(3)在函数实例使用者的参数处得使用 CTestFun:来引用函数实例。  
awesome approach:  http://mmdev.iteye.com/blog/1568412  
