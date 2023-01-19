// this is the ONLY file you should edit and submit to D2L

#include "deadlock_detector.h"
#include "common.h"
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <vector>

/// this is the function you need to (re)implement
///
/// parameter edges[] contains a list of request- and assignment- edges
///   example of a request edge, process "p1" resource "r1"
///     "p1 -> r1"
///   example of an assignment edge, process "XYz" resource "XYz"
///     "XYz <- XYz"
///
/// You need to process edges[] one edge at a time, and run a deadlock
/// detection after each edge. As soon as you detect a deadlock, your function
/// needs to stop processing edges and return an instance of Result structure
/// with edge_index set to the index that caused the deadlock, and dl_procs set
/// to contain with names of processes that are in the deadlock.
///
/// To indicate no deadlock was detected after processing all edges, you must
/// return Result with edge_index = -1 and empty dl_procs[].
///

struct Graph {
    std::unordered_map<std::string, std::vector<std::string>> adj_list;
    std::unordered_map<std::string, int> out_counts;
};




std::vector<std::string> topological_search(struct Graph & g) {
    std::vector<std::string> zero_outs; 
    std::vector<std::string> cycle; 
    
     std::unordered_map<std::string, int> counts_cp = g.out_counts;


    
    for(auto a : counts_cp){
        if (a.second == 0){
            zero_outs.push_back(a.first);

        }

    }


    while(zero_outs.size() != 0){
        std::string item= zero_outs.back();
        zero_outs.pop_back();
        
        for (auto x : g.adj_list[item]){
            counts_cp[x]--;
            if (counts_cp[x]==0){
                zero_outs.push_back(x);
                
            }
            

        }

             
        }
        

    
    for (auto y : counts_cp){
        if (y.second != 0 && y.first.find("*") != std::string::npos){
            std::string temp = y.first;
            temp.erase(temp.begin()+0);
            cycle.push_back(temp);
            


        }
    }

    return cycle;
}


void print_graph(struct Graph & g) {
    std::cout << "================== GRAPH ==================\n";
    for (auto [key, vector] : g.adj_list) {
        std::cout << key << ": ";
        for (auto val : vector) {
            std::cout << val << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "================ OUT COUNTS ===============\n";
    for (auto [key, count] : g.out_counts) {
        std::cout << key << ": " << count << std::endl; 
    }
    std::cout << "===========================================\n";
}


Result detect_deadlock(const std::vector<std::string> & edges)
{
    // let's try to guess the results :)
    struct Result result;
    struct Graph g;
    
    std::vector<std::string> edgeList = edges;

    for(int i =0 ; i<(int)edgeList.size() ; i++){
        std::vector<std::string> contents; 
        contents=split(edgeList[i]);
        std::string start;
        std::string end ;

        if(contents[1]=="->"){

            // * represents process.
            // # represents resource.
            start ="*"+contents[0];
            end ="#"+contents[2];

        }
        else if (contents[1]=="<-"){
            start ="#"+contents[2];
            end ="*"+contents[0];

        }

        

        g.adj_list[end].push_back(start);
        if (g.out_counts.count(start) == 0) g.out_counts[start] = 0;
        if (g.out_counts.count(end) == 0) g.out_counts[end] = 0;
        g.out_counts[start]++;
        //print_graph(g);
        
            std::vector<std::string> deadlock=topological_search(g);
            
            // for (int x =0 ; x<(int)deadlock.size();x++){
            //     std::cout<< deadlock[x] << "/n";
            // }

            if(deadlock.size()!=0){
            // deadlock detected.
            result.dl_procs=deadlock;
            result.edge_index=i;

            return result;
        


        }
   


    }
    

    result.edge_index=-1;
    return result;
}
