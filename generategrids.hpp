bool overlap(const Pentomino& p, const int& x, const int &y, const Array &l) {
    for (int xx = 0; xx < p.width; xx++) {
        for (int yy = 0; yy < p.height; yy++) {
            if (p.array[yy][xx] && l[y + yy][x + xx]) return true;
        }
    } return false;
};

void apply(const Pentomino& p, const int &x, const int &y, Array &l, const int &val) {
    for (int xx = 0; xx < p.width; xx++) {
        for (int yy = 0; yy < p.height; yy++) {
            if (p.array[yy][xx]) l[y + yy][x + xx] = val;
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

    // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    // F, I, L, N, T, U, V, W, X, Y, Z
    // I needs to be on the top row    -> 1
    // U needs to be on the top row    -> 5
    // X needs to contain row 4        -> 8
    // N needs to contain row 6        -> 3
    // Z needs to be on the bottom row -> 10
    // V needs to be on the bottom row -> 6
}