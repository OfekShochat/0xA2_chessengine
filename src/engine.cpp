#include <iostream>
#include <list>
#include <string>
#include <chrono>
#include "mcts/search.h"
#include "utils/sha.h"
#include "neural/tensorflow/use.h"
#include "uci.h"

using namespace std;
using namespace chrono;
int main() {

	hash<string> hasher;

	size_t hash = hasher(__TIMESTAMP__); // making hash of compile timestamp (e.g 'Fri May  5 00:29:26 2017')

	stringstream ss;
	ss << hash;
	string sService = ss.str();
	size_t hash1 = hasher(sService); // hashing again for more diversity in hashing (more difference in hashes)

	cout << " _         __\n";
	cout << "| | \\/ _   __|\n";
	cout << "|_| /\\|_|_|__ " << hex << hash1 << dec << endl;
	//cout << "built on " << __TIMESTAMP__ << endl;

	uci* ucipc = new uci();
	ucipc->loop();
}