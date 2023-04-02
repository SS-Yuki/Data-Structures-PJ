#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int T = 3;
const int KeyMax = 2 * T - 1;

template <class Key, class Val>
class BTree
{
    //结点结构
    struct Node
    {
        int key_num;
        bool leaf;
        Key key[KeyMax];
        Val val[KeyMax];
        Node* child[CHAR_MAX] = {NULL};
        Node() : key_num(0), leaf(true) {};
    };

    //定位Key位置的结构
    struct AddressKey
    {
        Node* node;
        int order;
        AddressKey(Node* node_, int order_) : node(node_), order(order_) {}
        AddressKey() {}
    };

    Node* root_;

    AddressKey BTreeSearch(Node* x, Key key_);
    void BTreeSplit(Node* x, int i);
    void BTreeInsertNonfull(Node* x, Key key_, Val val_);
    void BTreeAdjust(Node* &x, int &i);
    void BTreeSubDelete(Node* x, Key key_);
    void BTreePrint(Node* x);
    void BTreeClearSubtree(Node* x);

public:
    BTree();
    ~BTree();
    void BTreeInsert(Key key_, Val val_);
    void BTreeUpdate(Key key_, Val val_);
    void BTreeDelete(Key key_);
    void BTreeSearchShow(Key key_);
    void BTreeShow();
    void BTreeClear();
};

template <class Key, class Val>
typename BTree<Key, Val>::AddressKey BTree<Key, Val>::BTreeSearch(Node* x, Key key_) {
    int i = 0;
    while (i < x -> key_num && key_ > x -> key[i]) {
        i++;
    }
    if (i < x -> key_num && key_ == x -> key[i]) {
        return AddressKey(x, i);
    }
    else if (x -> leaf) {
        return AddressKey(NULL, -1);
    }
    else {
        return BTreeSearch(x -> child[i], key_);
    }  
}

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

