#include <iostream>
#include <string>
#include <fstream>
using namespace std;


template <class Key, class Val>
class RBTree
{
    enum Color {RED, BLACK};
    //结点结构
    struct Node
    {
        Key key;
        Val val;
        Color color;
        Node* left;
        Node* right;
        Node* parent;
        Node(Key _key, Val _val) : key(_key), val(_val), color(BLACK), left(NULL), right(NULL), parent(NULL) {}
        Node() : color(BLACK), left(NULL), right(NULL), parent(NULL) {}
    };
    Node* root_;
    Node* nil_;

    void LeftRotate(Node* x);
    void RightRotate(Node* x);
    void RBInsertFixup(Node* z);
    void RBTransplant(Node* x, Node* y);
    void RBDeleteFixup(Node* x);
    Node* TreeMin(Node* x);
    Node* TreeSearch(Node* x, Key key_);
    void RBPrint(Node* x);
    void RBClearSubtree(Node* x);

public:
    RBTree();
    ~RBTree();
    void RBInsert(Key key_, Val val_);
    void RBUpdate(Key key_, Val val_);
    void RBDelete(Key key_);
    void RBSearchShow(Key key_);
    void RBShow();
    void RBClear();
};

template <class Key, class Val>
void RBTree<Key, Val>:: LeftRotate(Node* x) {
    Node* y = x -> right;
    x -> right = y -> left;
    if (y -> left != this -> nil_) {
        y -> left -> parent = x;
    }
    y -> parent = x -> parent;
    if (x -> parent == this -> nil_) {
        this -> root_ = y;
    } 
    else if (x == x -> parent -> left) {
        x -> parent -> left = y;
    }
    else {
        x -> parent -> right = y;
    }
    y -> left = x;
    x -> parent = y;
}

template <class Key, class Val>
void RBTree<Key, Val>::RightRotate(Node* x) {
    Node* y = x -> left;
    x -> left = y -> right;
    if (y -> right != this -> nil_) {
        y -> right -> parent = x;
    }
    y -> parent = x -> parent;
    if (x -> parent == this -> nil_) {
        this -> root_ = y;
    } 
    else if (x == x -> parent -> left) {
        x -> parent -> left = y;
    }
    else {
        x -> parent -> right = y;
    }
    y -> right = x;
    x -> parent = y;
}

template <class Key, class Val>
void RBTree<Key, Val>::RBInsertFixup(Node* z) {
    while (z -> parent -> color == RED) {
        if (z -> parent == z -> parent -> parent -> left) {
            Node* y = z -> parent -> parent -> right;
            if (y -> color == RED) {
                z -> parent -> color = BLACK;
                y -> color = BLACK;
                z -> parent -> parent -> color = RED;
                z = z -> parent -> parent; 
            }

            else {
                if (z == z -> parent -> right) {
                    z = z -> parent;
                    LeftRotate(z);
                }
                z -> parent -> color = BLACK;
                z -> parent -> parent -> color = RED;
                RightRotate(z -> parent -> parent);
            }
        }

        else {
            Node* y = z -> parent -> parent -> left;
            if (y -> color == RED) {
                z -> parent -> color = BLACK;
                y -> color = BLACK;
                z -> parent -> parent -> color = RED;
                z = z -> parent -> parent; 
            }

            else {
                if (z == z -> parent -> left) {
                    z = z -> parent;
                    RightRotate(z);
                }
                z -> parent -> color = BLACK;
                z -> parent -> parent -> color = RED;
                LeftRotate(z -> parent -> parent);
            }
        }
    }
    this -> root_ -> color = BLACK;
}

template <class Key, class Val>
void RBTree<Key, Val>::RBTransplant(Node* x, Node* y) {
    if (x -> parent == this -> nil_) {
        this -> root_ = y;
    }
    else if (x == x -> parent -> left) {
        x -> parent -> left = y;
    }
    else {
        x -> parent -> right = y;
    }
    y -> parent = x -> parent;
}

