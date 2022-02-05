#include "Graph.h"
#include "GraphExceptions.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <fstream>
#include <sstream>
#include <cstdlib>

// Literally do nothing here
// default constructors of the std::vector is enough
Graph::Graph()
{
}

Graph::Graph(const std::string &filePath)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    // Tokens
    std::string tokens[3];

    std::ifstream mapFile(filePath.c_str());
    // Read line by line
    std::string line;
    while (std::getline(mapFile, line))
    {
        // Empty Line Skip
        if (line.empty())
            continue;
        // Comment Skip
        if (line[0] == '#')
            continue;

        // Tokenize the line
        int i = 0;
        std::istringstream stream(line);
        while (stream >> tokens[i])
            i++;

        // Single token (Meaning it is a vertex)
        if (i == 1)
        {
            InsertVertex(tokens[0]);
        }
        // Exactly three tokens (Meaning it is an edge)
        else if (i == 3)
        {
            int weight = std::atoi(tokens[0].c_str());
            if (!ConnectVertices(tokens[1], tokens[2], weight))
            {
                std::cerr << "Duplicate edge on "
                          << tokens[0] << "-"
                          << tokens[1] << std::endl;
            }
        }
        else
            std::cerr << "Token Size Mismatch" << std::endl;
    }
}

void Graph::InsertVertex(const std::string &vertexName)
{
    // TODO
    int sz = vertexList.size();
    for (int i = 0; i < sz; i++)
    {
        if (vertexList[i].name == vertexName)
        {
            throw DuplicateVertexNameException();
        }
    }
    GraphVertex newVertex;
    newVertex.name = vertexName;
    newVertex.edgeCount = 0;
    vertexList.push_back(newVertex);
}

bool Graph::ConnectVertices(const std::string &fromVertexName,
                            const std::string &toVertexName,
                            int weight)
{
    // TODO
    int sz = vertexList.size();
    int fromId = -1;
    int toId = -1;
    for (int i = 0; i < sz; i++)
    {
        if (vertexList[i].name == fromVertexName)
            fromId = i;
        if (vertexList[i].name == toVertexName)
            toId = i;
    }

    if (fromId == -1 || toId == -1)
        throw VertexNotFoundException();
    if (fromId == toId)
        return false;
    for (int i = 0; i < vertexList[fromId].edgeCount; i++)
    {

        int curEdge = vertexList[fromId].edgeIds[i];
        int curVertexTo = edgeList[curEdge].vertexId0;
        if (vertexList[curVertexTo].name == toVertexName)
        {

            return false;
        }
        curVertexTo = edgeList[curEdge].vertexId1;
        if (vertexList[curVertexTo].name == toVertexName)
        {

            return false;
        }
    }
    if (vertexList[fromId].edgeCount == MAX_EDGE_PER_VERTEX || vertexList[toId].edgeCount == MAX_EDGE_PER_VERTEX)
    {
        throw TooManyEdgeOnVertexExecption();
    }

    GraphEdge newEdge;
    newEdge.vertexId0 = fromId;
    newEdge.vertexId1 = toId;
    newEdge.weight = weight;
    newEdge.masked = false;
    edgeList.push_back(newEdge);
    int newEdgeId = edgeList.size() - 1;

    vertexList[fromId].edgeIds[vertexList[fromId].edgeCount] = newEdgeId;
    vertexList[fromId].edgeCount++;

    vertexList[toId].edgeIds[vertexList[toId].edgeCount] = newEdgeId;
    vertexList[toId].edgeCount++;

    return true;
}