//自上而下递归插入
template <class Key, class Val>
void BTree<Key, Val>::BTreeInsertNonfull(Node* x, Key key_, Val val_) {
    int i = x -> key_num - 1;
    if (x -> leaf) {
        while (i >= 0 && key_ < x -> key[i]) {
            x -> key[i + 1] = x -> key[i]; x -> val[i + 1] = x -> val[i];
            i--;
        }
        x -> key[i + 1] = key_; x -> val[i + 1] = val_;

        x -> key_num++;
    }
    else {
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

//删除过程中保证含有Key值的子树的根存在至少T个关键字
template <class Key, class Val>
void BTree<Key, Val>::BTreeAdjust(Node* &x, int &i) {
    Node* i_node = x -> child[i];
    int i_ori_n = x -> child[i] -> key_num; 
    
    if (i > 0 && x -> child[i - 1] -> key_num >= T) {       //左兄弟关键字个数>=T
        Node* i_left_node = x -> child[i - 1];
        int i_left_ori_n = x -> child[i - 1] -> key_num;
        
        for (int j = i_ori_n; j > 0; j--) {
            i_node -> key[j] = i_node -> key[j - 1]; i_node -> val[j] = i_node -> val[j - 1];
        }
        i_node -> key[0] = x -> key[i - 1]; i_node -> val[0] = x -> val[i - 1];
        if (!i_node -> leaf) {
            for (int j = i_ori_n + 1; j > 0; j--) {
                i_node -> child[j] = i_node -> child[j - 1];
            }
            i_node -> child[0] = i_left_node -> child[i_left_ori_n];
        }
        i_node -> key_num++;

        x -> key[i - 1] = i_left_node -> key[i_left_ori_n - 1]; x -> val[i - 1] = i_left_node -> val[i_left_ori_n - 1];

        i_left_node -> key_num--;
    }
    else if (i < x -> key_num && x -> child[i + 1] -> key_num >= T) {   //右兄弟关键字个数>=T
        Node* i_right_node = x -> child[i + 1];
        int i_right_ori_n = x -> child[i + 1] -> key_num;
        
        i_node -> key[i_ori_n] = x -> key[i]; i_node -> val[i_ori_n] = x -> val[i];
        if (!i_node -> leaf) {
            i_node -> child[i_ori_n + 1] = i_right_node -> child[0];
        }
        i_node -> key_num++;

        x -> key[i] = i_right_node -> key[0]; x -> val[i] = i_right_node -> val[0];

        for (int j = 0; j < i_right_ori_n - 1; j++) {
            i_right_node -> key[j] = i_right_node -> key[j + 1]; i_right_node -> val[j] = i_right_node -> val[j + 1]; 
        }
        if (!i_right_node -> leaf) {
            for (int j = 0; j < i_right_ori_n; j++) {
                i_right_node -> child[j] = i_right_node -> child[j + 1]; 
            }
        }
        i_right_node -> key_num--;

    }
    else {                                  //所有相邻兄弟都只有T-1个关键字
        if (i > 0) {
            Node* i_left_node = x -> child[i - 1];
            int i_left_ori_n = x -> child[i - 1] -> key_num;

            i_left_node -> key[i_left_ori_n] = x -> key[i - 1]; i_left_node -> val[i_left_ori_n] = x -> val[i - 1];
            for (int j = 0; j < i_ori_n; j++) {
                i_left_node -> key[i_left_ori_n + j + 1] = i_node -> key[j]; i_left_node -> val[i_left_ori_n + j + 1] = i_node -> val[j];
            }
            if (!i_node -> leaf) {
                for (int j = 0; j < i_ori_n + 1; j++) {
                    i_left_node -> child[i_left_ori_n + j + 1] = i_node -> child[j];
                }
            }
            i_left_node -> key_num = KeyMax;

            for (int j = i - 1; j < x -> key_num - 1; j++) {
                x -> key[j] = x -> key[j + 1]; x -> val[j] = x -> val[j + 1];
            }
            for (int j = i; j < x -> key_num; j++) {
                x -> child[j] = x -> child[j + 1];
            }
            x -> key_num--;

            delete i_node;
            i--;
        }

        else {
            Node* i_right_node = x -> child[i + 1];
            int i_right_ori_n = x -> child[i + 1] -> key_num;

            i_node -> key[i_ori_n] = x -> key[i]; i_node -> val[i_ori_n] = x -> val[i];
            for (int j = 0; j < i_right_ori_n; j++) {
                i_node -> key[i_ori_n + j + 1] = i_right_node -> key[j]; i_node -> val[i_ori_n + j + 1] = i_right_node -> val[j];
            }
            if (!i_node -> leaf) {
                for (int j = 0; j < i_right_ori_n + 1; j++) {
                    i_node -> child[i_ori_n + j + 1] = i_right_node -> child[j];
                }
            }
            i_node -> key_num = KeyMax;

            for (int j = i; j < x -> key_num - 1; j++) {
                x -> key[j] = x -> key[j + 1]; x -> val[j] = x -> val[j + 1];
            }
            for (int j = i + 1; j < x -> key_num; j++) {
                x -> child[j] = x -> child[j + 1];
            }
            x -> key_num--;

            delete i_right_node;
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

//递归删除操作
template <class Key, class Val>
void BTree<Key, Val>::BTreeSubDelete(Node* x, Key key_) {
    int i = x -> key_num - 1;
    while (i >= 0 && key_ < x -> key[i]) {
        i--;
    }
    //x是叶结点
    if (x -> leaf) {                        
        if (i < 0 || (i >=0 && key_ != x -> key[i])) {
            cout << "key " << key_ << " missing" << endl;
            return;
        }
        for (int j = i; j < x -> key_num - 1; j++) {
            x -> key[j] = x -> key[j + 1]; x -> val[j] = x -> val[j + 1];
        }
        x -> key_num--;
        return;
    }
    //x不是叶结点
    if (i >=0 && key_ == x -> key[i]) {         //Key值在内部结点x上
        Node* y = x -> child[i];
        Node* z = x -> child[i + 1];
        if (y -> key_num >= T) {
            Node* tem = y;
            while (!tem -> leaf) {
                tem = tem -> child[tem -> key_num];
            }
            x -> key[i] = tem -> key[tem -> key_num - 1]; x -> val[i] = tem -> val[tem -> key_num - 1];

            BTreeSubDelete(y, x -> key[i]);
        }

        else if (z -> key_num >= T) {
            Node* tem = z;
            while (!tem -> leaf) {
                tem = tem -> child[0];
            }
            x -> key[i] = tem -> key[0]; x -> val[i] = tem -> val[0];

            BTreeSubDelete(z, x -> key[i]);
        }

        else {
            y -> key[y -> key_num] = x -> key[i]; y -> val[y -> key_num] = x -> val[i];
            for (int j = 0; j < z -> key_num; j++) {
                y -> key[y -> key_num + j + 1] = z -> key[j]; y -> val[y -> key_num + j + 1] = z -> val[j];
            }
            if (!y -> leaf) {
                for (int j = 0; j < z -> key_num + 1; j++) {
                    y -> child[y -> key_num + j + 1] = z -> child[j];
                }
            }
            y -> key_num = KeyMax;

            for (int j = i; j < x -> key_num - 1; j++) {
                x -> key[j] = x -> key[j + 1]; x -> val[j] = x -> val[j + 1];
            }
            for (int j = i + 1; j < x -> key_num; j++) {
                x -> child[j] = x -> child[j + 1];
            }
            x -> key_num--;

            if (x -> key_num == 0 && x == this -> root_) {
                this -> root_ = y;
            }
            delete z;
            BTreeSubDelete(y, key_);
        }
    }
    else {                          //Key值不在内部结点x上
        i++;
        if (x -> child[i] -> key_num == T - 1) {
            BTreeAdjust(x, i);
            (i == 2 * T) ? BTreeSubDelete(x, key_) : BTreeSubDelete(x -> child[i], key_);
        }
        else {
            BTreeSubDelete(x -> child[i], key_); 
        }
        
    }
    
}

//递归打印操作
template <class Key, class Val>
void BTree<Key, Val>::BTreePrint(Node* x) {
    for (int j = 0; j < x -> key_num; j++) {
        if (!x -> leaf) {
            BTreePrint(x -> child[j]);
        }
        cout << x -> key[j] << " - " << x -> val[j] << endl;
    }
    if (!x -> leaf) {
        BTreePrint(x -> child[x -> key_num]);
    }
    
}

//子树清除函数
template <class Key, class Val>
void BTree<Key, Val>::BTreeClearSubtree(Node* x) {
    if (!x -> leaf) {
        for (int j = 0; j < x -> key_num + 1; j++) {
            BTreeClearSubtree(x -> child[j]);
        }
    }
    delete x;
}

template <class Key, class Val>
BTree<Key, Val>::BTree() {
    this -> root_ = new Node();
}

template <class Key, class Val>
BTree<Key, Val>::~BTree() {
    BTreeClearSubtree(this -> root_);
}

template <class Key, class Val>
void BTree<Key, Val>::BTreeInsert(Key key_, Val val_) {
    AddressKey ak = BTreeSearch(this -> root_, key_);
    if (ak.node != NULL) {
        cout << "key " << key_ << " conflict" << endl;
    }
    else {
        Node* r = this -> root_;
        if (r -> key_num == KeyMax) {
            Node* s = new Node();
            this -> root_ = s;
            s -> leaf = false;
            s -> key_num = 0;
            s -> child[0] = r;
            BTreeSplit(s, 0);
            BTreeInsertNonfull(s, key_, val_);
        }
        else {
            BTreeInsertNonfull(r, key_, val_);
        }
    }
}

template <class Key, class Val>
void BTree<Key, Val>::BTreeUpdate(Key key_, Val val_) {
    AddressKey ak = BTreeSearch(this -> root_, key_);
    if (ak.node != NULL) {
        ak.node -> val[ak.order] = val_;
    }
    else {
        BTreeInsert(key_, val_);
    }
}

template <class Key, class Val>
void BTree<Key, Val>::BTreeDelete(Key key_) {
    BTreeSubDelete(this -> root_, key_);
}

template <class Key, class Val>
void BTree<Key, Val>::BTreeSearchShow(Key key_) {
    AddressKey ak = BTreeSearch(this -> root_, key_);
    if (ak.node == NULL) {
        cout << "key " << key_ << " missing" << endl;
    }
    else {
        cout << ak.node -> val[ak.order] << endl;
    }
}

template <class Key, class Val>
void BTree<Key, Val>::BTreeShow() {
    BTreePrint(this -> root_);
}

template <class Key, class Val>
void BTree<Key, Val>:: BTreeClear() {
    BTreeClearSubtree(this -> root_);
    this -> root_ = new Node();
}


struct WordInfo
{
    char part_of_speech;
    string frequency;
}; 
ostream& operator<<(ostream& out, WordInfo& word_info) {
	out << "(" << word_info.part_of_speech << ", " << word_info.frequency << ")";
	return out;
}

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

void BExec::MunuPrint() {
    cout << "*********************************" << endl;
    cout << "****   1:Initialization      ****" << endl;
    cout << "****   2:Delete by command   ****" << endl;
    cout << "****   3:Delete by file      ****" << endl;
    cout << "****   4:Insert by command   ****" << endl;
    cout << "****   5:Insert by file      ****" << endl;
    cout << "****   6:Update by command   ****" << endl;
    cout << "****   7:Search              ****" << endl;
    cout << "****   8:Dump                ****" << endl;
    cout << "****  -1:Exit                ****" << endl;
    cout << "*********************************" << endl;
    cout << "Please input your choice: ";
}

void BExec::FileInit() {
    cout << "#Initialization" << endl;
    string init_file;
    cout << "Please input filename: ";
    cin >> init_file;
    cin.clear();
    cin.sync(); 

    ifstream ifs;
	ifs.open(init_file, ios::in);
	if (!ifs.is_open()) {
		cout << "file open failed." << endl;
        cout << "Please do something else." << endl;
		return;
	}

    b_tree.BTreeClear();

    string tem;
    getline(ifs, tem);

    Word word;
    WordInfo word_info;
    while (ifs >> word 
            && ifs >> word_info.part_of_speech
            && ifs >> word_info.frequency) {
        b_tree.BTreeInsert(word, word_info);
    }
    
    ifs.close();
}

void BExec::Delete() {
    cout << "#Delete by command" << endl;
    string word;
    cout << "Please input word: ";
    cin >> word;
    cin.clear();
    cin.sync(); 

    b_tree.BTreeDelete(word);
}

void BExec::FileDelete() {
    cout << "#Delete by file" << endl;
    string delete_file;
    cout << "Please input filename: ";
    cin >> delete_file;
    cin.clear();
    cin.sync(); 

    ifstream ifs;
	ifs.open(delete_file, ios::in);
	if (!ifs.is_open()) {
		cout << "file open failed" << endl;
        cout << "Please do something else." << endl;
		return;
	}
    string tem;
    Word word;
    while (getline(ifs, tem) 
            && ifs >> word) {
        b_tree.BTreeDelete(word);
    }
    
    ifs.close();
}

void BExec::Insert() {
    cout << "#Insert by command" << endl;
    Word word;
    cout << "Please input word: ";
    cin >> word;
    cin.clear();
    cin.sync(); 
    WordInfo word_info;
    cout << "Please input part of speech of the word: ";
    cin >> word_info.part_of_speech;
    cin.clear();
    cin.sync(); 
    cout << "Please input frequency of the word: ";
    cin >> word_info.frequency;
    cin.clear();
    cin.sync(); 

    b_tree.BTreeInsert(word, word_info);
}

void BExec::FileInsert() {
    cout << "#Insert by file" << endl;
    string insert_file;
    cout << "Please input filename: ";
    cin >> insert_file;
    cin.clear();
    cin.sync(); 

    ifstream ifs;
	ifs.open(insert_file, ios::in);
	if (!ifs.is_open()) {
		cout << "file open failed" << endl;
        cout << "Please do something else." << endl;
		return;
	}
    string tem;
    getline(ifs, tem);

    Word word;
    WordInfo word_info;
    while (ifs >> word 
            && ifs >> word_info.part_of_speech
            && ifs >> word_info.frequency) {
        b_tree.BTreeInsert(word, word_info);
    }
    
    ifs.close();
}

void BExec::Update() {
    cout << "#Update by command" << endl;
    Word word;
    cout << "Please input word: " << endl;
    cin >> word;
    cin.clear();
    cin.sync(); 
    WordInfo word_info;
    cout << "Please input part of speech of the word: ";
    cin >> word_info.part_of_speech;
    cin.clear();
    cin.sync(); 
    cout << "Please input frequency of the word: ";
    cin >> word_info.frequency;
    cin.clear();
    cin.sync(); 

    b_tree.BTreeUpdate(word, word_info);
}

void BExec::Search() {
    cout << "#Search" << endl;
    string word;
    cout << "Please input word: ";
    cin >> word;
    cin.clear();
    cin.sync(); 

    b_tree.BTreeSearchShow(word);
}

void BExec::Dump() {
    cout << "#Dump" << endl;

    b_tree.BTreeShow();
}

BExec::BExec() {}

void BExec::exec() {
    cout << "Welcome to the B-Tree!" << endl;
    while (true) {
        MunuPrint();
        int choice = 0;
        cin >> choice;
        cin.clear(); 
        cin.sync();
        switch (choice) {
            case -1 : exit(0); break;
            case 1 : FileInit(); break;
            case 2 : Delete(); break;
            case 3 : FileDelete(); break;
            case 4 : Insert(); break;
            case 5 : FileInsert(); break;
            case 6 : Update(); break;
            case 7 : Search(); break;
            case 8 : Dump(); break;
            default : cout << "Error! Please input again!" << endl; break;
        }
        cout << "Press any key to continue." << endl;
        getchar();
        system("cls");
    }
}

int main() {
   BExec a;
   a.exec();

   return 0;
}
