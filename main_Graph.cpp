#include "Graph.h"
#include "GraphExceptions.h"
#include <iostream>

using namespace std;
int main()
{
    Graph g;

    g.InsertVertex("A");
    g.InsertVertex("B");
    g.InsertVertex("C");
    g.InsertVertex("D");
    g.InsertVertex("E");
    g.InsertVertex("F");
    g.InsertVertex("G");

    g.ConnectVertices("A", "B", 3);
    g.ConnectVertices("A", "D", 1);
    g.ConnectVertices("A", "C", 4);
    g.ConnectVertices("B", "D", 2);
    g.ConnectVertices("B", "E", 10);
    g.ConnectVertices("E", "G", 6);
    g.ConnectVertices("D", "E", 2);
    g.ConnectVertices("D", "F", 8);
    g.ConnectVertices("D", "C", 2);
    g.ConnectVertices("C", "F", 5);
    g.ConnectVertices("G", "F", 1);
    g.ConnectVertices("G", "D", 4);

    /*try
    {
        g.ConnectVertices("A", "J", 9);
    }
    catch (const TooManyEdgeOnVertexExecption &e)
    {
        std::cout << e.what() << '\n';
    }*/
    // g.PrintAll();
    vector<vector<int>> ans;
    // g.ShortestPath(ans,"A","B");
    // g.ShortestPath(ans,"A","C");
    // g.ShortestPath(ans,"A","D");
    // g.ShortestPath(ans,"A","E");
    // g.ShortestPath(ans,"A","F");
    int a = g.MultipleShortPaths(ans, "A", "B", 4);

    // for(int i=0;i<ans.size();i++)
    // {
    //     cout<<ans[i]<<" ";
    // }
    for (int i = 0; i < a; i++)
    {
        g.PrintPath(ans[i]);
    }

    return 0;
}