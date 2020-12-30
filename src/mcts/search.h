#pragma once

#include "node.h"
#include <iostream>
#include <string>
#include <chrono>
//#include "../neural/tensorflow/use.h"

using namespace std;
using namespace chrono;

class Search {
public:
	Search(string b, int d = 0, int n = 0, int tt = 0);
	string mBoard;
	int depth;
	int nodes;
	int t;
	int n = 0;
	// defining root
	Node* root = new Node(mBoard);

	// functions/methods
	Node* go();
};