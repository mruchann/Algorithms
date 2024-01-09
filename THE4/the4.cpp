#include "the4.h"

#define N 600

std::vector<std::vector<int>> dp(N+1, std::vector<int>(N+1, -1)); // maximum area that can be used until this location

int max(int l, int r) {
    return l >= r ? l : r; 
}

int solve(int i, int j, bool**& possible_plots) {
    if (dp[i][j] != -1) {
        return dp[i][j];
    }

    if (possible_plots[i][j]) {
        return dp[i][j] = i*j;
    }

    int max_usable_area = 0;

    for (int k = 1; k <= j/2; k++) {
        max_usable_area = max(max_usable_area, solve(i, j-k, possible_plots) + solve(i, k, possible_plots));
    }

    for (int k = 1; k <= i/2; k++) {
        max_usable_area = max(max_usable_area, solve(i-k, j, possible_plots) + solve(k, j, possible_plots));
    }

    return dp[i][j] = max_usable_area;
}

int divide_land(int X, int Y, bool** possible_plots) {

/*  MEMOIZATION SOLUTION:
    int max_usable_area = solve(X, Y, possible_plots);
    return X*Y - max_usable_area;
*/

    for (int i = 1; i <= X; i++) {
        for (int j = 1; j <= Y; j++) {
            int max_usable_area = 0;
            if (possible_plots[i][j]) {
                dp[i][j] = i*j;
                continue;
            }

            for (int k = 1; k <= j/2; k++) {
                max_usable_area = max(max_usable_area, dp[i][k] + dp[i][j-k]);
            }

            for (int k = 1; k <= i/2; k++) {
                max_usable_area = max(max_usable_area, dp[k][j] + dp[i-k][j]);
            }
            dp[i][j] = max_usable_area;
        }
    }

    return X*Y - dp[X][Y];
}
