#pragma once

#include <Mymap.cpp>
#include <Matrix.cpp>
#include <MyQueue.cpp>

#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

class MaxFlow
{
    public:
        MaxFlow(const std::string& filename);
        ~MaxFlow();
        int get_maxflow();

    private:
        Matrix capacityMatrix;
        MyMap<int, std::string> vertexes;
        int* parent;
        int mapSize = 0;
        int maxFlow = 0;

        void loadFromFile(const std::string& filename);
        void parseFileForMapSize(const std::string& filename);
        void calculateMaxFlow(const int& sourceIndex, const int& sinkIndex);
        bool bfs(const int& sourceIndex, const int& sinkIndex);
};

/******************************************************************************
 *
 *                          PUBLIC METHODS
 *
 *****************************************************************************/

MaxFlow::MaxFlow(const std::string& filename)
{
    loadFromFile(filename);
    calculateMaxFlow(0, mapSize-1);
}

MaxFlow::~MaxFlow()
{
    delete[] parent;
}

int MaxFlow::get_maxflow()
{
    return maxFlow;
}


/******************************************************************************
 *
 *                          PRIVATE METHODS
 *
 *****************************************************************************/
void MaxFlow::loadFromFile(const std::string& filename)
{
    parseFileForMapSize(filename);
    mapSize = vertexes.get_size();
    parent = new int[mapSize];

    Matrix matrix(mapSize, mapSize);
    capacityMatrix = matrix;
    // fill adjacencyMatrix matrix with zeros
    for (int i = 0; i < mapSize; i++)
        for (int j = 0; j < mapSize; j++)
                capacityMatrix(i, j) = 0;
    
    std::string line;
    std::string weight;
    std::string firstVertex;
    std::string secondVertex;

    std::ifstream flightData(filename);
    if (flightData.is_open()) {
        while (getline(flightData, line)) {
            firstVertex = line.substr(0, line.find(' '));
            line.erase(0, line.find(' ') + 1);

            secondVertex = line.substr(0, line.find(' '));
            line.erase(0, line.find(' ') + 1);

            capacityMatrix(vertexes.findByValue(firstVertex), vertexes.findByValue(secondVertex)) = std::stod(line);
        }
    } else {
        throw std::invalid_argument("Cannot open the file");
    }
}

//  Fills the map with unique vertexes from file
void MaxFlow::parseFileForMapSize(const std::string& filename)
{
    std::string line;
    std::string cost;
    std::string firstVertex;
    std::string secondVertex;

    std::ifstream vertexList(filename);
    if (vertexList.is_open()) {
        while (getline(vertexList, line)) {
            firstVertex = line.substr(0, line.find(' '));
            line.erase(0, line.find(' ') + 1);
            if (!(vertexes.has(firstVertex))) {
                vertexes.insert(vertexes.get_size(), firstVertex);
            }

            secondVertex = line.substr(0, line.find(' '));
            line.erase(0, line.find(' ') + 1);
            if (!(vertexes.has(secondVertex))) {
                vertexes.insert(vertexes.get_size(), secondVertex);
            } 
        }
    } else {
        throw std::invalid_argument("Cannot open the file");
    }
    vertexList.close();
}

//  Calculates maxFlow using the Edmonds-Karp algorithm
void MaxFlow::calculateMaxFlow(const int& sourceIndex, const int& sinkIndex)
{
    int v = 0, u = 0;
    while (bfs(sourceIndex, sinkIndex)) {
        int pathFlow = INT_MAX;
        //  finding the minimum flow through the path found by bfs
        for (v = sinkIndex; v != sourceIndex; v = parent[v]) {
            u = parent[v];
            pathFlow = pathFlow < capacityMatrix(u, v) ? pathFlow : capacityMatrix(u, v);
        }
        //  updating residual capacities and reversing edges along the path
        for (v = sinkIndex; v != sourceIndex; v = parent[v]) {
            u = parent[v];
            capacityMatrix(u, v) -= pathFlow;
            capacityMatrix(v, u) += pathFlow;
        }
        maxFlow += pathFlow;
    }
}


bool MaxFlow::bfs(const int& sourceIndex, const int& sinkIndex) {
    bool visited[mapSize] = {0};
    Queue<int> queue;
    queue.push(sourceIndex);
    visited[sourceIndex] = true;
    parent[sourceIndex] = -1;
    while (!queue.empty()) {
        int u = queue.front();
        queue.pop();
        for (int v = 0; v < mapSize; v++) {
            if (visited[v] == false && capacityMatrix(u, v) > 0) {
                queue.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    // if sinkIndex is reached from sourceIndex return true
    return visited[sinkIndex] == true;
}


