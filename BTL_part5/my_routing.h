/*
 * my_routing.h
 *
 *  Created on: Jun 9, 2020
 *      Author: Dell
 */

#ifndef MY_ROUTING_H_
#define MY_ROUTING_H_

#include <string.h>
#include <stdio.h>
#include <vector>
#include <map>

using namespace std;

#define MAX 10000

vector<int> adj[MAX];

map<int, map<int, int>> suffixTables;

map<int, map<int, int>> prefixTables;

map<int, map<int, int>> corePrefixTables;

map<int, vector<int>> precomputedPaths;

int andressv4[MAX];

int checkChose[MAX / 2];

// The module class needs to be registered with OMNeT++

int isCoreSwitch(int i, int k);
int isAggSwitch(int i, int k);
int isServer(int i, int k);
int isEdgeSwitch(int i, int k);


void buildTable(int i, int k){

        int numEachPod = k * k / 4 + k;

        if(isEdgeSwitch(i, k)) {

            map<int, int> suffixTable;
            int offset = (i/numEachPod)*numEachPod;
            int e = i - offset - k*k/4;
            for (int suffix = 2; suffix <= k / 2 + 1; suffix++) {
                int agg = offset + (e + suffix - 2) % (k / 2) + k/2 + k*k / 4;
                suffixTable.insert(std::pair<int, int>(suffix, agg));
            }
            suffixTables.insert(std::pair<int, map<int, int>>(i, suffixTable));

        }else if(isAggSwitch(i, k)){
            int a = i - (i/numEachPod)*numEachPod - k*k/4 - k/2;
            map<int, int> suffixTable;
            for (int suffix = 2; suffix <= k / 2 + 1; suffix++) {
               int core = a * k / 2 + (suffix + a - 2) % (k / 2) + numEachPod * k;
               suffixTable.insert(std::pair<int, int>(suffix, core));
            }
            // inject to the behavior
            suffixTables.insert(std::pair<int, map<int, int>>(i, suffixTable));

            map<int, int> prefixTable;
            int p = i/numEachPod;
            int offset = (i/numEachPod)*numEachPod;
            for (int e = 0; e < k / 2; e++) {
                int edgeSwitch = offset + k * k / 4 + e;

                int triple = 10 << 16 | p << 8 | e;
                prefixTable.insert(std::pair<int, int>(triple, edgeSwitch));

            }
            prefixTables.insert(std::pair< int, map<int, int> >(i, prefixTable));
        }else if(isCoreSwitch(i, k)){

            map<int, int> corePrefixTable;
            int c = i - k * k * k / 4 - k * k;
            int pair;
            for (int p = 0; p < k; p++) {
                int offset = numEachPod * p;
                int agg = (c / (k / 2)) + k / 2 + k * k / 4 + offset;
                pair = 10 << 8 | p;
                corePrefixTable.insert(std::pair<int, int>(pair, agg));
            }

            corePrefixTables.insert(std::pair< int, map<int, int> >(i, corePrefixTable));
        }
}

void buildAddress(int i, int k) {

        int numEachPod = k * k / 4 + k;

        if(isEdgeSwitch(i, k) || isAggSwitch(i, k)){
            int p = i / numEachPod;
            int offset = numEachPod * p;
            int s = i - offset - k * k / 4;
            andressv4[i] = 10 << 24 | p << 16 | s << 8 | 1;
        }

        if(isCoreSwitch(i, k)){
            int offset = k * k * k / 4 + k * k;
            int i2 = (i - offset)% (k/2) + 1;
            int j = (i - offset + 1 - i2) / (k/2) + 1;

            andressv4[i] = 10 << 24 | k << 16 | j << 8 | i;
        }

        if(isServer(i, k)){
            int p = i / numEachPod;
            int offset = numEachPod * p;
            int e = ( (i - offset) % (k*k /4) ) / (k/2) ;
            int h = i - offset - e * k / 2 + 2;

            andressv4[i] = 10 << 24 | p << 16 | e << 8 | h;

        }

}

int idDesToPort(int current, int next, int k){
    if( isServer(current, k)) {
        return 0;
    }
    else if (isEdgeSwitch(current, k)){
        if(isAggSwitch(next, k)){
            //next is Agg
            int numberEachPod = k*k/4+ k;
            int offset = (next / numberEachPod)*numberEachPod;
            int idAgg = next - offset - k*k/4 - k/2;
            return idAgg + k/2;
        }else{
            //next is Server
            return next % (k/2);
        }
    }
    else if (isAggSwitch(current, k)){
        if(isCoreSwitch(next, k)){
            //next is CoreSwitch
            return next % (k/2) + k/2;
        }else{
            //next is EdgeSwitch
            int numberEachPod = k*k/4+ k;
            int offset = (next / numberEachPod)*numberEachPod;
            int idEdge = next - offset - k*k/4;

            return idEdge;
        }
    }else{
        // current is Core
        int numberEachPod = k*k/4+ k;
        return next / numberEachPod;
    }
}

