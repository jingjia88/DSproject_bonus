#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "graph.h"
using namespace std;
 
vector<string> convert;
vector<vector<int> > gra;
vector<pair<string,int> > ans;
vector<pair<string,int> > ans0;
string start;int total_time,ready,best;

void road(int,int,vector<pair<string,int> >,int,map<string,struct data>);
void road0(int,int,vector<pair<string,int> >,int,map<string,struct data>);
void maxHappy(int,map<string,struct data>);
void maxHappy_0(int,map<string,struct data>);
void writeFile(string,vector<pair<string,int> >);
void printout(vector<pair<string,int> > s){
    for(int i=0;i<s.size();i++){
        cout<<s[i].first<<" "<<s[i].second<<endl;
    }
}
int main(int argc,char* argv[]){
    //openfile
    string arg=argv[1];
    string arg1 = "./"+arg+"/tp.data";
    ifstream infile(arg1.c_str());
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
    for (itr=m.begin(); itr != m.end(); itr++){
        convert[itr->second.index] =itr->first;
        best+=itr->second.happy;
    }

    //count shortest path
    for(int i=0;i<nodes;i++){
        start = convert[i];
        g.dijkstra(gra,start,m);
        //find road
        maxHappy_0(nodes,m);
        if(!ans0.empty() && ans0[ans0.size()-1].second==best) break;
    }
    string arg3="./"+arg+"/ans1.txt";
    writeFile(arg3,ans0);
printout(ans0);
    for(int i=0;i<nodes;i++){
        start = convert[i];
        g.dijkstra(gra,start,m);
        //find road
        maxHappy(nodes,m);
        if(!ans.empty() && ans[ans.size()-1].second==best) break;
    }

    printout(ans);cout<<endl<<endl;
    

    //output ans2
    string arg2="./"+arg+"/ans2.txt";
    writeFile(arg2,ans);

    infile.close();
}
void writeFile(string arg,vector<pair<string,int> > s){
    ofstream outfile(arg.c_str());
    int usedTime = s[s.size()-2].second-ready;

    outfile << s[s.size()-1].second << " "<< usedTime << '\n';
    for (int i = 1; i < s.size(); i++){
        if(s[i-1].first!=s[i].first){
            outfile<<s[i-1].first<<" "<<s[i-1].second<<" "<<s[i-1].second<< '\n';
        }else{
            outfile<<s[i-1].first<<" "<<s[i-1].second<<" "<<s[i].second<< '\n';
            i++;
        }
    }
    outfile.close();
}
void maxHappy_0(int nodes,map<string,struct data> m){
    int start_i=m.find(start)->second.index;
    for(int j=0;j<nodes;j++){
        map<string, struct data>:: iterator itr; 
        for (itr=m.begin(); itr != m.end(); itr++)
            itr->second.visit=false;
        //if can go to other node,else stay at first node
        if(gra[start_i][j]>0 && (total_time-gra[start_i][j]) > m.find(start)->second.shortest){
            vector<pair<string,int> > res;
            res.push_back(make_pair(start,ready));
            m.find(start)->second.visit=true;
            road0(total_time-gra[start_i][j],j,res,m.find(start)->second.happy,m);
        }else{
            if(ans0.empty()){
                ans0.push_back(make_pair(start,ready));
                ans0.push_back(make_pair("final_grade",m.find(start)->second.happy));
            }else if(ans0[ans0.size()-1].second<m.find(start)->second.happy){
                ans0.clear();
                ans0.push_back(make_pair(start,ready));
                ans0.push_back(make_pair("final_grade",m.find(start)->second.happy));
            }
        }
        if(!ans0.empty() && ans0[ans0.size()-1].second==best) return;
    }
    return;
}
void road0(int limit,int v,vector<pair<string,int> > res,int grade,map<string,struct data> m){
    int u = m.find(res[res.size()-1].first)->second.index;
    string Vname=convert[v],Uname=convert[u];

    //is open && not close
    int now=res[res.size()-1].second+gra[u][v];
    res.push_back(make_pair(Vname,now));
    if(m.find(Vname)->second.visit==false){
        grade+=m.find(Vname)->second.happy;
    }
    m.find(Vname)->second.visit=true;
    //update best answer
    if(res[res.size()-1].first==start ){
        if(ans0.empty()){
            ans0 = res; 
            ans0.push_back(make_pair("final_grade",grade));
        }else if(grade> ans0[ans0.size()-1].second){
            ans0 = res; 
            ans0.push_back(make_pair("final_grade",grade));
        }
        if(ans0[ans0.size()-1].second==best) return;
    }
    //go to next node
    for(int i=m.size()-1;i>=0;i--){
        string Iname = convert[i];
        if(gra[v][i]==0) continue; 
        if((limit-gra[v][i]) >= m.find(Iname)->second.shortest)
            road0(limit-gra[v][i],i,res,grade,m);
        if(grade==best&&res[res.size()-1].first==start) break;
    }
    return;
}

