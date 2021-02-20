#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "../lib/tm_usage.h"
using namespace std;

class Vertex{
    public:
    int num;
    char color;
    int parent=-1;
    int key;
    int d ;
    vector<int> adj;
    vector<int> pointed;
    Vertex(){this->color = 'w'; this->parent = -1;this->adj = vector<int>();};
    bool operator==(const Vertex& b) {
        return (this->num==b.num);}
    bool operator!=(const Vertex& b) {
        return (this->num!=b.num);}
};

class Edge:public Vertex{
    public:
    int weight;
    Vertex start; Vertex end;
    Edge(){};
    Edge(Vertex s , Vertex e,int weight ){this->start =s , this->end = e , this->weight=weight;};
    bool operator==(const Edge& b) {
        return (this->start==b.start) && (this->end==b.end);};
};

void DFS_visit_d(Vertex& u, vector<Vertex>& G_V,vector<vector<int> > G_E,vector<Edge>& backedge,int& time){
    time=time+1;
    u.d = time;
    u.color = 'g';
    for (int i = 0 ; i<u.adj.size(); i++){
        Vertex &v = G_V[u.adj[i]];
        if ( v.color=='w'){
            v.parent = u.num;
            DFS_visit_d(v,G_V,G_E,backedge,time);
        }
        else if ( v.color=='g' && u.parent!=v.num){
            //backedge end
            backedge.push_back(Edge(u,v,G_E[u.num][v.num]));
        }
    }
    u.color = 'b';
    time=time+1;
}
bool compare_weight(Edge&a, Edge&b){
    return a.weight<b.weight;
}

int extract_max_posi(vector<Vertex>&Q){
    int max;
    int location=0;
    for (int i = 0 ; i<Q.size();i++){
        if (Q[i].color=='w'){
            max = Q[i].key;
            break;
        }
    }
    for (int i = 0 ; i<Q.size();i++){
        if (max<=Q[i].key && Q[i].color=='w'){
            max = Q[i].key;
            location = i;
        }
    }
    Q[location].color='b';
    return location;
}

void MST_PRIM(vector<Vertex>&G_V,vector<vector<int> >&G_E,vector<vector<int> >&G_Eu,Vertex&r,vector<Edge>&backedge){
    for (int i = 0 ; i<G_V.size();i++){
        Vertex &u = G_V[i];
        u.key = -101;
        u.color = 'w';
    }
    r.key = 0;
    int count = G_V.size();
    while (count!=0){
        Vertex& u = G_V[extract_max_posi(G_V)]; count--;
        for (int i =0 ; i <u.adj.size();i++){
            Vertex&v = G_V[u.adj[i]];
            if (G_Eu[u.num][v.num]>v.key){
                if(v.color=='w'){
                    v.key = G_Eu[u.num][v.num];
                    v.parent = u.num;
                }
            }
            if ( v.color=='b' && u.parent!=v.num){
                G_E[u.num][v.num]==G_Eu[u.num][v.num]?
                    backedge.push_back(Edge(u,v,G_Eu[u.num][v.num])):
                    backedge.push_back(Edge(v,u,G_Eu[u.num][v.num]));
            }
        }
    }
}

bool check_connected(vector<Vertex>&G_V, vector<Edge>& G_Eset){
    for (int i =0  ; i<G_V.size(); i++){
        bool exist = false;
        for (int j = 0 ; j<G_Eset.size();j++){
            if (G_Eset[j].start.num==i || G_Eset[j].end.num==i){
                exist = true;
                break;
            }
        }
        if (exist == false){return false;}
    }
    return true;
}


