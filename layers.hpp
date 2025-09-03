#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

struct Layer {
    int corner;
    vector<pair<int,int>> cells;
};
vector<Layer> history;

void getBounds(const ARRAY& g, int &minR,int &maxR,int &minC,int &maxC){
    minR=N; minC=N; maxR=-1; maxC=-1;
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            if(g[i][j]>0){
                minR=min(minR,i);
                maxR=max(maxR,i);
                minC=min(minC,j);
                maxC=max(maxC,j);
            }
}

bool wrap(ARRAY& g,int corner){
    int minR,maxR,minC,maxC;
    getBounds(g,minR,maxR,minC,maxC);

    int nextVal=0;
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            nextVal=max(nextVal,g[i][j]);
    nextVal++;

    if(corner==0 && (minR==0 || minC==0)) return false;
    if(corner==1 && (minR==0 || maxC==N-1)) return false;
    if(corner==2 && (maxR==N-1 || minC==0)) return false;
    if(corner==3 && (maxR==N-1 || maxC==N-1)) return false;

    Layer layer; layer.corner=corner;

    auto addCell=[&](int r,int c){
        g[r][c]=nextVal;
        layer.cells.push_back({r,c});
    };

    if(corner==0){ for(int j=minC;j<=maxC;j++) addCell(minR-1,j); for(int i=minR;i<=maxR;i++) addCell(i,minC-1); addCell(minR-1,minC-1);}
    else if(corner==1){ for(int j=minC;j<=maxC;j++) addCell(minR-1,j); for(int i=minR;i<=maxR;i++) addCell(i,maxC+1); addCell(minR-1,maxC+1);}
    else if(corner==2){ for(int j=minC;j<=maxC;j++) addCell(maxR+1,j); for(int i=minR;i<=maxR;i++) addCell(i,minC-1); addCell(maxR+1,minC-1);}
    else if(corner==3){ for(int j=minC;j<=maxC;j++) addCell(maxR+1,j); for(int i=minR;i<=maxR;i++) addCell(i,maxC+1); addCell(maxR+1,maxC+1);}

    history.push_back(layer);
    return true;
}

bool backtrack(ARRAY& g){
    if(history.empty()) return false;
    Layer layer=history.back();
    history.pop_back();
    for(auto [r,c]:layer.cells) g[r][c]=0;
    return true;
}

vector<ARRAY> possible;
bool apply(const vector<int> &marked) {
    ARRAY g{};
    g[N/2][N/2] = 1;

    for (int i = 0; i < 8; i++) {
        for (auto &[r, c] : history[i].cells) {
            g[r][c] = marked[i];
        }
    }
    vector<int> freq(N + 1);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            freq[g[i][j]]++;
        }
    }

    //printArray(g);

    for (int i = 1; i <= N; i++)
        if (freq[i] < i) return false;

    possible.push_back(g);
    return true;
}

int cnt = 0;
void check(ARRAY &g) {
    const int L = 8;
    vector<int> marked(L, false);
    marked[0] = 2; // It has to be the two.
    vector<pair<int, pair<int, int>>> known = {
        {5, {0, 4}}, {4, {1, 3}}, {8, {7, 5}}, {9, {8, 4}}
    };

    for (auto &x : known) {
        bool home = false;
        for (int i = 0; i < L && !home; i++) {
            if (marked[i]) continue;
            if (find(history[i].cells.begin(), history[i].cells.end(), x.second) != history[i].cells.end()) {
                marked[i] = x.first; home = true;
            }
        } if (!home) {
            return;
        }
    }

    set<int> row4, col3, col7;
    for (int i = 0; i < L; i++) {
        if (marked[i]) continue;
        for (auto &[r, c] : history[i].cells) {
            if (r == 3) row4.insert(i);
            if (c == 2) col3.insert(i);
            if (c == 6) col7.insert(i);
        }
    }

    for (auto &c3 : col3) {
        if (marked[c3]) continue;
        marked[c3] = 3;
    for (auto &c7 : col7) {
        if (marked[c7]) continue;
        marked[c7] = 7;
        for (auto &r4 : row4) {
            if (marked[r4]) continue;
            marked[r4] = 6;
            if (apply(marked)) {
                cnt++;
            }
            marked[r4] = 0;
        } marked[c7] = 0;
    } marked[c3] = 0;
    }
}

int searched = 0;
void generateAll(ARRAY& g){
    bool b = true;

    for(int dir=0;dir<4;dir++){
        if (wrap(g,dir)) {
            b = false;
            generateAll(g);
            backtrack(g);
        }
    }

    if (b) {
        searched++;
        // for (auto &layer1 : history) {
        //     for (auto &cell : layer1.cells) {
        //         for (auto &layer2 : history) {
        //             if (&layer1 == &layer2) continue;
        //             if (find(layer2.cells.begin(), layer2.cells.end(), cell) != layer2.cells.end()) {
        //                 cout << "CELLS NOT DISJOINT" << endl;
        //             }
        //         }
        //     }
        // }
        check(g);
    }
}
