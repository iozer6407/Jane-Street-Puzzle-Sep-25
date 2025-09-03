using Array = array<array<int, N>, N>;

void printArray(const Array& l) {
    for (int y = 0; y < N; y++) {
        for (int x = 0; x < N; x++) {
            cout << l[y][x] << " ";
        } cout << endl;
    } cout << endl;
}


// All the spots that the final
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