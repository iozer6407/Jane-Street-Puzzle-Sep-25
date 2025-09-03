#include <iostream>
#include <vector>
#include <array>

#define N 9
#define M 5
#define K 11

using namespace std;
#include "array.hpp"
#include "pentomino.hpp"
#include "layers.hpp"

bool check(const Pentomino& p, const int& x, const int& y, const Array &l) {
    int sum = 0;
    for (int i = 0; i < p.width; i++) {
        for (int j = 0; j < p.height; j++) {
            sum += p.array[j][i] * l[y + j][x + i];
        }
    } return sum % 5 == 0;
}

bool overlap(const Pentomino& p, const int& x, const int &y, const Array &l) {
    for (int i = 0; i < p.width; i++) {
        for (int j = 0; j < p.height; j++) {
            if (p.array[j][i] && l[y + j][x + i]) return true;
        }
    } return false;
};

void apply(const Pentomino& p, const int &x, const int &y, Array &l, const int &val) {
    for (int i = 0; i < p.width; i++) {
        for (int j = 0; j < p.height; j++) {
            if (p.array[j][i]) l[y + j][x + i] = val;
        }
    }
}

vector<pair<int, int>> findAll(const Pentomino& p, const Array &l) {
    vector<pair<int, int>> out;
    for (int x = 0; x < N - p.width + 1; x++) {
        for (int y = 0; y < N - p.height + 1; y++) {
            if (check(p, x, y, l)) {
                out.push_back({x, y});
            }
        }
    } return out;
}

void printPentomino(const Pentomino& p) {
    for (int i = 0; i < p.width; i++) {
        for (int j = 0; j < p.height; j++) {
            cout << p.array[i][j];
        } cout << endl;
    } cout << endl;
}

int solcnt = 0;
void checkSolution(const Array &a, const Array &l) {
    Array visited{};
    int total = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (a[i][j])
                total++;

    auto dfs = [&](auto self, const int &x, const int & y) -> int {
        if (x < 0 || y < 0 || x >= N || y >= N) return 0;
        if (visited[x][y] || a[x][y] == 0) return 0;

        visited[x][y] = true;
        int count = 1;
        count += self(self, x + 1, y);
        count += self(self, x - 1, y);
        count += self(self, x, y + 1);
        count += self(self, x, y - 1);
        return count;
    };

    int startx = -1, starty = -1;
    for (int i = 0; i < N && startx == -1; i++) {
        for (int j = 0; j < N && starty == -1; j++) {
            if (a[i][j] != 0) {
                startx = i;
                starty = j;
            }
        }
    }

    if (startx == -1) {
        cout << "ERROR!" << endl;
    }

    if (total != dfs(dfs, startx, starty)) {
        return;
    }

    solcnt++;
    
    vector<int> freq(N + 1);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (a[i][j]) {
                freq[l[i][j]]++;
            } else if (must[i][j]) {
                return;
            }
        }
    }

    for (int i = 1; i <= N; i++) {
        if (freq[i] != i) return;
    }

    cout << "Found!" << endl;

    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < M - 1; j++) {
            if (a[i][j] && a[i + 1][j + 1] && a[i + 1][j] && a[i][j + 1]) {
                return;
            }
        }
    }
}

Array generateBad(const Array &l) {
    Array b{};
    for (int col = 0; col < N; col++) {
        if (l[3][col] == 6) break;
        b[3][col] = 1;
    }
    for (int col = N - 1; col >= 0; col--) {
        if (l[5][col] == 2) break;
        b[5][col] = 1;
    }
    for (int row = 0; row < N; row++) {
        if (l[row][6] == 7) break;
        b[row][6] = 1;
    }
    for (int row = N - 1; row >= 0; row--) {
        if (l[row][2] == 3) break;
        b[row][2] = 1;
    }
    return b;
}

