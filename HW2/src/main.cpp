#include <cstring>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include "../lib/tm_usage.h"
#include <stack>
using namespace std;

class Chord{
    public:
        int start ; int end;
        Chord(int a, int b){ this->start = a ; this->end = b;};
        Chord(){};
};

bool chord_start_sort(Chord a, Chord b){return a.start<b.start;}


short TP(vector<Chord>& data,vector<vector<short> >&M,vector<vector<bool> >&S,int i ,int j){
    if (M[i][j]>=0){
        return M[i][j];
    }
    if (i == j){
        M[i][j] = 0;
        S[i][j] = 1;
        return 0;
    }
    if (M[i][j]<0){
        //case 1
        if ( data[j].end<i || data[j].end>j){
            M[i][j] = TP(data,M,S,i,j-1);
            S[i][j] = 0;
            return M[i][j];
        }
        //case 2
        else if(data[j].end == i){
            M[i][j] = TP(data,M,S,i+1,j-1)+1;
            S[i][j] = 1;
            return M[i][j];
        }
        else
        {
            int k = data[j].end;
            if (( TP(data,M,S,i,k-1)+TP(data,M,S,k+1,j)+1)>= TP(data,M,S,i,j-1)){
                M[i][j] = TP(data,M,S,i,k-1)+TP(data,M,S,k+1,j)+1;
                S[i][j] = 1;
                return M[i][j];
                }
            else{
                M[i][j] = TP(data,M,S,i,j-1);
                S[i][j] = 0;
                return M[i][j];
                }
        }
    }
};

void find_pair(vector<Chord>& data,vector<Chord>&pair, vector<vector<bool> >&S){
    stack<Chord> select;
    int i = 0 ; int j = S.size()-1;
    select.push(Chord(i,j));
    while (!select.empty()){
        int i  = select.top().start; int j = select.top().end;
        if (i>=j){select.pop();continue;}

        if(S[i][j]==1){
            select.pop();
            int k = data[j].end;
            pair.push_back(Chord(k,j));
            if (k!=i){
                select.push(Chord(k+1,j));
                select.push(Chord(i,k-1));}
            else{
                select.push(Chord(i+1,j-1));}
            }
        else{
            select.pop();
            select.push(Chord(i,j-1));}
        };
}

int main(int argc, char* argv[])
{
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;

    //////////// read the input file /////////////
    
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2],ios::out);
    int size; fin>>size;
    int i,j;
    vector<Chord> data(size,Chord());
    while (fin >> i >> j){
        data[i] = Chord(i,j);
        data[j] = Chord(j,i);
    }
    sort(data.begin(),data.end(),chord_start_sort);
    
    vector<vector<short> > M(size,vector<short>(size,-1));
    vector<vector<bool> > S(size,vector<bool>(size,0));
    TP(data,M,S,0,size-1);
    
    short answer = M[0][size-1];
    vector<vector<short> >().swap(M);

    vector<Chord> result_pair;
    find_pair(data,result_pair,S);
    sort(result_pair.begin(),result_pair.end(),chord_start_sort);
    
    fout << answer<<endl;
    for (int i =0; i<result_pair.size(); i++){
        fout << result_pair[i].start<< " "<< result_pair[i].end<<endl;
    }
    
    //////// time ////////////////
    tmusg.getPeriodUsage(stat);
    cout <<"The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms" << endl;
    cout <<"memory: " << stat.vmPeak << "KB" << endl; // print peak memory

    //////////// write the output file ///////////

    fin.close();
    fout.close();
    return 0;
}