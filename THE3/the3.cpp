#include "the3.h"
#include <iostream>

#define MAX_STRING_LENGTH 10000
#define MAX_WEIGHT 10
#define MAX_CALL_COUNT 1000000

int dp[MAX_STRING_LENGTH+1][MAX_STRING_LENGTH+1];

int GAP, MISMATCH, MATCH, SIZE_A, SIZE_B;

std::string A, B;

bool stackOverflow;

int max2(int left, int right) {
    return left >= right ? left : right;
}

int max(int left, int mid, int right) {
    return max2(max2(left, mid), right);
}

int solve(int iA, int iB, std::string& s, int& call_count) {
    call_count++;

    if (call_count >= MAX_CALL_COUNT) {
        stackOverflow = true;
        return INT_MIN;
    }

    if (iA < 0 && iB < 0) {
        s = "";
        return 0;
    }
    if (iA < 0) {
        for (int i = iB; i >= 0; i--) {
            s += '_';
        }
        return (iB+1) * (-GAP);
    }
    if (iB < 0) {
        for (int i = iA; i >= 0; i--) {
            s += '.';
        }
        return (iA+1) * (-GAP);
    }

    std::string stay_string, skipA_string, skipB_string;
    int stay = INT_MIN, skipA = INT_MIN, skipB = INT_MIN;

    stay = solve(iA-1, iB-1, stay_string, call_count) + (A[iA] == B[iB] ? MATCH : -MISMATCH);
    skipA = solve(iA-1, iB, skipA_string, call_count) - GAP;
    skipB = solve(iA, iB-1, skipB_string, call_count) - GAP;

    int max_score = max(stay, skipA, skipB);

    if (max_score == stay) {
        if (A[iA] == B[iB]) {
            stay_string += A[iA];
        }
        else {
            stay_string += '!';
        }
        s = stay_string;
    }
    else if (max_score == skipA) {
        skipA_string += '.';
        s = skipA_string;
    }
    else if (max_score == skipB) {
        skipB_string += '_';
        s = skipB_string;
    }

    if (call_count >= MAX_CALL_COUNT) {
        stackOverflow = true;
        return INT_MIN;
    }

    return max_score;
}

int recursive_alignment(std::string sequence_A, std::string sequence_B, int gap, int mismatch, int match, std::string &possible_alignment, int call_count) {
    GAP = gap;
    MISMATCH = mismatch;
    MATCH = match;
    SIZE_A = sequence_A.length();
    SIZE_B = sequence_B.length();

    A = sequence_A;
    B = sequence_B;

    int max_score = solve(SIZE_A-1, SIZE_B-1, possible_alignment, call_count);

    if (stackOverflow) {
        possible_alignment = "STACK LIMIT REACHED";
    }

    return max_score;
}

int dp_table_alignment(std::string sequence_A, std::string sequence_B, int gap, int mismatch, int match, std::string &possible_alignment) {
    GAP = gap;
    MISMATCH = mismatch;
    MATCH = match;
    SIZE_A = sequence_A.length();
    SIZE_B = sequence_B.length();

    A = sequence_A;
    B = sequence_B;
    
    int highest_alignment_score = INT_MIN;

    for (int i = 0; i <= SIZE_A; i++) {
        for (int j = 0; j <= SIZE_B; j++) {
            if (i == SIZE_A) {
                dp[i][j] = -GAP * (SIZE_B - j);
            }
            if (j == SIZE_B) {
                dp[i][j] = -GAP * (SIZE_A - i);
            }
        }
    }

    for (int iA = SIZE_A-1; iA >= 0; iA--) {
        for (int iB = SIZE_B-1; iB >= 0; iB--) {
            int stay = dp[iA+1][iB+1] + (A[iA] == B[iB] ? MATCH : -MISMATCH);
            int skipA = dp[iA+1][iB] - GAP;
            int skipB = dp[iA][iB+1] - GAP;
            int max_score = max(stay, skipA, skipB);

            if (stay == max_score) {
                dp[iA][iB] = stay;
            }
            else if (skipA == max_score) {
                dp[iA][iB] = skipA;
            }
            else if (skipB == max_score) {
                dp[iA][iB] = skipB;
            }
        }
    }

    int iA = 0, iB = 0;

    while (iA < SIZE_A && iB < SIZE_B) {
        int stayed = dp[iA+1][iB+1];
        int skippedA = dp[iA+1][iB];
        int skippedB = dp[iA][iB+1];
        int max_score = max(stayed, skippedA, skippedB);

        if (stayed == max_score && dp[iA][iB] == dp[iA+1][iB+1] + MATCH) {
            possible_alignment += A[iA];
            iA++;
            iB++;
        }
        else if (stayed == max_score && dp[iA][iB] + MISMATCH == dp[iA+1][iB+1]) {
            possible_alignment += '!';
            iA++;
            iB++;
        }
        else if (skippedB == max_score) {
            possible_alignment += '_';
            iB++;
        }
        else if (skippedA == max_score) {
            possible_alignment += '.';
            iA++;
        }
    }

    while (iA < SIZE_A) {
        possible_alignment += '.';
        iA++;
    }

    while (iB < SIZE_B) {
        possible_alignment += '_';
        iB++;
    }
    
    return dp[0][0];    
}
