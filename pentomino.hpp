#include <unordered_map>

#define K 11
#define M 5

const bool P[K][M][M] = { // 11 for the 11 pentominoes
    { // F
        0, 0, 0, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 0
    },
    { // I
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0
    },
    { // L
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0,
        1, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { // N
        0, 1, 0, 0, 0,
        0, 1, 0, 0, 0,
        1, 1, 0, 0, 0,
        1, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { // T
        1, 1, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    // This is where P would go, except there cannot be a P
    // This is because of the 1 of every 2x2 square being unfilled
    { // U
        1, 0, 1, 0, 0,
        1, 1, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { // V
        1, 0, 0, 0, 0,
        1, 0, 0, 0, 0,
        1, 1, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { // W
        1, 0, 0, 0, 0,
        1, 1, 0, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { // X
        0, 1, 0, 0, 0,
        1, 1, 1, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { // Y
        0, 1, 0, 0, 0,
        1, 1, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    },
    { // Z
        1, 1, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0
    }
};

unordered_map<char, int> Map = {
    {'F', 0}, 
    {'I', 1},
    {'L', 2},
    {'N', 3},
    {'T', 4},
    {'U', 5}, 
    {'V', 6},
    {'W', 7},
    {'X', 8},
    {'Y', 9},
    {'Z', 10},
};

unordered_map<int, char> RevMap = {
    {-1, '.'},
    {0, 'F'},
    {1, 'I'},
    {2, 'L'},
    {3, 'N'},
    {4, 'T'},
    {5, 'U'},
    {6, 'V'},
    {7, 'W'},
    {8, 'X'},
    {9, 'Y'},
    {10,'Z'}
};

struct Pentomino {
    bool array[M][M];
    int width, height;
    Pentomino (const bool arr[][M]) {
        int mx = M, my = M;
        for (int x = 0; x < M; x++) {
            for (int y = 0; y < M; y++) {
                if (arr[x][y]) {
                    if (x < mx) mx = x;
                    if (y < my) my = y;
                }
            }
        }
        for (int i = 0; i < M; i++)
            for (int j = 0; j < M; j++)
                array[i][j] = 0;
        for (int x = mx; x < M; x++)
            for (int y = my; y < M; y++)
                array[x - mx][y - my] = arr[x][y];
        width = height = 0;
        for (int x = 0; x < M; x++) {
            for (int y = 0; y < M; y++) {
                if (array[y][x]) {
                    if (x + 1 > width) width = x + 1;
                    if (y + 1 > height) height = y + 1;
                }
            }
        }
    }
    bool operator==(const Pentomino &O) const{
        for (int i = M; i--;)
            for (int j = M; j--;)
                if (array[i][j] ^ O.array[i][j])
                    return false;
        return true;
    }
    Pentomino rotate() {
        bool newArray[M][M];
        for (int i = M; i--;)
            for (int j = M; j--;)
                newArray[i][j] = array[M-1-j][i];
        return Pentomino(newArray);
    }
    Pentomino reflect() {
        bool newArray[M][M];
        for (int i = M; i--;)
            for (int j = M; j--;)
                newArray[i][j] = array[i][M-1-j];
        return Pentomino(newArray);
    }
    vector<Pentomino> orientations() {
        vector<Pentomino> result;

        auto addUnique = [&](const Pentomino &p) {
            for (const auto &q : result) {
                if (p == q) return;
            }
            result.push_back(p);
        };

        Pentomino cur = *this;
        for (int i = 0; i < 4; i++) {
            addUnique(cur);
            cur = cur.rotate();
        }
        cur = this->reflect();
        for (int i = 0; i < 4; i++) {
            addUnique(cur);
            cur = cur.rotate();
        }
        return result;
    }
};

void printPentomino(const Pentomino& p) {
    for (int i = 0; i < p.width; i++) {
        for (int j = 0; j < p.height; j++) {
            cout << p.array[i][j];
        } cout << endl;
    } cout << endl;
}