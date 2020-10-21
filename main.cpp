#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <vector>
#include <cmath>
#include <unordered_set>
#include <set>
#include <map>

using namespace std;

class Graph {
public:
    Graph(vector<string> nodes, vector< vector<int>> edges) {
        cout << "making graph" << endl;
        this->nodes = nodes;
        this->edges = edges;
        multiset <int> edgeStarts;
        for (vector<int> edge : edges) {
            edgeStarts.insert(edge[0]);
            edgesSet.insert(to_string(edge[0]) + " " + to_string(edge[1]));
        }
        numberOfNodes = nodes.size();
        for (int i = 0; i < numberOfNodes; i++) {
            int degree = edgeStarts.count(i);
            /*for (vector<int> edge : edges) {
                if (edge[0] == i) {
                    degree++;
                }
                edgesSet.insert( to_string(edge[0]) + " " + to_string(edge[1]) );
            }*/
            outdegrees.push_back(degree);
            initialProbabilities.push_back(1.0 / numberOfNodes);
            currentProbabilities = initialProbabilities;
        }
    }
    unordered_set <string>& getEdges() {
        return edgesSet;
    }

    vector<string> getNodes() {
        return nodes;
    }

    vector<int>& getDegrees() {
        return outdegrees;
    }

    vector<double>& getInitProbs() {
        return initialProbabilities;
    }

    vector<double>& getCurrentProbs() {
        return currentProbabilities;
    }

    void setCurrentProbs(vector<double> probs) {
        currentProbabilities = probs;
    }

    int getNumberOfNodes() {
        return numberOfNodes;
    }

private:
    vector<string> nodes;
    vector<vector<int>> edges;
    vector<int> outdegrees;
    vector<double> initialProbabilities;
    vector<double> currentProbabilities;
    unordered_set <string> edgesSet;
    int numberOfNodes;
};

vector<double> updateProbabilities(Graph& graph);

vector<double> getRanks(Graph graph, double e);

bool isEdge(int x, int y, unordered_set <string>& edges);

vector<vector<int>> retrieveEdges(ifstream& edgeFile);

vector<string> retrieveNodes(ifstream& nodeFile);

vector<double> printRanks(Graph graph);

//class topRanks {
//public:
//    topRanks(int n) {
//        tops = <vector<vector<int>>(2)(n);
//        numberOfRanks = n;
//    }
//
//    vector<int> getTopRanks() {
//        return tops;
//    }
//
//    //void checkRank
//private:
//    vector<vector<double>> tops;
//    int numberOfRanks;
//
//};

int main()
{
  
    ifstream nodeFile("test_nodes.txt");
    ifstream edgeFile("test_edges.txt");

    vector<vector<int>> edges = retrieveEdges(edgeFile);

    vector<string> nodes = retrieveNodes(nodeFile);

    Graph graphObject(nodes, edges);

    printRanks(graphObject);

    ifstream memberFile("members.txt");
    ifstream followFile("follows.txt");
    cout << "retrieving nodes and edges from congress" << endl;
    edges = retrieveEdges(followFile);
    nodes = retrieveNodes(memberFile);
    
    Graph congressGraph(nodes, edges);
    
    updateProbabilities(congressGraph);

    map<double, int> rankMap;
    vector<double> ranks = printRanks(congressGraph);
    for (int i = 0; i < ranks.size(); i++) {
        rankMap[ranks[i]] = i;
    }

    cout << "Top 5 Ranked Congress Members:" << endl;

    for (int i = 0; i < 5; i++) {
        auto it = prev(rankMap.end());
        cout << congressGraph.getNodes()[it->second] << ":  " << it->first << endl;
        rankMap.erase(prev(rankMap.end()));


    }

    ranks = updateProbabilities(congressGraph);
    for (int i = 0; i < ranks.size(); i++) {
        rankMap[ranks[i]] = i;
    }
    

    cout << "Top 5 Ranked Congress Members After Single Iteration:" << endl;

    for (int i = 0; i < 5; i++) {
        auto it = prev(rankMap.end());
        cout << congressGraph.getNodes()[it->second] << ":  " << it->first << endl;
        rankMap.erase(prev(rankMap.end()));


    }

   /* rankMap[5.5] = 3;
    rankMap[7.4] = 2;

    auto end = rankMap.begin();

    cout << end->first << endl;*/

    

    
}

vector<double> printRanks(Graph graph) {
    vector<double> probs = getRanks(graph, 0.001);

    for (int i = 0; i < graph.getNumberOfNodes(); i++) {
        cout << "Pagerank of  " << graph.getNodes()[i] << ":  " << probs[i] << endl;
    }

    cout << endl;
    return probs;
}

vector<vector<int>> retrieveEdges(ifstream& edgeFile) {
    
    vector<vector<int>> edges;

    if (edgeFile.is_open())
    {
        int a;
        int b;
        while (edgeFile >> a >> b)
        {
            vector<int> x;
            x.push_back(a);
            x.push_back(b);
            edges.push_back(x);
        }
        cout << endl;
    }
    else
    {
        cout << "Edge File not opening" << endl;
    }

    return edges; 
}

vector<string> retrieveNodes(ifstream& nodeFile) {
   
     vector<string> nodes;
     if (nodeFile.is_open())
     {
         string node;
         while (getline( nodeFile, node))
         {
             nodes.push_back(node);
         }
     }
     else
     {
         cout << "node File not opening" << endl;
     }

     return nodes; 
}

vector<double> getRanks(Graph graph, double e) {
    cout << "getting ranks..." << endl;
    double TVD = 0.0;
    graph.setCurrentProbs(graph.getInitProbs());
    int iterationCount = 0;
    do
    {
        vector<double> oldProbs = graph.getCurrentProbs();
        vector<double> newProbs = updateProbabilities(graph);
        for (int i = 0; i < graph.getNumberOfNodes(); i++) {
            TVD += abs(newProbs[i] - oldProbs[i]);
        }
        TVD /= 2;
        graph.setCurrentProbs(newProbs);
        iterationCount++; 
    } while (TVD >= e);
    cout << "It took " << iterationCount << " iterations to converge!" << endl;
    return graph.getCurrentProbs();
}

vector<double> updateProbabilities(Graph& graph) {
   
    int n = graph.getNumberOfNodes();
   
    vector<double> newProbs(n,0);
    
    vector<double> oldProbs = graph.getCurrentProbs();
    
    
    for (int j = 0; j < n; j++)
    {       
        
        for (int i = 0; i < n; i++)
        {
            //cout << "outer for loop of update" << endl;
            double pi = oldProbs[i];
            double P;
                        
            int outdegree = graph.getDegrees()[i];
            
       
            if (isEdge(i, j, graph.getEdges())) {
                P = 0.85 / outdegree + 0.15 / n;
            } else if (outdegree > 0) {
                P = 0.15 / n;
            }else {
                P = 1.0 / n;
            }
            
            newProbs[j] += pi * P; 
        }
    }
    graph.setCurrentProbs(newProbs);
    
    return newProbs;
}

bool isEdge(int x, int y, unordered_set <string>& edges) {
    string edge = to_string(x) + " " + to_string(y);
    if (edges.count(edge) > 0) {
        return true;
    }
    /*for (vector<int> edge : edges) {
        if (edge[0] == x) {
            if (edge[1] == y) {
                return true;
            }
        }
    }*/
    return false;
}

