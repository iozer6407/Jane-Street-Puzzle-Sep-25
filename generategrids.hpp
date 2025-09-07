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

long long runcnt = 0;
bool checkGrid(const Array &g) {
    // The purpose of this function is to check that:
    //    1. There is no 2x2 that is completely full
    //    2. A single connected component of numbers
    runcnt++;
    for (int i = 0; i < N - 1; i++)
        for (int j = 0; j < N - 1; j++)
            if (g[i][j] && g[i + 1][j + 1] && g[i + 1][j] && g[i][j + 1])
                return false;

    Array visited{};
    int total = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (g[i][j])
                total++;
            else if (must[i][j])
                return false;
        }
    }

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
    
    printArray(g);

    return true;
}

void generategrids(const Array &l) {
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
                        if (!check(l, pent, {x, y})) continue;
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
            apply(g, pent, state.pos, 9 - left);
            if (left == 0) {
                if (checkGrid(g)) grids.push_back(g);
            }
            else {
                for (int jdx = idx + 1; jdx <= unc - left; jdx++) self(self, jdx, left - 1);
            }
            apply(g, pent, state.pos, 0);
        }
    };

    for (const auto &istate : all[Map['I']]) { // I loop
        apply(g, get(Map['I'], istate.rot), istate.pos, 1);
        for (const auto &ustate : all[Map['U']]) { // U Loop
            if (ustate.pos.first < istate.pos.first || 
                overlap(g, get(Map['U'], ustate.rot), ustate.pos)
            ) continue;
            apply(g, get(Map['U'], ustate.rot), ustate.pos, 2);
            for (const auto &zstate : all[Map['Z']]) { // Z Loop
                apply(g, get(Map['Z'], zstate.rot), zstate.pos, 3);
                for (const auto &vstate : all[Map['V']]) { // V Loop
                    if (vstate.pos.first < zstate.pos.first || 
                        overlap(g, get(Map['V'], vstate.rot), vstate.pos)
                    ) continue;
                    apply(g, get(Map['V'], vstate.rot), vstate.pos, 4);
                    for (const auto &nstate : all[Map['N']]) { // N Loop
                        if (overlap(g, get(Map['N'], nstate.rot), nstate.pos)) continue;
                        apply(g, get(Map['N'], nstate.rot), nstate.pos, 5);
                        for (const auto &xstate : all[Map['X']]) { // X Loop
                            if (
                                (nstate.pos.second <= 3 && 
                                3 <= nstate.pos.second + get(Map['N'], nstate.rot).height - 1 &&
                                xstate.pos.first < nstate.pos.first) ||
                                overlap(g, get(Map['X'], xstate.rot), xstate.pos)
                            ) continue;
                            apply(g, get(Map['X'], xstate.rot), xstate.pos, 6);
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
    cout << runcnt << " " << grids.size() << endl;
}
