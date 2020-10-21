#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <vector>

using namespace std;

class PageRank {
public:
    PageRank(string n) {
        name = n;
        cout << "Object created" << endl;
    }
    string getName() {
        return name;
    }
    void setName(string n) {
        name = n;
    }
private:
    string name;


};



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

    vector<double> probabilities;
    for (int i = 0; i < nodes.size(); i++)
    {
        probabilities.push_back(1.0/nodes.size());
        cout << probabilities[i] << endl;
    }

  
  
}

