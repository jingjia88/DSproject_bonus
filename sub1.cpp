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

void min(string);
string read(ifstream&,int);
int main(int argc,char* argv[]){
    string argf = argv[1];
    string arg = "./"+argf+"/tp.data";
    ifstream infile(arg.c_str());

    if(!infile){
        cout<<"Something wrong"<<endl;
        return 1;
    }
    int nodes,edges,total;
    infile>>nodes>>edges>>total;

    //count shortest path 
    string start = read(infile,nodes);
    min(start);

    // unordered_map<string,struct data>::iterator iter;
    // for(iter = place.begin();iter!=place.end();iter++){
    //     cout<<iter->first<<" "<<iter->second.shortest;
    // }

    //count value
}

void Maxhappy(int total,string start){
    vector<string> ans;int maxhappy;
    unordered_map<string,struct data>::iterator iter;
    for(iter = place.begin();iter!=place.end();iter++){
        iter->second.visit = false;
    }
    ans.push_back(start); place.find(start)->second.visit=true;
    struct neighbor* here = map.find(start)->second;
    while(here!=NULL){
        next(total,here,ans,0);
        here = here->next;
    }
}
void next(int total,struct neighbor* here,vector<string> ans,int happy){
    place.find(here->node)->second.visit = true;
    total -=here->dist; ans.push_back(here->node);
    struct neighbor* go = map.find(here->node)->second;
    while(go!=NULL){
        if(total-go->dist>place.find(go->node)->second.shortest){
            if(place.find(go->node)->second.visit == false){
                happy+=place.find(go->node)->second.happy;
            }
            next(total,go,ans,happy);
        } 
        go = go->next;
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

