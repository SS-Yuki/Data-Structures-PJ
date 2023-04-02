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

//�ߵ���Ϣ
struct Edge {
    int end;
    int line;
    int length;
};
//վ��ԭʼ���ݴ���
struct Data {
    string name;
    string time;
};
//���ȶ��д���Ԫ��
struct Node {
	int vertex;
	int dis;
	bool operator>(const Node& a) const {return dis > a.dis;}
};

int v_num;                                              //վ������
map<string, int> sta_store;                             //վ������->���ֱ��
string sta_get[STA_MAX];                                //���ֱ��->վ������
vector<Edge> adj[STA_MAX];                              //�ڽӱ�
int road_length[STA_MAX][STA_MAX];                      //����վ�����������ʱ��

int dis[STA_MAX];                                       //ĳվ�㵽Դ����̹���ʱ��
int vis[STA_MAX];                                       //�������
priority_queue<Node, vector<Node>, greater<Node>> q;    //���ȶ���
vector<Edge> prevs[STA_MAX];                            //ǰ��
vector<vector<int>> roads;                              //���в�ͬ���·�Ĵ���
int time_sum;                                           //��ʱ��
vector<int> route;                                      //ĳһ�����·�ļ�¼

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
    cout << "��ӭ!" << endl;
    print_direct();
    int direct;
    while (true) {
        direct = 0;
        cout << "������ָ��: ";
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
            default: cout << "����������������롣" << endl; break;
        }
    }
}

void print_direct() {
    cout << "-------------------------------------------------------------------------" << endl; 
    cout << "1:�鿴����ָ��" << endl;
    cout << "2:��ʼ����ͼ" << endl;
    cout << "3:չʾ����վ��" << endl;
    cout << "4:�����ض�վ�㣬��ʾ�����Ϣ" << endl;
    cout << "5:������㡢�յ㣬����������·" << endl;
    cout << "6:������㡢�յ㣬���һ�����˴������ٵ����·" << endl;
    cout << "7:������㡢�յ��Լ����ɸ��м�վ���ֱ����������վ�������·" << endl;
    cout << "8:������㡢�յ��Լ����ɸ��м�վ�����һ�����˴����ٵ����·" << endl;
    cout << "9:�ļ�����" << endl;
    cout << "-1:�˳�" << endl; 
    cout << "-------------------------------------------------------------------------" << endl; 
}

void init_map() {
  	ifstream ifs;
	ifs.open(FILE_NAME, ios::in);
	if (!ifs.is_open()) {
		cout << "��ʼ��ʧ�ܡ�" << endl;
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
    cout << "��ʼ���ɹ���" << endl;
}

void show_map() {
    cout << "����վ������:" << endl;
    _rep(i, 1, v_num) {
        cout << i << ". " << sta_get[i] << endl;
        for (auto e : adj[i]) {
            cout << sta_get[e.end] << " Line" << e.line << " ����ʱ��Ϊ" << e.length << "����" << endl;
        }
        cout << endl;
    }
}

void search_map() {
    cout << "������վ������:";
    string tem;
    cin >> tem;
    cin.clear(), cin.sync(); 
    if (sta_store.find(tem) != sta_store.end()) {
        cout << sta_store[tem] << ". " << tem << endl;
        for (auto e : adj[sta_store[tem]]) {
            cout << sta_get[e.end] << " Line" << e.line << " ����ʱ��Ϊ" << e.length << "����" << endl;
        }
        cout << endl;
    }
    else 
        cout << "�޴�վ�㡣" << endl;
}

void print_all() {
    cout << "������վ������:" << "(��ʽ: վ1 վ2 ... ����)" << endl;
    vector<int> stas;
    while (true) {
        string sta_;
        cin >> sta_;
        if (sta_ == "����") break;
        if (sta_store.find(sta_) != sta_store.end()) {
            stas.push_back(sta_store[sta_]);
        }
    }
    if (stas.size() == 0) {
        cout << "��������" << endl;
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
    cout << "������վ������:" << "(��ʽ: վ1 վ2 ... ����)" << endl;
    vector<int> stas;
    time_sum = 0;
    vector<int> _together;
    while (true) {
        string sta_;
        cin >> sta_;
        if (sta_ == "����") break;
        if (sta_store.find(sta_) != sta_store.end()) {
            stas.push_back(sta_store[sta_]);
        }
    }
    if (stas.size() == 0) {
        cout << "��������" << endl;
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
    cout << "�����ļ�Ϊ:";
    cin >> in;
    cin.clear(), cin.sync();
    cout << "����ļ�Ϊ:";
    cin >> out;
    cin.clear(), cin.sync();
    ifstream ifs;
	ifs.open(in, ios::in);
	if (!ifs.is_open()) {
		cout << "�ļ���ʧ�ܡ�" << endl;
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
        cout << "Ԥ������ʱ��: " << "0 ����" << endl;
        cout << "Ԥ�ƻ�����: " << "0 ��" << endl;
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
    cout << "Ԥ������ʱ��: " <<  road_length[a[0]][a[a.size() - 1]] << " ����" << endl;
    cout << "Ԥ�ƻ�����: " << change_sum << " ��" << endl;
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
    out << "Ԥ������ʱ��: " << time_sum << " ����" << endl;
    out << "Ԥ�ƻ�����: " << change_sum << " ��" << endl;
}