#include "neural/tensorflow/use.h"
//#include "neural/tensorflow/Tensor.h"
//#include "neural/tensorflow/Model.h"
#include "chess/thc.h"
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

Net::Net(string path) : model(path) {
	model.init();
}

list<float> Net::make(string fen) {
	//list<list<float>> b;
	list<float> tb;
	const char* c = fen.c_str();
	thc::ChessRules cr;
	cr.Forsyth(c);
	string s = cr.ToDebugStr();

	for (auto piece = s.cbegin(); piece != s.cend(); ++piece) {
		switch (*piece) {
		case 'R':
			tb.push_back(5.0);
			break;
		case 'N':
			tb.push_back(3.2);
			break;
		case 'B':
			tb.push_back(3.4);
			break;
		case 'Q':
			tb.push_back(9.0);
			break;
		case 'K':
			tb.push_back(200.0);
			break;

		case 'r':
			tb.push_back(-5.0);
			break;
		case 'n':
			tb.push_back(-3.2);
			break;
		case 'b':
			tb.push_back(-3.4);
			break;
		case 'q':
			tb.push_back(-9.0);
			break;
		case 'k':
			tb.push_back(-200.0);
			break;
			/*case '\n':
				b.push_back(tb);
				tb.clear();
				break;*/
		}
	}
	return tb;
}

float Net::predict(string fen) {
	Tensor input{ model, "input" };
	Tensor output{ model, "output" };
	list<float> d = make(fen);
	vector<float_t> data;
	copy(d.begin(), d.end(), data.begin());
	input.set_data(data, { 1,8,8,1 });
	model.run(input, output);

	return output.get_data<float>()[0];
}