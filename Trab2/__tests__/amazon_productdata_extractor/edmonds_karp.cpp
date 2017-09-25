/*
 * C++ Program to Implement The Edmonds-Karp Algorithm
 */
#include<cstdio>
#include<cstdio>
#include<queue>
#include<cstring>
#include<vector>
#include<iostream>
 
using namespace std; 
 
#define MAX 6000

int capacities[MAX][MAX];
int flowPassed[MAX][MAX];
vector<int> graph[MAX];
int parentsList[MAX];       
int currentPathCapacity[MAX];  
 
int bfs(int startNode, int endNode)
{
    memset(parentsList, -1, sizeof(parentsList));
    memset(currentPathCapacity, 0, sizeof(currentPathCapacity));
 
    queue<int> q;
    q.push(startNode);
 
    parentsList[startNode] = -2;
    currentPathCapacity[startNode] = 999;
 
    while(!q.empty())
    {
        int currentNode = q.front();
        q.pop();
 
        for(int i=0; i<graph[currentNode].size(); i++)
        {
            int to = graph[currentNode][i];
            if(parentsList[to] == -1)
            {
                if(capacities[currentNode][to] - flowPassed[currentNode][to] > 0)
                {
                    parentsList[to] = currentNode;
                    currentPathCapacity[to] = min(currentPathCapacity[currentNode], 
                    capacities[currentNode][to] - flowPassed[currentNode][to]);
                    if(to == endNode)
                    {
                        return currentPathCapacity[endNode];
                    }
                    q.push(to);
                }
            }
        }
    }
    return 0;
}
 
int edmondsKarp(int startNode, int endNode)
{
    int maxFlow = 0;
      while(true)
    {
        int flow = bfs(startNode, endNode);
        if (flow == 0) 
        {
            break;
        }
        maxFlow += flow;
        int currentNode = endNode;
        while(currentNode != startNode)
        {
            int previousNode = parentsList[currentNode];
            flowPassed[previousNode][currentNode] += flow;
            flowPassed[currentNode][previousNode] -= flow;
            currentNode = previousNode;
        }
    }
    return maxFlow;
}
int main()
{
    //int nodesCount, edgesCount;
    //cout<<"enter the number of nodes and edges\n";
    //cin>>nodesCount>>edgesCount;
 
    int source, sink;
    cout<<"enter the source and sink\n";
    cin>>source>>sink;
 
    int from, to, capacity;
    while(cin >> from >> to >> capacity)
    {
        cout<<"enter the start and end vertex alongwith capacity\n";
    //    cin>>from>>to>>capacity;
 
        capacities[from][to] = capacity;
        graph[from].push_back(to);
 
        graph[to].push_back(from);
    }
 
    int maxFlow = edmondsKarp(source, sink);
 
    cout<<endl<<endl<<"Max Flow is:"<<maxFlow<<endl;
 
}
