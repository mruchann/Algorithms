#include "the7.h"

int max(int l, int r) {
    return l >= r ? l : r;
}

void get_infection_scores(const std::vector<std::vector<std::pair<int, int>>>& network,
                          std::vector<float>& infection_scores) {
        
    int N = network.size();

    std::vector<std::vector<int>> delay_graph(N, std::vector<int>(N, INT_MAX));
    infection_scores.resize(N);

    for (int from = 0; from < N; from++) {
        for (const std::pair<int, int>& p: network[from]) {
            int to = p.first;
            int delay = p.second;
            delay_graph[from][to] = delay;
        }
    }

    for (int k = 0; k < N; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (delay_graph[i][k] == INT_MAX || delay_graph[k][j] == INT_MAX) {
                    continue;
                }

                int curr_delay = delay_graph[i][k] + delay_graph[k][j];  

                if (curr_delay < delay_graph[i][j]) {
                    delay_graph[i][j] = curr_delay;
                }
            }
        }
    }

    int max_delay = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (delay_graph[i][j] != INT_MAX) {
                max_delay = max(max_delay, delay_graph[i][j]);
            }
        }
    }

    for (int from = 0; from < N; from++) {
        float delay_sum = 0;
        for (int to = 0; to < N; to++) {
            if (from == to) {
                continue;
            }

            if (delay_graph[from][to] == INT_MAX) {
                delay_graph[from][to] = max_delay + 1;
            }

            delay_sum += delay_graph[from][to];
        }

        infection_scores[from] = (N-1) / delay_sum;
    }


}