#include "the6.h"
#include <iostream>

typedef std::vector<std::vector<std::pair<int,int>>>& PAIR_2D_REF;

#define START 0
#define INF 1000000000

int find_structure(std::vector< std::vector<std::pair<int,int> > >& bond_energies, std::vector< std::vector<std::pair<int,int> > >& lowest_energy_structure) {
    int numberOfVertices = bond_energies.size();

    // {distance, vertex}
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    std::vector<bool> inMST(numberOfVertices, false);
    std::vector<int> parent(numberOfVertices, -1);
    std::vector<int> distance(numberOfVertices, INF);

    pq.push({0, START});
    distance[START] = 0;

    while (!pq.empty()) {
        int from = pq.top().second; pq.pop();

        if (inMST[from]) {
            continue;
        }

        inMST[from] = true;

        for (std::pair<int, int>& p : bond_energies[from]) {
            int to = p.first;
            int curr_distance = p.second;

            if (!inMST[to] && curr_distance < distance[to]) {
                distance[to] = curr_distance;
                pq.push({distance[to], to});
                parent[to] = from;
            }
        }
    }

    int lowest_total_energy = 0;

    for (int v = 0; v < numberOfVertices; v++) {
        if (parent[v] == -1) {
            continue;
        }

        for (std::pair<int, int>& p : bond_energies[parent[v]]) {
            int to = p.first;
            int curr_distance = p.second;

            if (to == v) {
                lowest_energy_structure[v].push_back({parent[v], curr_distance});
                lowest_energy_structure[parent[v]].push_back({v, curr_distance});
                lowest_total_energy += curr_distance;
                break;
            }
        }
    }
    return lowest_total_energy;
}

void dfs(int source, int destination, std::vector<int>& chain, std::vector<bool>& visited, PAIR_2D_REF molecule_structure) {
    chain.push_back(source);
    visited[source] = true;

    for (std::pair<int, int>& p : molecule_structure[source]) {
        int to = p.first;

        if (!visited[to]) {
            dfs(to, destination, chain, visited, molecule_structure);
        }
    }

    if (chain.back() != destination) {
        chain.pop_back();
    }
    else {
        return;
    }
}

int find_longest_chain(std::vector< std::vector<std::pair<int,int> > >& molecule_structure, std::vector<int>& chain) {
    int numberOfVertices = molecule_structure.size();
    std::queue<int> q;

    std::pair<int, int> furthest;
    std::vector<bool> visited(numberOfVertices, false);

    q.push(START);

    while (!q.empty()) {
        int level_size = q.size();

        while (level_size--) {
            int from = q.front(); q.pop();
            visited[from] = true;

            for (std::pair<int, int>& p: molecule_structure[from]) {
                int to = p.first;
                if (!visited[to]) {
                    q.push(to);    
                }
            }

            if (q.empty()) {
                furthest.first = from;
            }
        }
    }

    q.push(furthest.first);
    visited.assign(numberOfVertices, false);

    while (!q.empty()) {
        int level_size = q.size();

        while (level_size--) {
            int from = q.front(); q.pop();
            visited[from] = true;

            for (std::pair<int, int>& p: molecule_structure[from]) {
                int to = p.first;
                if (!visited[to]) {
                    q.push(to);    
                }
            }

            if (q.empty()) {
                furthest.second = from;
            }
        }
    }

    std::cout << furthest.first << " " << furthest.second << std::endl; // 3 0

    visited.assign(numberOfVertices, false);

    dfs(furthest.first, furthest.second, chain, visited, molecule_structure);

    return chain.size();
}