template <class Key, class Val>
void RBTree<Key, Val>::RBDeleteFixup(Node* x) {
    while (x != this -> root_ && x -> color == BLACK) {
        if (x == x -> parent -> left) {
            Node* w = x -> parent -> right;
            //case 1: 兄弟结点是红色的
            if (w -> color == RED) {
                w -> color = BLACK;
                x -> parent -> color = RED;
                LeftRotate(x -> parent);
                w = x -> parent -> right;
            }
            //case2: 兄弟结点是黑色的，且它的两个子结点都是黑色的
            if (w -> left -> color == BLACK && w -> right -> color == BLACK) {
                w -> color = RED;
                x = x -> parent;
            }
            else {
                //case 3: 兄弟结点是黑色的，且它的右子结点是黑色的（左为红）
                if (w -> right -> color == BLACK) {
                    w -> left -> color = BLACK;
                    w -> color = RED;
                    RightRotate(w);
                    w = x -> parent -> right;
                }
                //case 4: 兄弟结点是黑色的，且它的右子结点是红色的
                w -> color = x -> parent -> color;
                x -> parent -> color = BLACK;
                w -> right -> color = BLACK;
                LeftRotate(x -> parent);
                x = this -> root_;
            }
        }
        else {
            Node* w = x -> parent -> left;
            if (w -> color == RED) {
                w -> color = BLACK;
                x -> parent -> color = RED;
                RightRotate(x -> parent);
                w = x -> parent -> left;
            }
            if (w -> left -> color == BLACK && w -> right -> color == BLACK) {
                w -> color = RED;
                x = x -> parent;
            }
            else {
                if (w -> left -> color == BLACK) {
                    w -> right -> color = BLACK;
                    w -> color = RED;
                    LeftRotate(w);
                    w = x -> parent -> left;
                }
                w -> color = x -> parent -> color;
                x -> parent -> color = BLACK;
                w -> left -> color = BLACK;
                RightRotate(x -> parent);
                x = this -> root_;
            }
        }
    }
    x -> color = BLACK;
}

template <class Key, class Val>
typename RBTree<Key, Val>::Node* RBTree<Key, Val>::TreeMin(Node* x) {
    while (x -> left != this -> nil_) {
        x = x -> left;
    }
    return x;
}

template <class Key, class Val>
typename RBTree<Key, Val>::Node* RBTree<Key, Val>::TreeSearch(Node* x, Key key_) {
    if (x == this -> nil_ || key_ == x -> key) {
        return x;
    }
    if (key_ < x -> key) {
        return TreeSearch(x -> left, key_);
    }
    else {
        return TreeSearch(x -> right, key_);
    }
}

template <class Key, class Val>
void RBTree<Key, Val>::RBPrint(Node* x) {
    if (x != nil_) {
        RBPrint(x -> left);
        cout << x -> key << " - " << x -> val << endl;
        RBPrint(x -> right);
    }
}

//递归清除子树
template <class Key, class Val>
void RBTree<Key, Val>::RBClearSubtree(Node* x) {
    if (x != this -> nil_) {
        RBClearSubtree(x -> left);
        RBClearSubtree(x -> right);
        if (x != this -> root_) {
            if (x == x -> parent -> left) {
                x -> parent -> left = this -> nil_;
            }
            else {
                x -> parent -> right = this -> nil_;
            }
        }
        else {
            this -> root_ = this -> nil_;
        }
            delete x;
    }
}


template <class Key, class Val>
RBTree<Key, Val>::RBTree() {
    this -> nil_ = new Node();
    this -> root_ = this -> nil_;
}

template <class Key, class Val>
RBTree<Key, Val>::~RBTree() {
    RBClearSubtree(this -> root_);
    delete this -> nil_;
}