bool Graph::ShortestPath(std::vector<int> &orderedVertexIdList,
                         const std::string &from,
                         const std::string &to) const
{
    // TODO

    if (from == to)
    {
        return false;
    }

    int fromId = -1, toId = -1;
    for (int i = 0; i < vertexList.size(); i++)
    {
        if (vertexList[i].name == from)
        {
            fromId = i;
        }
        if (vertexList[i].name == to)
        {
            toId = i;
        }
    }

    if (fromId == -1 || toId == -1)
    {
        throw VertexNotFoundException();
    }

    std::priority_queue<DistanceVertexIdPair> pq;

    std::vector<int> visited(TotalVertexCount(), 0);
    std::vector<int> dis(TotalVertexCount(), -1);
    int order = 0;

    pq.push(DistanceVertexIdPair(fromId, 0));

    while (pq.size())
    {
        DistanceVertexIdPair tmp = pq.top();
        if (visited[tmp.vId])
        {
            pq.pop();
            continue;
        }
        order++;
        visited[tmp.vId] = order;
        dis[tmp.vId] = tmp.currentWeight;
        pq.pop();
        for (int j = 0; j < vertexList[tmp.vId].edgeCount; j++)
        {
            int edgeId = vertexList[tmp.vId].edgeIds[j];

            if (edgeList[edgeId].masked == true)
            {
                continue;
            }
            int weight = tmp.currentWeight + edgeList[edgeId].weight;
            int curVid = (tmp.vId == edgeList[edgeId].vertexId0) ? edgeList[edgeId].vertexId1 : edgeList[edgeId].vertexId0;
            pq.push(DistanceVertexIdPair(curVid, weight));
        }
    }

    if (dis[toId] == -1)
    {
        return false;
    }

    std::vector<int> road;
    int cur = toId;
    road.push_back(cur);

    while (cur != fromId)
    {
        int tmpVid = -1;
        int tmpVidW;
        for (int i = 0; i < vertexList[cur].edgeCount; i++)
        {
            int edgeId = vertexList[cur].edgeIds[i];
            int adjV = (cur == edgeList[edgeId].vertexId0) ? edgeList[edgeId].vertexId1 : edgeList[edgeId].vertexId0;
            if (edgeList[edgeId].masked == true || visited[adjV] == 0)
                continue;
            int wCur = dis[adjV] + edgeList[edgeId].weight;
            if (tmpVid == -1)
            {
                tmpVid = adjV;
                tmpVidW = wCur;
            }
            else if (wCur < tmpVidW || (wCur == tmpVidW && visited[adjV] < visited[tmpVid]))
            {
                tmpVid = adjV;
                tmpVidW = wCur;
            }
        }
        cur = tmpVid;
        road.push_back(cur);
    }

    for (int i = road.size() - 1; i >= 0; i--)
    {
        orderedVertexIdList.push_back(road[i]);
    }
    return true;
}

int Graph::MultipleShortPaths(std::vector<std::vector<int>> &orderedVertexIdList,
                              const std::string &from,
                              const std::string &to,
                              int numberOfShortestPaths)
{

    UnMaskAllEdges();
    int ans = 0;
    for (int i = 0; i < numberOfShortestPaths; i++)
    {
        std::vector<int> cur;
        if (ShortestPath(cur, from, to))
        {
            orderedVertexIdList.push_back(cur);
            ans++;
        }
        else
        {
            return ans;
        }
        int maxEdgeId = -1;

        for (int j = 0; j < orderedVertexIdList[i].size() - 1; j++)
        {
            int fromId = orderedVertexIdList[i][j];
            int toId = orderedVertexIdList[i][j + 1];

            for (int k = 0; k < vertexList[fromId].edgeCount; k++)
            {
                int curEdgeId = vertexList[fromId].edgeIds[k];
                if (edgeList[curEdgeId].vertexId0 == toId || edgeList[curEdgeId].vertexId1 == toId)
                {
                    if (j == 0)
                    {
                        maxEdgeId = curEdgeId;
                    }
                    else if (edgeList[maxEdgeId].weight < edgeList[curEdgeId].weight)
                    {
                        maxEdgeId = curEdgeId;
                    }
                    break;
                }
            }
        }
        if (maxEdgeId == -1)
            return ans;
        edgeList[maxEdgeId].masked = true;
    }
    UnMaskAllEdges();
    return ans;
}

