#pragma once
#include <iostream>
#include <string>
#include "mcts/search.h"
#include <list>
#include <sstream>
using namespace std;

class uci {
public:
	void loop();
private:

	//Net* nn = NULL;
	Search* s = NULL;

	string processpos(string cmd);
	void isready(); // implemintation: filesystem::exists("helloworld.txt");
	void uciok();
	void processgo(string cmd, string fen);
	bool init();
};