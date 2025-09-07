bool overlap(const Array &l, const Pentomino& p, const pair<int, int> &pos) {
    for (int xx = 0; xx < p.width; xx++) {
        for (int yy = 0; yy < p.height; yy++) {
            if (p.array[yy][xx] && l[pos.second + yy][pos.first + xx]) return true;
        }
    } return false;
};

bool check(const Array &l, const Pentomino& p, const pair<int, int> &pos) {
    int sum = 0;
    for (int xx = 0; xx < p.width; xx++) {
        for (int yy = 0; yy < p.height; yy++) {
            sum += p.array[yy][xx] * l[pos.second + yy][pos.first + xx];
        }
    } return sum % 5 == 0;
};

void apply(Array &l, const Pentomino& p, const pair<int, int> &pos, const int &val) {
    for (int xx = 0; xx < p.width; xx++) {
        for (int yy = 0; yy < p.height; yy++) {
            if (p.array[yy][xx]) l[pos.second + yy][pos.first + xx] = val;
        }
    }
}

// All the spots that the final grid must contain
const Array must = {
    0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0,
};

void finalCheck(const Array &g, const Array &l) {
    Array num = g;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (g[i][j]) {
                num[i][j] = l[i][j];
            }
        }
    }

    // dir: 0 = row left→right
    //      1 = row right→left
    //      2 = col top→bottom
    //      3 = col bottom→top
    auto checkFirst = [&](const Array &a, const int& fixed, const int& value, const int& dir) -> bool {
        if (dir == 0) { // row left→right
            for (int j = 0; j < N; j++) {
                if (a[fixed][j]) {
                    return a[fixed][j] == value;
                }
            }
        } 
        else if (dir == 1) { // row right→left
            for (int j = N - 1; j >= 0; j--) {
                if (a[fixed][j]) {
                    return a[fixed][j] == value;
                }
            }
        } 
        else if (dir == 2) { // col top→bottom
            for (int i = 0; i < N; i++) {
                if (a[i][fixed]) {
                    return a[i][fixed] == value;
                }
            }
        } 
        else if (dir == 3) { // col bottom→top
            for (int i = N - 1; i >= 0; i--) {
                if (a[i][fixed]) {
                    return a[i][fixed] == value;
                }
            }
        }
        return false; // no nonzero found, empty row / col
    };
    
    if (!checkFirst(num, 3, 6, 0)) return; // value 6 in row 4 left→right
    if (!checkFirst(num, 5, 2, 1)) return; // value 2 in row 6 right→left
    if (!checkFirst(num, 6, 7, 2)) return; // value 7 in col 7 top→bottom
    if (!checkFirst(num, 2, 3, 3)) return; // value 3 in col 3 bottom→top

    if (!checkFirst(g, 0, Map['I'] + 1, 0)) return; // value I in row 1 left→right
    if (!checkFirst(g, 5, Map['N'] + 1, 0)) return; // value N in row 6 left→right
    if (!checkFirst(g, 8, Map['Z'] + 1, 0)) return; // value Z in row 9 left→right

    if (!checkFirst(g, 0, Map['U'] + 1, 1)) return; // value U in row 1 right→left
    if (!checkFirst(g, 3, Map['X'] + 1, 1)) return; // value X in row 4 right→left
    if (!checkFirst(g, 8, Map['V'] + 1, 1)) return; // value V in row 9 right→left

    printArray(l);
    printArray(num);
    printArray(g, true);
}

long long runcnt = 0;
bool checkGrid(const Array &g, const Array &l) {
    // The purpose of this function is to check that:
    //    1. There is no 2x2 that is completely full
    //    2. A single connected component of numbers
    runcnt++;
    for (int i = 0; i < N - 1; i++)
        for (int j = 0; j < N - 1; j++)
            if (g[i][j] && g[i + 1][j + 1] && g[i + 1][j] && g[i][j + 1])
                return false;


    int total = 0, f[N + 1] = {0};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (g[i][j]) {
                total++;
                f[l[i][j]]++;
            }
            else if (must[i][j])
                return false;
        }
    }

    for (int i = 1; i <= N; i++) {
        if (f[i] != i) return false;
    }

    Array visited{};
    auto dfs = [&](auto self, const int &x, const int & y) -> int {
        if (x < 0 || y < 0 || x >= N || y >= N) return 0;
        if (visited[x][y] || g[x][y] == 0) return 0;

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
            if (g[i][j] != 0) {
                startx = i;
                starty = j;
            }
        }
    }

    if (startx == -1) {
        cout << "ERROR!" << endl;
        return false;
    }

    if (total != dfs(dfs, startx, starty)) {
        return false;
    }

    finalCheck(g, l);
    return true;
}

Array generateBad(const Array &l) {
    Array b{};  // same shape as l, all false/0 initially

    // dir: 0 = row left→right
    //      1 = row right→left
    //      2 = col top→bottom
    //      3 = col bottom→top
    auto placeUntil = [&](int fixed, int value, int dir) {
        if (dir == 0) { // row left→right
            for (int j = 0; j < N; j++) {
                if (l[fixed][j] == value) break; // stop marking when value seen
                b[fixed][j] = true;
            }
        }
        else if (dir == 1) { // row right→left
            for (int j = N - 1; j >= 0; j--) {
                if (l[fixed][j] == value) break;
                b[fixed][j] = true;
            }
        }
        else if (dir == 2) { // col top→bottom
            for (int i = 0; i < N; i++) {
                if (l[i][fixed] == value) break;
                b[i][fixed] = true;
            }
        }
        else if (dir == 3) { // col bottom→top
            for (int i = N - 1; i >= 0; i--) {
                if (l[i][fixed] == value) break;
                b[i][fixed] = true;
            }
        }
    };

    // just call it four times:
    placeUntil(3, 6, 0); // value 6 in row 4 left→right
    placeUntil(5, 2, 1); // value 2 in row 6 right→left
    placeUntil(6, 7, 2); // value 7 in col 7 top→bottom
    placeUntil(2, 3, 3); // value 3 in col 3 bottom→top

    return b;
}

