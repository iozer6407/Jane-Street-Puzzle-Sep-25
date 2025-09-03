bool overlap(const Array &l, const Pentomino& p, const pair<int, int> &pos) {
    for (int xx = 0; xx < p.width; xx++) {
        for (int yy = 0; yy < p.height; yy++) {
            if (p.array[yy][xx] && l[pos.second + yy][pos.first + xx]) return true;
        }
    } return false;
};

void apply(Array &l, const Pentomino& p, const pair<int, int> &pos, const int &val) {
    for (int xx = 0; xx < p.width; xx++) {
        for (int yy = 0; yy < p.height; yy++) {
            if (p.array[yy][xx]) l[pos.second + yy][pos.first + xx] = val;
        }
    }
}

bool checkGrid(const Array &g) {
    // The purpose of this function is to check that:
    //    1. There is no 2x2 that is completely full
    //    2. A single connected component of numbers
    for (int i = 0; i < N - 1; i++)
        for (int j = 0; j < M - 1; j++)
            if (g[i][j] && g[i + 1][j + 1] && g[i + 1][j] && g[i][j + 1])
                return false;

    Array visited{};
    int total = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (g[i][j])
                total++;

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
    
    return true;
}

void generategrids() {
    vector<vector<Pentomino>> V(K); // Pregenerating the orientations
    for (int idx = 0; idx < K; idx++)
        V[idx] = Pentomino(P[idx]).orientations();

    using State = pair<int, pair<int, int>>; // Pregenerating placements
    vector<vector<State>> all(11);

    // I needs to be on the top row    -> 1
    // N needs to contain row 6        -> 3
    // U needs to be on the top row    -> 5
    // V needs to be on the bottom row -> 6
    // X needs to contain row 4        -> 8
    // Z needs to be on the bottom row -> 10
    //-1, 8,-1, 5,-1, 8, 0,-1, 3,-1, 0 
    // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    // F, I, L, N, T, U, V, W, X, Y, Z
    vector<int> filters = {
        -1, 0, -1, 5, -1, 0, 8, -1, 3, -1, 8  
    };

    // Filtering and positioning for both constrained and unconstrained
    auto populate = [&] (const int &idx, const int &row, const bool &ignore) {
        for (int i = 0; i < V[idx].size(); i++) {
            for (int y = 0; y < N - V[idx][i].height + 1; y++) {
            for (int x = 0; x < N - V[idx][i].width + 1; x++) {
                if (ignore || (y <= row && row <= y + V[idx][i].height - 1)) {
                    all[idx].push_back(make_pair(i, make_pair(x, y)));
                }
            } }
        }
    };

    for (int idx = 0; idx < K; idx++)
        populate(idx, filters[idx], filters[idx] == -1);
}