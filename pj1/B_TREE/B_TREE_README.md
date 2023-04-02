# 一、简易介绍

​		此B树程序源代码为*b_tree.cpp*，该源代码主要内容为两个类——**BTree**类和**BExec**类。其中，**BTree**类利用模板设计了通用的B树模型，对外提供了B树的插入、更新、删除、搜索、展示、重置功能，为题目要求的实现作基础；**BExec**类作为程序的功能引擎，通过类的组合，将**BTree**类声明模板类型的对象作为其成员，此时可进一步实现以文件为单位进行的初始化、插入、删除操作，同时将其他题目要求操作进行包装，对外只提供总运行函数*exec()*。*main()*函数创建一个**BExec**类对象*a*，并且运行*a.exec()*。

​        B树的最小度数设为3。

​		该源代码文件采用UTF-8编码；利用g++编译器进行编译，生成了exe可执行程序。



# 二、使用说明

​        B树程序进入界面如下图所示，此时需要用户根据图中指令数字对应关系，输入数字，进行相应操作，无效输入会报错；每个要求的操作完成后，按任意键后界面会重新回到原始状态，等待用户输入。

![](https://i.loli.net/2021/11/30/BQuhUKw74es13l6.png)



# 三、设计细节

​       在通用B树设计过程中，开始是将node结构的定义放在了B树类的外面，但后来选择将node结构定义在B树类的内部，因为node结构只在B树内部应用，这样保证了node结构的不被暴露。

​	    B树设计中插入和删除较为繁琐，二者具有的相似的一点是：通过”预先操作“，保证在B树在单程下行过程中完成操作（删除有例外情况）。

​	    插入操作有两大主要函数：*BTreeSplit*和*BTreeInsertNonfull*。其中，*BTreeSplit*可处理满子结点，保证后续可以直接插入：

```c++
//分裂B树结点
template <class Key, class Val>
void BTree<Key, Val>::BTreeSplit(Node* x, int i) {
    Node* z = new Node();
    Node* y = x -> child[i];
    z -> leaf = y -> leaf;
    z -> key_num = T - 1;
    for (int j = 0; j < T - 1; j++) {
        z -> key[j] = y -> key[j + T]; z -> val[j] = y -> val[j + T];
    }
    if (!y -> leaf) {
        for (int j = 0; j < T; j++) {
            z -> child[j] = y -> child[j + T];
        }
    }
    y -> key_num = T - 1;

    for (int j = x -> key_num; j > i; j--) {
        x -> child[j + 1] = x -> child[j];
    }
    x -> child[i + 1] = z;

    for (int j = x -> key_num - 1; j > i - 1; j--) {
        x -> key[j + 1] = x -> key[j]; x -> val[j + 1] = x -> val[j];
    }
    x -> key[i] = y -> key[T - 1]; x -> val[i] = y -> val[T - 1];

    x -> key_num++;
}
```

*BTreeInsertNonfull*则是在保证非满条件下通过分类递归地实现插入操作：



```c++
//自上而下递归插入
template <class Key, class Val>
void BTree<Key, Val>::BTreeInsertNonfull(Node* x, Key key_, Val val_) {
    int i = x -> key_num - 1;
    if (x -> leaf) {	//x是叶结点
        while (i >= 0 && key_ < x -> key[i]) {
            x -> key[i + 1] = x -> key[i]; x -> val[i + 1] = x -> val[i];
            i--;
        }
        x -> key[i + 1] = key_; x -> val[i + 1] = val_;

        x -> key_num++;
    }
    else {			//x不是叶结点
        while (i >= 0 && key_ < x -> key[i]) {
            i--;
        }
        i++;
        if (x -> child[i] -> key_num == KeyMax) {
            BTreeSplit(x, i);
            if (key_ > x -> key[i]) {
                i++;
            }
        }
        BTreeInsertNonfull(x -> child[i], key_, val_);
    }
}
```



​	  删除操作也主要有两大函数组成：*BTreeAdjust*和*BTreeSubDelete*。*BTreeAdjust*保证子结点关键字个数大于等于T，从而可以直接进行删除操作：

```c++
//删除过程中保证含有Key值的子树的根存在至少T个关键字
template <class Key, class Val>
void BTree<Key, Val>::BTreeAdjust(Node* &x, int &i) {
    Node* i_node = x -> child[i];
    int i_ori_n = x -> child[i] -> key_num; 
    
    //左兄弟关键字个数>=T
    if (i > 0 && x -> child[i - 1] -> key_num >= T) {       
        ...
    }
    //右兄弟关键字个数>=T
    else if (i < x -> key_num && x -> child[i + 1] -> key_num >= T) {   
        ...
    }
    //所有相邻兄弟都只有T-1个关键字
    else {                                  
        if (i > 0) {
            ...
        }

        else {
            ...
        }
        //若x失去一个关键字成为空结点后的处理
        if (x -> key_num == 0) {                
            Node* tem = x;
            x = this -> root_ = x -> child[0];
            i = 2 * T;
            delete tem;
        }
    }
}

```

*BTreeSubDelete*通过分类递归地实现删除操作：

```c++
//递归删除操作
template <class Key, class Val>
void BTree<Key, Val>::BTreeSubDelete(Node* x, Key key_) {
    int i = x -> key_num - 1;
    while (i >= 0 && key_ < x -> key[i]) {
        i--;
    }
    //x是叶结点
    if (x -> leaf) {                        
        return;
    }
    //x不是叶结点
    if (i >=0 && key_ == x -> key[i]) {         //Key值在内部结点x上
        Node* y = x -> child[i];
        Node* z = x -> child[i + 1];
        if (y -> key_num >= T) {
            ...
        }

        else if (z -> key_num >= T) {
            ...
        }

        else {
            ...
        }
    }
    else {                          //Key值不在内部结点x上
        ...
    }
    
}
```

 **BExec**类是程序的运行类，是对Key、Val类型确定的B树功能的再次包装：

```c++
class BExec
{
    typedef string Word;
    typedef string FileName;

    BTree<Word, WordInfo> b_tree;

    void MunuPrint();
    void FileInit();
    void Delete();
    void FileDelete();
    void Insert();
    void FileInsert();
    void Update();
    void Search();
    void Dump();

public:
    BExec();
    void exec();
};
```



# 四、感悟想法

​        B树的功能函数实现中，最启迪我的是在操作时为维护B树的性质做出的预先处理操作。如设计细节中所言，插入和删除都有类似的流程，总会有一步操作是处理==即将处理的子结点==，这样的细节实现使得插入（删除）操作可以直接进行，而不必担心对B树的性质造成破坏；同时，也就避免了B树修复过程中的上下往复读取，做到自上而下地完成操作。

​	    另一方面，在此次PJ中，红黑树与B树的要求实现是相同的，代码方面也有许多重复代码，所以，在写法上，可以将二者继承于同一基类，在总运行函数中对二者进行选择，这样可以使最后代码成果更为统一。



