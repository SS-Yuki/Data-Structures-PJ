#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <fstream>
#include <queue>
using namespace std;

#define _rep(i, a, b) for (int i = (a); i <= (b); ++i)
#define _dep(i, a, b) for (int i = (a); i >= (b); --i)
#define STA_MAX 350
#define STA_ONE_LINE 50
#define FILE_NAME "data.txt"

//边的信息
struct Edge {
    int end;
    int line;
    int length;
};
//站点原始数据储存
struct Data {
    string name;
    string time;
};
//优先队列储存元素
struct Node {
	int vertex;
	int dis;
	bool operator>(const Node& a) const {return dis > a.dis;}
};

int v_num;                                              //站点总数
map<string, int> sta_store;                             //站点名称->数字编号
string sta_get[STA_MAX];                                //数字编号->站点名称
vector<Edge> adj[STA_MAX];                              //邻接表
int road_length[STA_MAX][STA_MAX];                      //两个站点间所需的最短时间

int dis[STA_MAX];                                       //某站点到源的最短估计时间
int vis[STA_MAX];                                       //标记数组
priority_queue<Node, vector<Node>, greater<Node>> q;    //优先队列
vector<Edge> prevs[STA_MAX];                            //前驱
vector<vector<int>> roads;                              //所有不同最短路的储存
int time_sum;                                           //总时间
vector<int> route;                                      //某一条最短路的记录

void system();
void print_direct();
void init_map();
void show_map();
void search_map();
void print_all();
void print_one();
void file_process();

void dijkstra(int s);
void store_path(int s, int v);
void print_path(const vector<int> &a);
int change_cal(const vector<int> &a);
void print_together_(const vector<int> &a, ostream &out);

int main() {
    system();

    return 0;
}

void system() {
    cout << "欢迎!" << endl;
    print_direct();
    int direct;
    while (true) {
        direct = 0;
        cout << "请输入指令: ";
        cin >> direct;
        cin.clear(), cin.sync(); 
        switch (direct)
        {
            case 1: print_direct(); break;
            case 2: init_map(); break;
            case 3: show_map(); break;
            case 4: search_map(); break;
            case 5: print_all(); break;
            case 6: print_one(); break;
            case 7: print_all(); break;
            case 8: print_one(); break;
            case 9: file_process(); break;
            case -1: exit(0); break;
            default: cout << "输入错误，请重新输入。" << endl; break;
        }
    }
}

void print_direct() {
    cout << "-------------------------------------------------------------------------" << endl; 
    cout << "1:查看可用指令" << endl;
    cout << "2:初始化地图" << endl;
    cout << "3:展示所有站点" << endl;
    cout << "4:查找特定站点，显示相关信息" << endl;
    cout << "5:输入起点、终点，输出所有最短路" << endl;
    cout << "6:输入起点、终点，输出一条换乘次数最少的最短路" << endl;
    cout << "7:输入起点、终点以及若干个中间站，分别输出相邻两站所有最短路" << endl;
    cout << "8:输入起点、终点以及若干个中间站，输出一条换乘次数少的最短路" << endl;
    cout << "9:文件操作" << endl;
    cout << "-1:退出" << endl; 
    cout << "-------------------------------------------------------------------------" << endl; 
}

void init_map() {
  	ifstream ifs;
	ifs.open(FILE_NAME, ios::in);
	if (!ifs.is_open()) {
		cout << "初始化失败。" << endl;
		return;
	}
    int line_max, group_num;
    ifs >> line_max >> group_num;
    int line_order;
    int sta_num;
    Data sta[STA_ONE_LINE];
    _rep(i, 1, group_num) {
        ifs >> line_order >> sta_num;
        _rep(j, 1, sta_num) {
            ifs >> sta[j].name >> sta[j].time;
            if (sta_store.find(sta[j].name) == sta_store.end()) {
                sta_store.insert(pair<string, int>(sta[j].name, ++v_num));
                sta_get[v_num] = sta[j].name;
            }
            if (j > 1) {
                int w = (sta[j].time[1] > sta[j - 1].time[1]) 
                    ? (sta[j].time[3] - '0') * 10 + (sta[j].time[4] - '0') + 60 - (sta[j - 1].time[3] - '0') * 10 - (sta[j - 1].time[4] - '0')
                    : (sta[j].time[3] - '0') * 10 + (sta[j].time[4] - '0') - (sta[j - 1].time[3] - '0') * 10 - (sta[j - 1].time[4] - '0');
                int u = sta_store[sta[j].name];
                int v = sta_store[sta[j - 1].name];
                adj[u].push_back({v, line_order, w});
                adj[v].push_back({u, line_order, w});
            }
        }
    }
    ifs.close();
    cout << "初始化成功。" << endl;
}