int isCoreSwitch(int i, int k){
    if(i >= k*k*k / 4 + k*k) return 1;
        else return 0;
}
int isAggSwitch(int i, int k){
    if(i >= k * k * k / 4 + k*k) return 0;

    int numEachPod = k * k / 4 + k;

    if((i % numEachPod) >= (k*k / 4 + k / 2)) return 1;
        else return 0;
}
int isServer(int i, int k){
    if(i >= k * k * k / 4 + k*k) return 0;
    int numEachPod = k * k / 4 + k;

    if((i % numEachPod) < k*k / 4 ) return 1;
            else return 0;
}
int isEdgeSwitch(int i, int k){
    if(i >= k * k * k / 4 + k*k) return 0;
    int numEachPod = k * k / 4 + k;
    if((i % numEachPod) >= k*k / 4 && (i % numEachPod) < (k*k / 4 + k/2)) return 1;
            else return 0;
}

void addEdge(int i, int j){
    adj[i].push_back(j);
    adj[j].push_back(i);
}

void initAdj(int k){

            int numServers = k * k * k / 4;
            int numPodSwitches = k * k;
            int numCores = k * k / 4;
            int numEachPod = k * k / 4 + k;
            for (int p = 0; p < k; p++) {
                int offset = numEachPod * p;

                // between server and edge
                for (int e = 0; e < k / 2; e++) {
                    int edgeSwitch = offset + k * k / 4 + e;
                    for (int s = 0; s < k / 2; s++) {
                        int server = offset + e * k / 2 + s;
                        addEdge(edgeSwitch, server);
                    }
                }

                // between agg and edge
                for (int e = 0; e < k / 2; e++) {
                    int edgeSwitch = offset + k * k / 4 + e;
                    for (int a = k / 2; a < k; a++) {
                        int aggSwitch = offset + k * k / 4 + a;
                        addEdge(edgeSwitch, aggSwitch);
                    }
                }

                // between agg and core
                for (int a = 0; a < k / 2; a++) {
                    int aggSwitch = offset + k * k / 4 + k / 2 + a;
                    for (int c = 0; c < k / 2; c++) {
                        int coreSwitch = a * k / 2 + c + numPodSwitches + numServers;
                        addEdge(aggSwitch, coreSwitch);
                    }
                }

            }
}

int checkContainAdj(int current, int destination){
    for(vector<int>::iterator it = adj[current].begin() ; it != adj[current].end(); ++it){
        if(*it == destination) {
            return 1;
        }
    }
    return 0;
}

int next(int source, int current, int destination, int k) {

        if (isServer(current, k)) {
            return adj[current].at(0);

        } else if (checkContainAdj(current, destination) == 1) {
            return destination;
        } else
            if (isCoreSwitch(current, k)) {

                int address1 = (andressv4[destination] & (255 << 24)) >> 24 ;
                int address2 = (andressv4[destination] & (255 << 16)) >> 16 ;
                int prefix = address1 << 8 | address2;


                map<int , int> corePrefixTable = corePrefixTables[current];

                return corePrefixTable[prefix];

            } else if (isAggSwitch(current, k)) {

                int address1 = (andressv4[destination] & (255 << 24)) >> 24 ;
                int address2 = (andressv4[destination] & (255 << 16)) >> 16 ;
                int address3 = (andressv4[destination] & (255 << 8)) >> 8 ;
                int prefix = address1 << 16 | address2 << 8 | address3;
                int suffix = andressv4[destination] & (255) ;

                map<int, int> prefixTable = prefixTables[current];


                map <int, int> suffixTable = suffixTables[current];

                int checkContainsKey = prefixTable.count(prefix);

                if (checkContainsKey != 0) {
                    return prefixTable[prefix];
                } else {
                    return suffixTable[suffix];
                }
            } else {

                int suffix = andressv4[destination] & (255);
                map<int, int> suffixTable = suffixTables[current];
                return suffixTable[suffix];
            }

    }

vector<int> path(int source, int destination, int k) {
    int pairSrcDes = source << 8 | destination;
    int checkContainsKey = precomputedPaths.count(pairSrcDes);
    if(checkContainsKey != 0){
        return precomputedPaths[pairSrcDes];
    }else{
        vector<int> pathTemp;
        int current = source;
        while (current != destination) {

             current = next(source, current, destination, k);
             pathTemp.push_back(current);

        }

        precomputedPaths.insert(std::pair<int, vector<int>>(pairSrcDes, pathTemp));
        return pathTemp;
    }

}



#endif /* MY_ROUTING_H_ */