void generategrids(const Array &l) {
    const Array bad = generateBad(l);

    vector<vector<Pentomino>> V(K); // Pregenerating the orientations
    for (int idx = 0; idx < K; idx++)
        V[idx] = Pentomino(P[idx]).orientations();

    struct State {
        int rot;
        pair<int, int> pos;
    };
    vector<vector<State>> all(K);

    auto get = [&] (const int& pentomino, const int &rotation) -> Pentomino{
        return V[pentomino][rotation];
    };

    // I needs to be on the top row    -> 0
    // N needs to contain row 6        -> 5
    // U needs to be on the top row    -> 0
    // V needs to be on the bottom row -> 8
    // X needs to contain row 4        -> 3
    // Z needs to be on the bottom row -> 8
    //-1, 0,-1, 5,-1, 0, 8,-1, 3, -1, 8 
    // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    // F, I, L, N, T, U, V, W, X, Y, Z
    vector<int> filters = {
        -1, 0, -1, 5, -1, 0, 8, -1, 3, -1, 8  
    };

    auto populate = [&] (const int &idx, const int &row, const bool &ignore) {
        for (int rot = 0; rot < V[idx].size(); rot++) {
            const auto pent = get(idx, rot);
            for (int y = 0; y < N - pent.height + 1; y++) {
                for (int x = 0; x < N - pent.width + 1; x++) {
                    if (ignore || (y <= row && row <= y + pent.height - 1)) {
                        if (!check(l, pent, {x, y}) || overlap(bad, pent, {x, y})) continue;
                        all[idx].push_back({rot, {x, y}});
                    }
                }
            }
        }
    };

    for (int idx = 0; idx < K; idx++)
        populate(idx, filters[idx], filters[idx] == -1);
    
    Array g{};
    vector<Array> grids;

    const int unc = 5; // 5 unconstrained pentominoes
    vector<int> mp = {0, 2, 4, 7, 9};
    auto generateFinal = [&](auto self, const int& idx = 0, const int& left = 2) -> void {
        const auto index = mp[idx];
        for (const auto &state : all[index]) {
            const auto pent = get(index, state.rot);
            if (overlap(g, pent, state.pos)) continue;
            apply(g, pent, state.pos, index + 1);
            if (left == 0) {
                checkGrid(g, l);
            }
            else {
                for (int jdx = idx + 1; jdx <= unc - left; jdx++) self(self, jdx, left - 1);
            }
            apply(g, pent, state.pos, 0);
        }
    };

    // I N U V X Z
    // 1 3 5 6 8 10
    // 2 4 6 7 9 11

    // I U Z  V N X
    // 2 6 11 7 4 9

    for (const auto &istate : all[Map['I']]) { // I loop
        apply(g, get(Map['I'], istate.rot), istate.pos, 2);
        for (const auto &ustate : all[Map['U']]) { // U Loop
            if (ustate.pos.first < istate.pos.first || 
                overlap(g, get(Map['U'], ustate.rot), ustate.pos)
            ) continue;
            apply(g, get(Map['U'], ustate.rot), ustate.pos, 6);
            for (const auto &zstate : all[Map['Z']]) { // Z Loop
                apply(g, get(Map['Z'], zstate.rot), zstate.pos, 11);
                for (const auto &vstate : all[Map['V']]) { // V Loop
                    if (vstate.pos.first < zstate.pos.first || 
                        overlap(g, get(Map['V'], vstate.rot), vstate.pos)
                    ) continue;
                    apply(g, get(Map['V'], vstate.rot), vstate.pos, 7);
                    for (const auto &nstate : all[Map['N']]) { // N Loop
                        if (overlap(g, get(Map['N'], nstate.rot), nstate.pos)) continue;
                        apply(g, get(Map['N'], nstate.rot), nstate.pos, 4);
                        for (const auto &xstate : all[Map['X']]) { // X Loop
                            if (
                                (nstate.pos.second <= 3 && 
                                3 <= nstate.pos.second + get(Map['N'], nstate.rot).height - 1 &&
                                xstate.pos.first < nstate.pos.first) ||
                                overlap(g, get(Map['X'], xstate.rot), xstate.pos)
                            ) continue;
                            apply(g, get(Map['X'], xstate.rot), xstate.pos, 9);
                            generateFinal(generateFinal);
                            apply(g, get(Map['X'], xstate.rot), xstate.pos, 0);
                        }
                        apply(g, get(Map['N'], nstate.rot), nstate.pos, 0);
                    }
                    apply(g, get(Map['V'], vstate.rot), vstate.pos, 0);
                }
                apply(g, get(Map['Z'], zstate.rot), zstate.pos, 0);
            }
            apply(g, get(Map['U'], ustate.rot), ustate.pos, 0);
        }
        apply(g, get(Map['I'], istate.rot), istate.pos, 0);
    }
    //cout << runcnt << endl;
}
