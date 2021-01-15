
/*
0xA2_chess-engine
Copyright (C) 2020-2021  Ofek Shochat

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "node.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
//#include "../neural/tensorflow/use.h"

using namespace std;
using namespace chrono;

class Search {
public:
	Search(string b, int d = 0, int n = 0, int tt = 0, bool timemm = false);
	string mBoard;
	int depth;
	int nodes;
	int t;
	int n = 0;
	bool timemanage;
	// defining root
	Node* root = new Node(mBoard);

	// functions/methods
	Node* go();
private:
	int timem();
};