void show_map() {
    cout << "所有站点如下:" << endl;
    _rep(i, 1, v_num) {
        cout << i << ". " << sta_get[i] << endl;
        for (auto e : adj[i]) {
            cout << sta_get[e.end] << " Line" << e.line << " 所需时间为" << e.length << "分钟" << endl;
        }
        cout << endl;
    }
}

void search_map() {
    cout << "请输入站点名称:";
    string tem;
    cin >> tem;
    cin.clear(), cin.sync(); 
    if (sta_store.find(tem) != sta_store.end()) {
        cout << sta_store[tem] << ". " << tem << endl;
        for (auto e : adj[sta_store[tem]]) {
            cout << sta_get[e.end] << " Line" << e.line << " 所需时间为" << e.length << "分钟" << endl;
        }
        cout << endl;
    }
    else 
        cout << "无此站点。" << endl;
}

void print_all() {
    cout << "请输入站点名称:" << "(格式: 站1 站2 ... 结束)" << endl;
    vector<int> stas;
    while (true) {
        string sta_;
        cin >> sta_;
        if (sta_ == "结束") break;
        if (sta_store.find(sta_) != sta_store.end()) {
            stas.push_back(sta_store[sta_]);
        }
    }
    if (stas.size() == 0) {
        cout << "输入有误。" << endl;
        return;
    }
    _rep(i, 0, stas.size() - 2) {
        for (auto e : roads) e.clear();
        roads.clear();
        cout << sta_get[stas[i]] << " -> " << sta_get[stas[i + 1]] << endl;
        dijkstra(stas[i]);
        store_path(stas[i], stas[i + 1]);
        for (auto a : roads) 
            print_path(a);
        cout << endl;
    }
    cin.clear(), cin.sync(); 
}

void print_one() {
    cout << "请输入站点名称:" << "(格式: 站1 站2 ... 结束)" << endl;
    vector<int> stas;
    time_sum = 0;
    vector<int> _together;
    while (true) {
        string sta_;
        cin >> sta_;
        if (sta_ == "结束") break;
        if (sta_store.find(sta_) != sta_store.end()) {
            stas.push_back(sta_store[sta_]);
        }
    }
    if (stas.size() == 0) {
        cout << "输入有误。" << endl;
        return;
    }
    cout << sta_get[stas.front()] << " -> " << sta_get[stas.back()] << endl;
    _together.push_back(stas.front());
    _rep(i, 0, stas.size() - 2) {
        for (auto e : roads) e.clear();
        roads.clear();
        dijkstra(stas[i]);
        store_path(stas[i], stas[i + 1]);
        vector<int> obj = roads.front();
        for (auto a : roads) 
            if (change_cal(a) < change_cal(obj)) {
                obj = a;
            }
        if (obj.size() == 1) _together.push_back(prevs[obj.front()].front().line), _together.push_back(obj.front());
        else {
            int _change = obj[1];
            _together.push_back(_change);
            for (int i = 1; i < obj.size(); i += 2) {
                if (obj[i] != _change) {
                    _change = obj[i];
                    _together.push_back(obj[i - 1]), _together.push_back(_change);
                }
            }
            _together.push_back(obj.back());
        }
        time_sum += road_length[stas[i]][stas[i + 1]];
    }
    print_together_(_together, cout);
    cin.clear(), cin.sync(); 
}

