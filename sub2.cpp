#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include "graph.h"
using namespace std;
 
map<string,struct data> m;
vector<vector<int> > gra;
string start;

int main(int argc,char* argv[]){
    //openfile
    string arg=argv[1];
    arg = "./"+arg+"/tp.data";
    ifstream infile(arg.c_str());
    if(!infile) return 1;

    //read total record
    int nodes,edges,total_time,ready;
    infile>>nodes>>edges>>total_time>>ready;
    gra.resize(nodes,vector<int>(nodes,0));
    Graph g;
    start=g.readData(infile,nodes,edges,m,gra);
    g.dijkstra(gra,start,m);

    // string arg1="./"+arg+"/ans1.txt";
    // ofstream outfile(arg1.c_str());

    infile.close();
    //outfile.close();
}
void maxHappy(){

}

//delete couldn't arrive node
void prune(){

}
//count shortest path from start to each node
void short_Path(){

}
