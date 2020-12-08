#include "uci.h"
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include "mcts/search.h"
#include "chess/thc.h"

using namespace std;

void uci::loop() {
	string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

	while (true) {
		string cmd;
		cin >> cmd;
		//istringstream& ss(cmd);

		string first = cmd.substr(0, cmd.find(" "));

		//cout << first << endl;

		if (first == "uci") {
			uciok();
		}
		else if (first == "quit") {
			break;
		}
		else if (first == "isready") {
			isready();
		}

		else if (first == "go") {
			processgo(cmd, fen);
		}
		else if (first == "position") {
			fen = processpos(cmd);
		}
		else {
			cout << "invalid command" << endl;
		}
	}
}

void uci::uciok() {
	cout << "id name 0xA2" << endl;
	cout << "id author Ofek Shochat" << endl;
	cout << "option name netPath type pathString default ./networks/net.pb" << endl;
	cout << "uciok" << endl;
}

bool uci::init() {
	// here should be net object definition(check in here function if it has been definedand if not, define it).
	ifstream infile("./networks/net.pb");
	if (infile) {
		nn = new Net("./networks/net.pb");
		return true;
	}
	return false;
}

void uci::isready() {
	// temp implementation. 
	if (init()) {
		cout << "readyok" << endl;
	}
	else {
		cout << "notreadyok" << endl;
	}
}

void uci::processgo(string cmd, string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {


	if (nn == NULL) {
		if (!init()) {
			cout << "errono 2: problem with nn initialization.";
		}
	}

	string t;

	istringstream tokens(cmd);

	bool d = false;
	bool n = false;
	bool tt = false;
	tokens >> t;
	while (tokens >> t) {
		if (t == "depth") {
			d = true;
		}
		else if (t == "nodes") {
			n = true;
		}
		else if (t == "movetime") {
			tt = true;
		}

		if (d) {
			s = new Search(fen, nn, stoi(t), 0, 0);
			break;
		}
		else if (n) {
			s = new Search(fen, nn, 0, stoi(t), 0);
			break;
		}
		else if (tt) {
			s = new Search(fen, nn, 0, 0, stoi(t));
			break;
		}
		cout << "d: " << d << " n: " << n << " tt: " << tt << endl;
	}
	s->go();
}

string uci::processpos(string cmd) {
	string t;

	istringstream tokens(cmd);

	bool f = false;
	bool more = false;
	bool moves = false;

	string ff;
	tokens >> t;
	while (tokens >> t) {
		if (t == "startpos")
			return "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
		else if (t == "fen") {
			f = true;
		}

		else if (f) {
			ff = t;
			more = true;
		}
		else if (more) {
			if (t == "moves")
				moves = true;
		}
		else if (moves) {
			
		}
	}
	return ff;
}