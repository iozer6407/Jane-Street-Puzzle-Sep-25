#include <iostream>
#include <vector>
#include <array>

using namespace std;
#include "pentomino.hpp"
#include "array.hpp"
#include "generategrids.hpp"
#include "layers.hpp"

int main() {
    Array g{};
    g[N/2][N/2] = 1;
    wrap(g, 2);
    generateAll(g);
    backtrack(g);
    wrap(g, 3);
    generateAll(g);
    
    for (auto &pl : possible) {
        generategrids(pl);
    }
}
