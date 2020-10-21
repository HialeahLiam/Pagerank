#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <vector>

using namespace std;

class Graph {
public:
    Graph(vector<string> nodes, vector< vector<int>> edges) {
        this->nodes = nodes;
        this->edges = edges;
        for (int i = 0; i < nodes.size(); i++) {
            int degree = 0;
            for (vector<int> edge : edges) {
                if (edge[0] == i) {
                    degree++;
                }
            }
            outdegrees.push_back(degree);
            initialProbabilities.push_back(1.0 / nodes.size());
        }
    }
    vector<vector<int>> getEdges() {
        return edges;
    }

    vector<string> getNodes() {
        return nodes;
    }

    vector<int> getDegrees() {
        return outdegrees;
    }

    vector<double> getInitProbs() {
        return initialProbabilities;
    }

private:
    vector<string> nodes;
    vector<vector<int>> edges;
    vector<int> outdegrees;
    vector<double> initialProbabilities;
};

vector<double> updateProbabilities(vector<double> initProbs, vector<string> nodes, vector<vector<int>> edges);

vector<double> getRanks(vector<double> initProbs, double e);

bool isEdge(int x, int y, vector<vector<int>>& edges);

int main()
{
  
    ifstream nodeFile("test_nodes.txt");
    ifstream edgeFile("test_edges.txt");

    vector<vector<int>> edges;
    vector<string> nodes;

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
        cout << "EDge File not opening" << endl;
    }

    if (nodeFile.is_open())
    {
        string node;
        while (nodeFile >> node)
        {
            nodes.push_back(node);
        }
    }
    else
    {
        cout << "node File not opening" << endl;
    }

    Graph graphObject(nodes, edges);

    for (double p : graphObject.getInitProbs()) {
        cout << p << endl;
    }

    for (int p : graphObject.getDegrees()) {
        cout << p << endl;
    }
   
    
}

//vector<double> updateProbabilities(vector<double> initProbs, vector<string> nodes, vector<vector<int>> edges) {
//    int i = 0;
//    int j = 0;
//    int n = initProbs.size();
//    
//    for (int k = 0; k < n; k++)
//    {
//        double newProb = 0.0;
//        
//        for (int l = 0; l < n; l++)
//        {
//            double transProb;
//
//        }
//    }
//}

bool isEdge(int x, int y, vector<vector<int>>& edges) {
    for (vector<int> edge : edges) {
        if (edge[0] == x) {
            if (edge[1] == y) {
                return true;
            }
        }
    }
    return false;
}