void Graph::MaskEdges(const std::vector<StringPair> &vertexNames)
{
    // TODO
    int sz = vertexNames.size();
    for (int i = 0; i < sz; i++)
    {
        std::string v1Name = vertexNames[i].s0;
        std::string v2Name = vertexNames[i].s1;
        bool chc = true;
        for (int j = 0; j < edgeList.size(); j++)
        {
            int fromId = edgeList[j].vertexId0;
            int toId = edgeList[j].vertexId1;
            if (vertexList[fromId].name == v1Name && vertexList[toId].name == v2Name)
            {
                edgeList[j].masked = true;
                chc = false;
                break;
            }
            else if (vertexList[fromId].name == v2Name && vertexList[toId].name == v1Name)
            {
                edgeList[j].masked = true;
                chc = false;
                break;
            }
        }
        if (chc)
        {
            throw VertexNotFoundException();
        }
    }
}

void Graph::UnMaskEdges(const std::vector<StringPair> &vertexNames)
{
    // TODO
    int sz = vertexNames.size();
    for (int i = 0; i < sz; i++)
    {
        std::string v1Name = vertexNames[i].s0;
        std::string v2Name = vertexNames[i].s1;
        bool chc = true;
        for (int j = 0; j < edgeList.size(); j++)
        {
            int fromId = edgeList[j].vertexId0;
            int toId = edgeList[j].vertexId1;
            if (vertexList[fromId].name == v1Name && vertexList[toId].name == v2Name)
            {
                edgeList[j].masked = false;
                chc = false;
                break;
            }
            else if (vertexList[fromId].name == v2Name && vertexList[toId].name == v1Name)
            {
                edgeList[j].masked = false;
                chc = false;
                break;
            }
        }
        if (chc)
        {
            throw VertexNotFoundException();
        }
    }
}

void Graph::UnMaskAllEdges()
{
    // TODO
    int sz = edgeList.size();
    for (int i = 0; i < sz; i++)
    {
        edgeList[i].masked = false;
    }
}

void Graph::MaskVertexEdges(const std::string &name)
{
    // TODO
    GraphVertex *wantedV = NULL;
    for (int i = 0; i < vertexList.size(); i++)
    {
        if (vertexList[i].name == name)
        {
            wantedV = &vertexList[i];
        }
    }
    if (!wantedV)
    {
        throw VertexNotFoundException();
    }
    for (int i = 0; i < wantedV->edgeCount; i++)
    {
        edgeList[wantedV->edgeIds[i]].masked = true;
    }
}

void Graph::UnMaskVertexEdges(const std::string &name)
{
    // TODO
    GraphVertex *wantedV = NULL;
    for (int i = 0; i < vertexList.size(); i++)
    {
        if (vertexList[i].name == name)
        {
            ;
            wantedV = &vertexList[i];
        }
    }
    if (!wantedV)
    {
        throw VertexNotFoundException();
    }
    for (int i = 0; i < wantedV->edgeCount; i++)
    {
        edgeList[wantedV->edgeIds[i]].masked = false;
    }
}

void Graph::ModifyEdge(const std::string &vName0,
                       const std::string &vName1,
                       float newWeight)
{
    // TODO
    int fromId = -1;
    int toId = -1;
    for (int i = 0; i < vertexList.size(); i++)
    {
        if (vertexList[i].name == vName0)
        {
            fromId = i;
        }
        if (vertexList[i].name == vName1)
        {
            toId = i;
        }
    }
    if (fromId == -1 || toId == -1)
    {
        throw VertexNotFoundException();
    }
    for (int i = 0; i < edgeList.size(); i++)
    {
        if (edgeList[i].vertexId0 == fromId && edgeList[i].vertexId1 == toId)
        {
            edgeList[i].weight = newWeight;
            break;
        }
        else if (edgeList[i].vertexId0 == toId && edgeList[i].vertexId1 == fromId)
        {
            edgeList[i].weight = newWeight;
            break;
        }
    }
    return;
}

void Graph::ModifyEdge(int vId0, int vId1,
                       float newWeight)
{
    // TODO
    for (int i = 0; i < edgeList.size(); i++)
    {
        if (edgeList[i].vertexId0 == vId0 && edgeList[i].vertexId1 == vId1)
        {
            edgeList[i].weight = newWeight;
            break;
        }
        else if (edgeList[i].vertexId0 == vId1 && edgeList[i].vertexId1 == vId0)
        {
            edgeList[i].weight = newWeight;
            break;
        }
    }
    return;
}

