#include "DisjointSet.h"
#include "stdafx.h"
DisjointSet::DisjointSet() {
	
}

void DisjointSet::makeSet(int n) {
	for (int i = 0; i < n; i++) {
		set.push_back(i);
	}
}

void DisjointSet::unionSet(int x, int y) {
	if (set.at(x) == set.at(y)) {
		return;
	}

	int gx = set.at(x);
	int gy = set.at(y);
	for (int i = 1; i < set.size(); i++) {
		if (set.at(i) == gy) {
			set.at(i) = gx;
		}
	}		
}

int DisjointSet::find(int x) {
	for (int i = 1; i < set.size(); i++) {
		if (set.at(i) == set.at(x)) {
			return i;
		}
	}
}