template <class Key, class Val>
void RBTree<Key, Val>::RBInsert(Key key_, Val val_) {
    if (TreeSearch(this -> root_, key_) != this -> nil_) {
        cout << "key " << key_ << " conflict" << endl;
    }
    else {
        Node* z = new Node(key_, val_);
        Node* y = this -> nil_;
        Node* x = this -> root_;
        while (x != this -> nil_) {
            y = x;
            if (z -> key < x -> key) {
                x = x -> left;
            }
            else {
                x = x -> right;
            }
        }
        z -> parent = y;

        if (y == this -> nil_) {
            this -> root_ = z;
            z -> parent = this -> nil_;
        }
        else if (z -> key < y -> key) {
            y -> left = z;
        }
        else {
            y -> right = z;
        }

        z -> right = this -> nil_; 
        z -> left = this -> nil_; 
        z -> color = RED;

        RBInsertFixup(z);
    }
    

}

template <class Key, class Val>
void RBTree<Key, Val>::RBUpdate(Key key_, Val val_) {
    if (TreeSearch(this -> root_, key_) != this -> nil_) {
        Node* z = TreeSearch(this -> root_, key_);
        z -> val = val_;
    }
    else {
        RBInsert(key_, val_);
    }
}

template <class Key, class Val>
void RBTree<Key, Val>::RBDelete(Key key_) {
    Node* z = TreeSearch(this -> root_, key_);
    if (z == this -> nil_) {
        cout << "key " << key_ << " missing" << endl;
        return;
    }
    Node* x;
    Node* y = z;
    Color y_ori_color = y -> color;
    if (z -> left == this -> nil_) {
        x = z -> right;
        RBTransplant(z, z -> right);
    }
    else if (z -> right == this -> nil_) {
        x = z -> left;
        RBTransplant(z, z -> left);
    }
    else {
        y = TreeMin(z -> right);
        y_ori_color = y -> color;
        x = y -> right;
        if (y -> parent == z) {
            x -> parent = y;
        }
        else {
            RBTransplant(y, y -> right);
            y -> right = z -> right;
            y -> right -> parent = y;
        }
        RBTransplant(z, y);
        y -> left = z -> left;
        y -> left -> parent = y;
        y -> color = z -> color;
    }
    if (y_ori_color == BLACK) {
        RBDeleteFixup(x);
    }
    delete z;
}

template <class Key, class Val>
void RBTree<Key, Val>::RBSearchShow(Key key_) {
    Node* z = TreeSearch(this -> root_, key_);
    if (z == this -> nil_) {
        cout << "key " << key_ << " missing" << endl;
    }
    else {
        cout << z -> val << endl;
    }
}

template <class Key, class Val>
void RBTree<Key, Val>::RBShow() {
    RBPrint(this -> root_);
}

template <class Key, class Val>
void RBTree<Key, Val>::RBClear() {
    RBClearSubtree(this -> root_);
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

void RBExec::MunuPrint() {
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

void RBExec::FileInit() {
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
    rb.RBClear();
    string tem;
    getline(ifs, tem);

    Word word;
    WordInfo word_info;
    while (ifs >> word 
            && ifs >> word_info.part_of_speech
            && ifs >> word_info.frequency) {
        rb.RBInsert(word, word_info);
    }
    
    ifs.close();
}

void RBExec::Delete() {
    cout << "#Delete by command" << endl;
    string word;
    cout << "Please input word: ";
    cin >> word;
    cin.clear();
    cin.sync(); 

    rb.RBDelete(word);
}

void RBExec::FileDelete() {
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
        rb.RBDelete(word);
    }
    
    ifs.close();
}

void RBExec::Insert() {
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

    rb.RBInsert(word, word_info);
}

void RBExec::FileInsert() {
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
        rb.RBInsert(word, word_info);
    }
    
    ifs.close();
}

void RBExec::Update() {
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

    rb.RBUpdate(word, word_info);
}

void RBExec::Search() {
    cout << "#Search" << endl;
    string word;
    cout << "Please input word: ";
    cin >> word;
    cin.clear();
    cin.sync(); 

    rb.RBSearchShow(word);
}

void RBExec::Dump() {
    cout << "#Dump" << endl;

    rb.RBShow();
}


RBExec::RBExec() {}

void RBExec::exec() {
    cout << "Welcome to the Red-Black Tree!" << endl;
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
    RBExec a;
    a.exec();
    
    return 0;
}


    
    
    
    
    
    