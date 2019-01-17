#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
using namespace std;
struct data{
    int happy,index;
    int open,close,shortest;
    bool visit;
};
class Graph{
    public:
    string readData(ifstream &infile,int nodes,int edges,map<string,struct data> &m,vector<vector<int> > &graph){
        string s; getline(infile,s);string star;
        //read nodes
        for(int i=0;i<nodes;i++){
            getline(infile,s);
            istringstream input(s);
            string name;struct data now;
            input>>name>>now.happy>>now.open>>now.close;
            if(i==0) star=name;
            now.index=i;now.shortest=1000000;
            m[name]=now;
        }
        //read edges
        for(int i=0;i<edges;i++){
            getline(infile, s);
            istringstream input(s);
            string place1,place2;int dis;
            input>>place1>>place2>>dis;
            graph[m.find(place1)->second.index][m.find(place2)->second.index]=dis;
            graph[m.find(place2)->second.index][m.find(place1)->second.index]=dis;
        }
        //print(nodes,graph);
        return star;
    }
    int minDistance(map<string,struct data> &m,vector<bool> &sptSet) 
    { 
        // Initialize min value 
        int min = 1000000, min_index; 
        
        map<string, struct data>:: iterator itr; 
        for (itr=m.begin(); itr != m.end(); itr++){
            int v=itr->second.index;
            if (sptSet[v] == false && itr->second.shortest <= min) 
                min = itr->second.shortest, min_index = v; 
        } 

        return min_index; 
    } 
    
    void dijkstra(vector<vector<int> > graph,string src,map<string,struct data> &m) 
    {
        vector<bool> sptSet(graph.size(),false);
        // Distance of source vertex from itself is always 0 
        m.find(src)->second.shortest=0;
    
        // Find shortest path for all vertices 
        for (int count = 0; count < graph.size(); count++) 
        {
            int u = minDistance(m, sptSet); 
        
            // Mark the picked vertex as processed 
            sptSet[u] = true; 
        
            // Update dist value of the adjacent vertices of the picked vertex. 
            map<string, struct data>:: iterator itr; 
            int uname;
            for (itr=m.begin(); itr != m.end(); itr++)
                if(itr->second.index==u) uname = itr->second.shortest;
            for (itr=m.begin(); itr != m.end(); itr++){
                int v = itr->second.index;
                if (!sptSet[v] && graph[u][v]>0 && uname != 10000000 
                                            && uname+graph[u][v] < itr->second.shortest)
                    itr->second.shortest = uname + graph[u][v]; 
            }
        } 
        //printSolution(m);
    } 

    void printSolution(map<string,struct data> &m) 
    { 
        cout<<"Vertex   Distance from Source"<<endl; 
        map<string, struct data>:: iterator itr; 
        for (itr=m.begin(); itr != m.end(); itr++)
            cout<<itr->first<<" "<<itr->second.shortest<<endl;
    } 
    
    void print(int nodes,vector<vector<int> > graph){
        for(int i=0;i<nodes;i++){
            for(int j=0;j<nodes;j++){
                cout<<graph[i][j]<<" ";
            }
            cout<<endl;
        }
    }
   
};