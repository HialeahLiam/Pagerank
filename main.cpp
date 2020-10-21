#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <vector>
#include <cmath>

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
            currentProbabilities = initialProbabilities;
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

    vector<double> getCurrentProbs() {
        return currentProbabilities;
    }

    void setCurrentProbs(vector<double> probs) {
        currentProbabilities = probs;
    }

private:
    vector<string> nodes;
    vector<vector<int>> edges;
    vector<int> outdegrees;
    vector<double> initialProbabilities;
    vector<double> currentProbabilities;
};

vector<double> updateProbabilities(Graph& graph);

vector<double> getRanks(Graph graph, double e);

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
    for (double p : graphObject.getCurrentProbs()) {
        cout << p << endl;
    }
    cout << endl;

    vector<double> probs = updateProbabilities(graphObject);

    probs = getRanks(graphObject, 0.001);

    for (int i = 0; i < graphObject.getNodes().size(); i++) {
        cout << "Pagerank of  " <<graphObject.getNodes()[i] << ":  " << probs[i] << endl;
    }

    cout << endl;

    
}

vector<double> getRanks(Graph graph, double e) {
    double TVD = 0.0;
    graph.setCurrentProbs(graph.getInitProbs());
    do
    {
        vector<double> oldProbs = graph.getCurrentProbs();
        vector<double> newProbs = updateProbabilities(graph);
        for (int i = 0; i < graph.getNodes().size(); i++) {
            TVD += abs(newProbs[i] - oldProbs[i]);
        }
        TVD /= 2;
        graph.setCurrentProbs(newProbs);
    } while (TVD >= e);
    return graph.getCurrentProbs();
}

vector<double> updateProbabilities(Graph& graph) {
    int n = graph.getInitProbs().size();
    vector<double> newProbs(n,0);
    vector<double> oldProbs = graph.getCurrentProbs();
    
    for (int j = 0; j < n; j++)
    {       
        for (int i = 0; i < n; i++)
        {
            double pi = oldProbs[i];
            double P;
            vector<vector<int>> edges = graph.getEdges();
            int outdegree = graph.getDegrees()[i];
            if (isEdge(i, j, edges)) {
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

