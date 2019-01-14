#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "graph.h"
using namespace std;
 
vector<string> convert;
vector<vector<int> > gra;
vector<pair<string,int>> ans;
string start;int total_time,ready;

void road(int,int,vector<pair<string,int>>,int,map<string,struct data>);
void maxHappy(int,map<string,struct data>);
void printout(vector<pair<string,int>> ans){
    for(int i=0;i<ans.size();i++){
        cout<<ans[i].first<<" "<<ans[i].second<<endl;
    }
}
int main(int argc,char* argv[]){
    //openfile
    string arg=argv[1];
    arg = "./"+arg+"/tp.data";
    ifstream infile(arg.c_str());
    if(!infile) return 1;

    //read total record
    map<string,struct data> m;
    int nodes,edges;
    infile>>nodes>>edges>>total_time>>ready;
    gra.resize(nodes,vector<int>(nodes,0));
    Graph g;
    start=g.readData(infile,nodes,edges,m,gra);

    //index convert to name
    map<string, struct data>:: iterator itr; 
    convert.resize(m.size());
    for (itr=m.begin(); itr != m.end(); itr++)
        convert[itr->second.index] =itr->first;

    //count shortest path
    g.dijkstra(gra,start,m);

    //find road
    maxHappy(nodes,m);
printout(ans);

    // string arg1="./"+arg+"/ans1.txt";
    // ofstream outfile(arg1.c_str());

    infile.close();
    //outfile.close();
}
void maxHappy(int nodes,map<string,struct data> m){
    int start_i=m.find(start)->second.index;
    for(int j=0;j<nodes;j++){
        map<string, struct data>:: iterator itr; 
        for (itr=m.begin(); itr != m.end(); itr++)
            itr->second.visit=false;
        
        if(gra[start_i][j]>0 && (total_time-gra[start_i][j]) > m.find(start)->second.shortest){
            vector<pair<string,int>> res;
            res.push_back(make_pair(start,ready));
            m.find(start)->second.visit=true;
            road(total_time-gra[start_i][j],j,res,m.find(start)->second.happy,m);
        }
    }
}
void road(int limit,int v,vector<pair<string,int>> res,int grade,map<string,struct data> m){
    int u = m.find(res[res.size()-1].first)->second.index;
    string Vname=convert[v],Uname=convert[u];

    //is open && not close
    int now=res[res.size()-1].second+gra[u][v];
    if(Uname==Vname) now = m.find(Vname)->second.open;
    res.push_back(make_pair(Vname,now));
    if(now>=m.find(Vname)->second.open && now<=m.find(Vname)->second.close 
                                                    && m.find(Vname)->second.visit==false){
        grade+=m.find(Vname)->second.happy;
        m.find(Vname)->second.visit=true;
    }
    //go to next node
    for(int i=0;i<m.size();i++){
        string Iname = convert[i];
        if(gra[v][i]==0 && Vname!=Iname) continue; 
        if(v==i){
            int wait = m.find(Iname)->second.open-res[res.size()-1].second;
            if((limit-wait) >= m.find(Iname)->second.shortest && wait>0){
                road(limit-wait,i,res,grade,m);
            }
        }else{
            if((limit-gra[v][i]) >= m.find(Iname)->second.shortest)
                road(limit-gra[v][i],i,res,grade,m);
        }
    }
    //update best answer
    if(res[res.size()-1].first==start ){
        res.push_back(make_pair("final_grade",grade));
        if(ans.empty()){
            ans = res;
        }else if(res[res.size()-1].second> ans[ans.size()-1].second){
            ans = res;
        }else if(res[res.size()-1].second==ans[ans.size()-1].second && res.size()<ans.size()){
            ans = res;
        }
    }//printout(ans);
}


