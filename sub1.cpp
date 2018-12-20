#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
using namespace std;

#define edge pair<string,string>
struct neighbor{
    string node;
    int dist;
    struct neighbor* next;
};
struct data{
    int happy;
    int shortest;
    bool visit;
};
unordered_map<string,struct neighbor*> map;
unordered_map<string,struct data> place;
string start;vector<string> ans; int limit;

void min(string);
string read(ifstream&,int);
void next(int,struct neighbor*,vector<string>,int,int);
void Maxhappy(int,int);

int main(int argc,char* argv[]){
    string argf = argv[1];
    string arg = "./"+argf+"/tp.data";
    ifstream infile(arg.c_str());

    if(!infile){
        cout<<"Something wrong"<<endl;
        return 1;
    }
    int nodes,edges,total,ready;
    infile>>nodes>>edges>>total>>ready;
    limit = total+ready;
    //count shortest path 
    start = read(infile,nodes);
    min(start);

    //count value
    Maxhappy(total,ready);

    string arg1 = "./"+argf+"/ans1.txt";
    ofstream outfile(arg1.c_str());
    
    int cost = atoi(ans[ans.size()-2].c_str())-atoi(ans[1].c_str());
    outfile << ans[ans.size()-1] <<" "<< cost << '\n';
    for (int i = 0; i < ans.size()-1; i+=2)
        outfile<<ans[i]<<" "<<ans[i+1]<<" "<<ans[i+1]<< '\n';

    infile.close();
    outfile.close();
}

void Maxhappy(int total,int ready){
    int maxhappy = 0;
    struct neighbor* here = map.find(start)->second;
    while(here!=NULL){
        unordered_map<string,struct data>::iterator iter;
        for(iter = place.begin();iter!=place.end();iter++){
            iter->second.visit = false;
        }
        if((total-(here->dist)) >= place.find(here->node)->second.shortest){
            place.find(start)->second.visit=true;
            vector<string> res;
            int max=place.find(start)->second.happy;
            res.push_back(start); 
            res.push_back(to_string(ready));
            next(total,here,res,max,ready);
        }
        here = here->next;
    } 
}
void next(int total,struct neighbor* here,vector<string> res,int max,int now){
    if(place.find(here->node)->second.visit == false){
        max+=place.find(here->node)->second.happy; 
    }
    place.find(here->node)->second.visit = true;
    now = now + here->dist;
    res.push_back(here->node); res.push_back(to_string(now));
    struct neighbor* go = map.find(here->node)->second;
    while(go!=NULL){
        if(total-(go->dist)>=place.find(go->node)->second.shortest){
            total = total-(go->dist);
            next(total,go,res,max,now);
        }
        go = go->next;
    }
    if(res[res.size()-2]==start){
        res.push_back(to_string(max));
        if(ans.empty()){
            ans = res;
        }else if(atoi(res[res.size()-1].c_str())> atoi(ans[ans.size()-1].c_str())){
            ans = res;
        }
    }
}


//shortest path
void min(string n){
    queue<string> q;
    q.push(n);
    while(!q.empty()){
        string name = q.front(); q.pop();  
        struct neighbor* iter = map.find(name)->second;
        while(iter!=NULL){
            int f_short = place.find(name)->second.shortest;
            int& t_short = place.find(iter->node)->second.shortest;
            if(t_short==-1){
                t_short = iter->dist+ f_short;
                q.push(iter->node);
            }else if(t_short > iter->dist+ f_short){
                t_short = iter->dist+ f_short;
            }
            iter = iter->next;
        }
    }
}

//read data
string read(ifstream& infile,int nodes){
    string s,start;
    getline(infile, s);
    for(int i = 0; i < nodes; ++i){
       getline(infile, s);
       istringstream input(s);
       struct data data; string node;
       input>>node>>data.happy;
       data.shortest = -1;
       if(i==0){
           data.shortest = 0;
           start = node;
       } 
       place.insert(pair<string,struct data>(node, data));
    }

    while(getline(infile, s)){
        istringstream input(s);
        string place1,place2;int dis;
        if(input>>place1>>place2>>dis){
            struct neighbor* list=new struct neighbor;
            list->node = place2;
            list->dist = dis;
            list->next = NULL;
            struct neighbor* list1=new struct neighbor;
            list1->node = place1;
            list1->dist = dis;
            list1->next = NULL;
            if(map.find(place1)!=map.end()){
                struct neighbor* nextone = map.find(place1)->second;
                while(nextone->next!=NULL) {nextone = nextone->next;}
                nextone->next= list;
            }else{
                map.insert(pair<string, struct neighbor*>(place1, list));
            } 
            if(map.find(place2)!=map.end()){
                struct neighbor* nextone = map.find(place2)->second;
                while(nextone->next!=NULL) nextone = nextone->next;
                nextone->next= list1;
            }else{
                map.insert(pair<string, struct neighbor*>(place2, list1));
            } 
        }
    }
    return start;
}