void findSolution(const Array &l) {
    const Array b = generateBad(l);
    vector<vector<vector<pair<int, int>>>> V(K); // Pentomino, orientation, i -> (row, col)
    vector<vector<Pentomino>> O(K); // Keeping the orientations
    vector<int> k(K); // Keeping the number of orientations
    for (int idx = 0; idx < K; idx++) {
        O[idx] = Pentomino(P[idx]).orientations();
        k[idx] = O[idx].size();
        V[idx].resize(k[idx]);
        for (int i = 0; i < k[idx]; i++)
            V[idx][i] = findAll(O[idx][i], l);
    }

    // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    // F, I, L, N, T, U, V, W, X, Y, Z
    // I needs to be on the top row    -> 1
    // U needs to be on the top row    -> 5
    // X needs to contain row 4        -> 8
    // N needs to contain row 6        -> 3
    // Z needs to be on the bottom row -> 10
    // V needs to be on the bottom row -> 6
    vector<pair<int, int>> filters = {
        {1, 0}, {5, 0}, {8, 3}, {3, 5}, {10, N - 1}, {6, N - 1}
    };

    auto filter = [&](int p, int row, bool ignore = false) -> vector<pair<int, pair<int,int>>> {
        vector<pair<int, pair<int,int>>> res;
        for (int o = 0; o < k[p]; o++) {
            for (auto &c : V[p][o]) {
                if (overlap(O[p][o], c.first, c.second, b))
                    continue;
                if (ignore || (c.second <= row && row < (c.second + O[p][o].height))) {
                    res.push_back(make_pair(o, c));
                }
            }
        }
        return res;
    };

    // Schema: key, i -> Orientation, (row, col)
    Array a{};
    vector<vector<pair<int, pair<int, int>>>> necessary(6);
    for (int i = 0; i < 6; i++) {
        necessary[i] = filter(filters[i].first, filters[i].second);
    }
    
    // Precompute states with the 6 necessary charachters valid
    vector<Array> possibleStates;

    auto recurse = [&](auto self, int idx) -> void {
        if (idx == 6) { possibleStates.push_back(a); return; }
        for (const auto &x : necessary[idx]) {
            if (overlap(O[filters[idx].first][x.first], x.second.first, x.second.second, a)) continue;
            apply(O[filters[idx].first][x.first], x.second.first, x.second.second, a, idx + 1);
            self(self, idx + 1);
            apply(O[filters[idx].first][x.first], x.second.first, x.second.second, a, 0);
        } 
    };
    recurse(recurse, 0);

    if (possibleStates.size() == 0) {
        return;
    }

    cout << possibleStates.size() << endl;

    // Now choose through the 5 other options
    vector<int> unfiltered = {0, 2, 4, 7, 9};
    vector<vector<pair<int, pair<int, int>>>> sub(5);
    for (int i = 0; i < 5; i++) {
        sub[i] = filter(unfiltered[i], -1, true);
    }

    auto rec2 = [&](auto self, int idx, int cnt) -> void {
        for (const auto &x : sub[idx]) {
            if (overlap(O[unfiltered[idx]][x.first], x.second.first, x.second.second, a)) continue;
            apply(O[unfiltered[idx]][x.first], x.second.first, x.second.second, a, cnt + 7);
            if (cnt == 2) { checkSolution(a, l); }
            else for (int nidx = idx + 1; nidx < 5; nidx++) self(self, nidx, cnt + 1);
            apply(O[unfiltered[idx]][x.first], x.second.first, x.second.second, a, 0);
        } 
    };

    for (const auto &state : possibleStates) {
        a = state;
        rec2(rec2, 0, 0);
    }
}

int main() {
    Array g{};
    g[N/2][N/2] = 1;
    generateAll(g);
    cout << searched << endl;

    sort(possible.begin(), possible.end());
    possible.erase(unique(possible.begin(), possible.end()), possible.end());

    cout << possible.size() << endl;

    for (auto &state : possible) {
        findSolution(state);
        // printArray(state);
        // string s; getline(cin, s);
    }
    cout << solcnt << endl;
}
