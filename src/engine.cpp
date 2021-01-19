
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
#include <iostream> 
#include <string> 
#include <sstream> 
#include <vector> 
#include <chrono>
#include "mcts/search.h"
//#include "utils/sha.h"
//#include "neural/tensorflow/use.h"
#include "uci.h"
#include "utils/commandline.h"

using namespace std;
using namespace chrono;
int main(int argc, const char* argv[]) {

	string version = "0.0.5";

	hash<string> hasher;

	size_t hash = hasher(__TIMESTAMP__); // making hash of compile timestamp (e.g 'Fri May  5 4:29:26 2017')

	string weights = "./net.pb";

	stringstream ss;
	ss << hash;
	string sService = ss.str();
	size_t hash1 = hasher(sService); // hashing again for more diversity in hashing (more difference in hashes)

	// arguments
	int rply = 4;

	cout << " _         __\n";
	cout << "| | \\/ _   __|\n";
	cout << "|_| /\\|_|_|__ " << hex << hash1 << dec << endl;
	//cout << "built on " << __TIMESTAMP__ << endl;

	/*
	argparse::ArgumentParser parser("0xA2", "the official 0xA2 chess engine yay");
	parser.add_argument("-w", "--weights", "weights");
	parser.add_argument("-rply", "--rollout-ply", "rply");

	auto err = parser.parse(argc, argv);
	if (err) {
		cout << err << endl;
		return -1;
	}
	if (parser.exists("rply")) {
		rply = parser.get<unsigned int>("rply");
	}
	if (parser.exists("weights")) {
		weights = parser.get<string>("weights");
	}*/

	uci* ucipc = new uci();
	ucipc->loop();
	//Net* nn = new Net(weights);
	//Search* s = new Search("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", nn, 0, 100, 0);
}