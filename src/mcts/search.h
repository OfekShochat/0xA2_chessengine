#pragma once

#include "node.h"
#include <iostream>
#include <string>
#include <chrono>
#include "../neural/tensorflow/use.h"

using namespace std;
using namespace chrono;

class Search {
public:
	Search(string b, Net* nn, int d = 0, int n = 0, int tt = 0);
	string mBoard;
	int depth;
	int nodes;
	int t;
	Net* net;

	// defining root
	Node* root = new Node(mBoard);

	// functions/methods
	Node* go();
private:
	int n = 0;
};