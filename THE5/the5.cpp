#include "the5.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

std::vector<std::vector<int> > dependencyList, reversedDependencyList;
std::vector<int> color, path, stack;
std::vector<bool> visited;

void swap(int &l, int &r) {
    int temp = l; l = r; r = temp;
}

std::vector<int> reverse(std::vector<int> p) {
    int n = p.size();

    for (int i = 0; i < n/2; i++) {
        swap(p[i], p[n-i-1]);
    }
    return p;
}

bool dfs(int node) { // handles isCompilable and compileOrder
    if (color[node] == GRAY) {
        return false;
    }
    if (color[node] == BLACK) {
        return true;
    }

    color[node] = GRAY; 

    for (int &to: dependencyList[node]) {
        if (!dfs(to)) {
            return false;
        }
    }

    color[node] = BLACK;
    path.push_back(node);
    return true;
}

void dfs1(int node) {
    visited[node] = true;
    for (int to: dependencyList[node]) {
        if (!visited[to]) {
            dfs1(to);
        }
    }
    stack.push_back(node);
}

void dfs2(int node, std::vector<int>& scc) {
    visited[node] = true;
    scc.push_back(node);
    for (int to: reversedDependencyList[node]) {
        if (!visited[to]) {
            dfs2(to, scc);
        }
    }
}

std::vector<std::vector<int> > SCC(const std::vector<std::vector<int>>& dependencyMatrix) {

    int numberOfNodes = dependencyMatrix.size();

    for (int v = 0; v < numberOfNodes; v++) {
        if (!visited[v]) {
            dfs1(v);
        }
    }

    visited.assign(numberOfNodes, false);
    std::vector<std::vector<int>> strongly_connected_components;

    while (!stack.empty()) {
        int v = stack.back(); stack.pop_back();
        if (!visited[v]) {
            std::vector<int> scc;
            dfs2(v, scc);
            if ((scc.size() > 1) || (scc.size() == 1 && dependencyMatrix[scc[0]][scc[0]])) {
                strongly_connected_components.push_back(scc);
            }
        }
    }

    return strongly_connected_components;
}

void run(const std::vector<std::vector<int> >& dependencyMatrix, 
         bool& isCompilable,
         std::vector<int>& compileOrder, 
         std::vector<std::vector<int> >& cyclicDependencies) {

    int numberOfNodes = dependencyMatrix.size();
    dependencyList.resize(numberOfNodes);
    reversedDependencyList.resize(numberOfNodes);

    color.resize(numberOfNodes, WHITE);
    visited.resize(numberOfNodes, false);

    for (int from = 0; from < numberOfNodes; from++) {
        for (int to = numberOfNodes-1; to >= 0; to--) {
            if (dependencyMatrix[from][to]) {
                dependencyList[from].push_back(to);
            }
        }
    }

    for (int v = 0; v < numberOfNodes; v++) {
        for (int to: dependencyList[v]) {
            reversedDependencyList[to].push_back(v);
        }
    }

    isCompilable = true;
    for (int i = numberOfNodes-1; i >= 0; i--) {
        isCompilable &= dfs(i);
    }
 
    compileOrder = reverse(path);
    cyclicDependencies = SCC(dependencyMatrix);
}