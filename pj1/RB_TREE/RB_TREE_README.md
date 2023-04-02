# 一、简易说明

​        此红黑树程序源代码为*rb_tree.cpp*，该源代码主要内容为两个类——**RBTree**类和**RBExec**类。其中，**RBTree**类利用模板设计了通用的红黑树模型，对外提供了红黑树的插入、更新、删除、搜索、展示、重置功能，为题目要求的实现作基础；**RBExec**类作为程序的功能引擎，通过类的组合，将**RBTree**类声明模板类型的对象作为其成员，此时可进一步实现以文件为单位进行的初始化、插入、删除操作，同时将其他题目要求操作进行包装，对外只提供总运行函数*exec()*。*main()*函数创建一个**RBExec**类对象*a*，并且运行*a.exec()*。

​		该源代码文件采用UTF-8编码；利用g++编译器进行编译，生成了exe可执行程序。

# 二、使用说明

​		红黑树程序进入界面如下图所示，此时需要用户根据图中指令数字对应关系，输入数字，进行相应操作，无效输入会报错；每个要求的操作完成后，按任意键后界面会重新回到原始状态，等待用户输入。

![](https://i.loli.net/2021/11/30/xAsG2Jd3nyjpvWQ.png)





# 三、设计细节

​        在通用红黑树设计过程中，开始是将node结构的定义放在了红黑树类的外面，但后来选择将node结构定义在红黑树类的内部，因为node结构只在红黑树内部应用，这样保证了node结构的不被暴露。

​        在红黑树类中，如同算法导论上给出的方法，定义哨兵*nil*结点，而非沿用二叉搜索树的NIL进行边界条件的处理。

​        红黑树中基本的插入和删除算法均采用算法导论上的分类方式进行设计。

​	    **RBExec**类是程序的运行类，是对Key、Val类型确定的红黑树功能的再次包装：

```c++
class RBExec
{
    typedef string Word;
    typedef string FileName;

    RBTree<Word, WordInfo> rb;

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
    RBExec();
    void exec();
};

```



# 四、感悟想法

在此次PJ中，红黑树与B树的要求实现是相同的，代码方面也有许多重复代码，所以，在写法上，可以将二者继承于同一基类，在总运行函数中对二者进行选择，这样可以使最后代码成果更为统一。