void Graph::PrintAll() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    for (size_t i = 0; i < vertexList.size(); i++)
    {
        const GraphVertex &v = vertexList[i];
        std::cout << v.name << "\n";
        for (int j = 0; j < v.edgeCount; j++)
        {
            int edgeId = v.edgeIds[j];
            const GraphEdge &edge = edgeList[edgeId];
            // Skip printing this edge if it is masked
            if (edge.masked)
                continue;

            // List the all vertex names and weight
            std::cout << "-" << std::setfill('-')
                      << std::setw(2) << edge.weight
                      << "-> ";
            int neigVertexId = (static_cast<int>(i) == edge.vertexId0)
                                   ? edge.vertexId1
                                   : edge.vertexId0;
            std::cout << vertexList[neigVertexId].name << "\n";
        }
    }
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

void Graph::PrintPath(const std::vector<int> &orderedVertexIdList,
                      bool sameLine) const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this file !       //
    // ============================= //
    for (size_t i = 0; i < orderedVertexIdList.size(); i++)
    {
        int vertexId = orderedVertexIdList[i];
        if (vertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        const GraphVertex &vertex = vertexList[vertexId];
        std::cout << vertex.name;
        if (!sameLine)
            std::cout << "\n";
        // Only find and print the weight if next is available
        if (i == orderedVertexIdList.size() - 1)
            break;
        int nextVertexId = orderedVertexIdList[i + 1];
        if (nextVertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        // Find the edge between these two vertices
        int edgeId = INVALID_INDEX;
        if (vertexId < static_cast<int>(vertexList.size()) &&
            nextVertexId < static_cast<int>(vertexList.size()))
        {
            // Check all of the edges of vertex
            // and try to find
            const GraphVertex &fromVert = vertexList[vertexId];
            for (int i = 0; i < fromVert.edgeCount; i++)
            {
                int eId = fromVert.edgeIds[i];
                // Since the graph is not directional
                // check the both ends
                if ((edgeList[eId].vertexId0 == vertexId &&
                     edgeList[eId].vertexId1 == nextVertexId) ||
                    (edgeList[eId].vertexId0 == nextVertexId &&
                     edgeList[eId].vertexId1 == vertexId))
                {
                    edgeId = eId;
                    break;
                }
            }
        }
        if (edgeId != INVALID_INDEX)
        {
            const GraphEdge &edge = edgeList[edgeId];
            std::cout << "-" << std::setfill('-')
                      << std::setw(2)
                      << edge.weight << "->";
        }
        else
        {
            std::cout << "-##-> ";
        }
    }
    // Print endline on the last vertex if same line is set
    if (sameLine)
        std::cout << "\n";
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

int Graph::TotalVertexCount() const
{
    // TODO
    return vertexList.size();
}

int Graph::TotalEdgeCount() const
{
    // TODO
    return edgeList.size();
}

std::string Graph::VertexName(int vertexId) const
{
    // TODO
    if (vertexId < 0 || vertexId >= vertexList.size())
    {
        return "";
    }
    return vertexList[vertexId].name;
}

int Graph::TotalWeightInBetween(std::vector<int> &orderedVertexIdList)
{
    // TODO
    int totalWeight = 0;
    for (int i = 0; i < orderedVertexIdList.size() - 1; i++)
    {
        int fromId = orderedVertexIdList[i];
        int toId = orderedVertexIdList[i + 1];

        if (fromId < 0 || toId < 0 || fromId >= TotalVertexCount() || toId >= TotalVertexCount())
        {
            throw VertexNotFoundException();
        }
        bool chc = true;
        for (int j = 0; j < vertexList[fromId].edgeCount; j++)
        {
            int curEdgeId = vertexList[fromId].edgeIds[j];

            if (edgeList[curEdgeId].vertexId0 == toId || edgeList[curEdgeId].vertexId1 == toId)
            {
                if (edgeList[curEdgeId].masked == true)
                {
                    return -1;
                }
                chc = false;
                totalWeight += edgeList[curEdgeId].weight;
                continue;
            }
        }
        if (chc)
        {
            return -1;
        }
    }
    return totalWeight;
}