#define N 9

using Array = array<array<int, N>, N>;

void printArray(const Array& l, bool special = false) {
    if (special) {
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                cout << RevMap[l[y][x] - 1] << " ";
            } cout << endl;
        } cout << endl;
    }
    else {
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                cout << l[y][x] << " ";
            } cout << endl;
        } cout << endl;
    }
}