void maxHappy(int nodes,map<string,struct data> m){
    int start_i=m.find(start)->second.index;
    for(int j=0;j<nodes;j++){
        map<string, struct data>:: iterator itr; 
        for (itr=m.begin(); itr != m.end(); itr++)
            itr->second.visit=false;
        
        if(gra[start_i][j]>0 && (total_time-gra[start_i][j]) > m.find(start)->second.shortest){
            vector<pair<string,int> > res;
            res.push_back(make_pair(start,ready));
            m.find(start)->second.visit=true;
            if(ready>=m.find(start)->second.open && ready<=m.find(start)->second.close){
                road(total_time-gra[start_i][j],j,res,m.find(start)->second.happy,m);
            }else{
                road(total_time-gra[start_i][j],j,res,0,m);
            }
        }else{
            if(ans.empty()){
                if(ready>=m.find(start)->second.open && ready<=m.find(start)->second.close){
                    ans.push_back(make_pair(start,ready));
                    ans.push_back(make_pair("final_grade",m.find(start)->second.happy));
                }
            }else if(ans[ans.size()-1].second<m.find(start)->second.happy){
                if(ready>=m.find(start)->second.open && ready<=m.find(start)->second.close){
                    ans.clear();
                    ans.push_back(make_pair(start,ready));
                    ans.push_back(make_pair("final_grade",m.find(start)->second.happy));
                }
            }
        }
        if(!ans.empty() && ans[ans.size()-1].second==best) return;
    }
    return;
}
void road(int limit,int v,vector<pair<string,int> > res,int grade,map<string,struct data> m){
    if(limit<=0) return;
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
    //update best answer
    if(res[res.size()-1].first==start ){
        if(ans.empty()){
            ans = res; 
            ans.push_back(make_pair("final_grade",grade));
        }else if(grade> ans[ans.size()-1].second){
            ans = res; 
            ans.push_back(make_pair("final_grade",grade));
        }
        if(!ans.empty() && ans[ans.size()-1].second==best) return;
    }
    //go to next node
    for(int i=m.size()-1;i>=0;i--){
        string Iname = convert[i];
        if(gra[v][i]==0 && Vname!=Iname) continue; 
        if(m.find(Iname)->second.visit==false && m.find(Iname)->second.open<res[res.size()-1].second){
            if(v==i){
                if(m.find(Iname)->second.visit==true) continue;
                int wait = m.find(Iname)->second.open-res[res.size()-1].second;
                if((limit-wait) >= m.find(Iname)->second.shortest && wait>0){
                    road(limit-wait,i,res,grade,m);
                }
            }else{
                if((limit-gra[v][i]) >= m.find(Iname)->second.shortest)
                    road(limit-gra[v][i],i,res,grade,m);
            }
        }else{
            if(v==i){
                if(m.find(Iname)->second.visit==true) continue;
                int wait = m.find(Iname)->second.open-res[res.size()-1].second;
                if((limit-wait) >= m.find(Iname)->second.shortest && wait>0){
                    road(limit-wait,i,res,grade,m);
                }
            }else{
                if((limit-gra[v][i]) >= m.find(Iname)->second.shortest)
                    road(limit-gra[v][i],i,res,grade,m);
            }
        }
    }
    return;
}