int main(int argc, char* argv[]){
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;
    
    fstream fin(argv[1]);
    char testcase; fin>>testcase;
    int V,E; fin>>V>>E;
    cout << "the case is "<<testcase<<endl; cout << "vextex: "<<V<<" edges: "<<E<<endl;

    vector<Vertex> G_V(V,Vertex());
    vector<Edge> G_Eset;
    for (int i = 0; i<V;i++){G_V[i].num=i;G_V[i].d=0;}
    vector<vector<int> > G_E(V,vector<int>(V,0));
    vector<vector<int> > G_Eu(V,vector<int>(V,0));
//undirected graph
    if (testcase=='u'){
        fstream fout;
        fout.open(argv[2],ios::out);
        int i, j ,k;
        while (fin >> i >> j >> k){
            G_V[i].adj.push_back(G_V[j].num);
            G_V[j].adj.push_back(G_V[i].num);
            G_E[i][j] = k;
            G_Eu[i][j] = k;
            G_Eu[j][i] = k;
        }

        vector<Edge> cut;
        //使用Prim algo
        MST_PRIM(G_V,G_E,G_Eu,G_V[0],cut);
        int ans_u = 0 ;
        for (int i=0;i<cut.size();i++){Edge&x=cut[i];ans_u = ans_u+x.weight;}
        fout << ans_u<<endl;
        cout << "Total weight is "<< ans_u<<endl;
        for (int i=0;i<cut.size();i++){
            Edge&x=cut[i];
            fout << x.start.num<<" "<<x.end.num<<" "<<x.weight<<endl;
        }
    }

//directed graph
    else if (testcase=='d'){
        fstream fout; fstream gout;fstream hout;
        fout.open(argv[2],ios::out);

        int i, j ,k;
        while (fin >> i >> j>>k){
            G_V[i].adj.push_back(G_V[j].num);
            G_E[i][j] = k;
            G_Eset.push_back(Edge(G_V[i],G_V[j],k));
        }

        //確認有無backedge
        vector<Edge> backedge;
        int time = 0;
        for (int i = 0 ; i<G_V.size();i++){
            Vertex &u = G_V[i];
            if ( u.color=='w'){
                DFS_visit_d(u,G_V,G_E,backedge,time);
            }
        }
        if (backedge.size()==0){
          fout<<0<<endl;
          return 0;
        }

        // //delete edge
        vector<Edge> delete_edge;
        delete_edge = backedge;
        for (int x=0 ; x<delete_edge.size();x++){
            for (vector<Edge>::iterator it = G_Eset.begin(); it!=G_Eset.end();it++){
                if (*it == delete_edge[x]){
                    G_Eset.erase(it);
                    break;
                }
            }
        }
        //刪除多餘的負edge，同時要檢查使否還維持connected
        sort(G_Eset.begin(),G_Eset.end(),compare_weight);
    
        vector<Edge> G_Eset2 = G_Eset;
        for (int i = 0 ; i<G_Eset.size();i++){
            if(G_Eset[i].weight<0){
                for (vector<Edge>::iterator it = G_Eset2.begin(); it!=G_Eset2.end();it++){
                    if (*it == G_Eset2[i]){
                        vector<Edge> temp=G_Eset2;
                        G_Eset2.erase(it);
                        if (check_connected(G_V,G_Eset2)){
                            delete_edge.push_back(G_Eset[i]);
                        }
                        else{
                            G_Eset2 = temp;
                        }
                        break;
                    }
                }
            }
        }
        //output file
        int sum = 0;
        for(int x=0; x<delete_edge.size();x++){sum=sum+delete_edge[x].weight;}
        fout <<sum<<endl;
        cout << "Total weight is "<< sum<<endl;
        for (int x = 0 ; x<delete_edge.size();x++){
            fout << delete_edge[x].start.num<<" "<<delete_edge[x].end.num<<" "<<delete_edge[x].weight<<endl;

        }
  }
    //////// time ////////////////
    tmusg.getPeriodUsage(stat);
    cout <<"The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms" << endl;
    cout <<"memory: " << stat.vmPeak << "KB" << endl; // print peak memory

    //////// time ////////////////
    return 0;      
}