void file_process() {
    string in, out;
    vector<string> ins;
    cout << "输入文件为:";
    cin >> in;
    cin.clear(), cin.sync();
    cout << "输出文件为:";
    cin >> out;
    cin.clear(), cin.sync();
    ifstream ifs;
	ifs.open(in, ios::in);
	if (!ifs.is_open()) {
		cout << "文件打开失败。" << endl;
		return;
	}
    string buf;
	while (getline(ifs, buf)) ins.push_back(buf);
    ifs.close();
    ofstream ofs;
	ofs.open(out, ios::out);
    for (auto str : ins) {
        string::size_type pos1, pos2;
        pos1 = 0;
        pos2 = str.find("\t");
        vector<int> stas;
        while(pos2 != string::npos) {
            if (sta_store.find(str.substr(pos1, pos2 - pos1)) != sta_store.end())
                stas.push_back(sta_store[str.substr(pos1, pos2 - pos1)]);
            pos1 = pos2 + 1;
            pos2 = str.find("\t", pos1);
        }
        if (pos1 != str.length() && (sta_store.find(str.substr(pos1)) != sta_store.end())) 
            stas.push_back(sta_store[str.substr(pos1)]);
        if (stas.size() == 0) continue;
        vector<int> _together;
        time_sum = 0;
        _together.push_back(stas.front());
        _rep(i, 0, stas.size() - 2) {
            for (auto e : roads) e.clear();
            roads.clear();
            dijkstra(stas[i]);
            store_path(stas[i], stas[i + 1]);
            vector<int> obj = roads.front();
            for (auto a : roads) 
                if (change_cal(a) < change_cal(obj)) {
                    obj = a;
                }
            if (obj.size() == 1) _together.push_back(prevs[obj.front()].front().line), _together.push_back(obj.front());
            else {
                int _change = obj[1];
                _together.push_back(_change);
                for (int i = 1; i < obj.size(); i += 2) {
                    if (obj[i] != _change) {
                        _change = obj[i];
                        _together.push_back(obj[i - 1]), _together.push_back(_change);
                    }
                }
                _together.push_back(obj.back());
            }
            time_sum += road_length[stas[i]][stas[i + 1]];
        }
        print_together_(_together, ofs);
    }
    ofs.close();
}

void dijkstra(int s) {
    memset(dis, 63, sizeof(dis));
    memset(vis, 0, sizeof(vis));
    _rep(i, 0, STA_MAX - 1) prevs[i].clear();
    dis[s] = 0;
    prevs[s].push_back({s, adj[s].front().line, 0});
    q.push({s, 0});
    while (!q.empty()) {
		int u = q.top().vertex;
		q.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (auto e : adj[u]) {
			int v = e.end;
			int w = e.length;
			if (dis[v] >= dis[u] + w) {
                if (dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    road_length[s][v] = dis[v];
                    q.push({v, dis[v]});
                    prevs[v].clear();
                }
                prevs[v].push_back({u, e.line, e.length});
			}
		}
    }  
}

void store_path(int s, int v) {
	if (s == v) {
        route.push_back(s);
        roads.push_back(vector<int>(route.rbegin(), route.rend()));
        route.pop_back();
        return;
	}
    _rep(i, 0, prevs[v].size() - 1) {
        route.push_back(v);
        route.push_back(prevs[v][i].line);
        store_path(s, prevs[v][i].end);
        route.pop_back(); route.pop_back(); 
    }
}

void print_path(const vector<int> &a) {
    if (a.size() == 1) {
        cout << sta_get[a.front()] <<  "-Line " << prevs[a.front()].front().line << "-" << sta_get[a.front()] << endl;
        cout << "预计所需时间: " << "0 分钟" << endl;
        cout << "预计换乘数: " << "0 次" << endl;
        return;
    }
    int _change = a[1];
    int change_sum = 0;
    cout << sta_get[a.front()] <<  "-Line " << _change;
    for (int i = 1; i < a.size(); i += 2) {
        if (a[i] != _change) {
            _change = a[i];
            change_sum++;

            cout << "-" << sta_get[a[i - 1]] << "-Line " << _change;  
        }
    }
    cout << "-" << sta_get[a.back()] << endl;
    cout << "预计所需时间: " <<  road_length[a[0]][a[a.size() - 1]] << " 分钟" << endl;
    cout << "预计换乘数: " << change_sum << " 次" << endl;
}

int change_cal(const vector<int> &a) {
    if (a.size() == 1) {
        return 0;
    }
    int _change = a[1];
    int change_sum = 0;
    for (int i = 1; i < a.size(); i += 2) {
        if (a[i] != _change) {
            _change = a[i];
            change_sum++;
        }
    }   
    return change_sum;
}

void print_together_(const vector<int> &a, ostream &out) {
    int _change = a[1];
    int change_sum = 0;
    out << sta_get[a.front()];
    for (int i = 1; i < a.size(); i += 2) {
        if (a[i] != _change) {
            _change = a[i];
            change_sum++;
        }
        out <<  "-Line " << a[i] << "-" << sta_get[a[i + 1]];  
    }
    out << endl;
    out << "预计所需时间: " << time_sum << " 分钟" << endl;
    out << "预计换乘数: " << change_sum << " 次" << endl;
}