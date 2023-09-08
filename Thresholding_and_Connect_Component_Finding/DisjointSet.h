#pragma once
#include <vector>
class DisjointSet
{
public:
    DisjointSet();
    void makeSet(int n);
    void unionSet(int x, int y);
    int find(int x);
    std::vector<int> set;
};

