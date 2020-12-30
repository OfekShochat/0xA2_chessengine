#include "evaluate.h"
#include "chess/thc.h"

float evaluator::evaluate(string board) {
    thc::ChessRules cr;
    const char* c = board.c_str();
    cr.Forsyth(c);

    float evaluation = 0.0;

    string b = cr.ToDebugStr();
    for (auto piece = b.cbegin(); piece != b.cend(); ++piece) {
		switch (*piece) {
		case 'P': 
			evaluation += 0.1;
			break;
		case 'R':
            evaluation += 0.5;
			break;
		case 'N':
			evaluation += 0.32;
			break;
		case 'B':
			evaluation += 0.34;
			break;
		case 'Q':
			evaluation += 0.9;
			break;
		case 'K':
			evaluation += 20.0;
			break;

		case 'p': 
			evaluation += -0.1;
			break;
		case 'r':
			evaluation += -0.5;
			break;
		case 'n':
			evaluation += -0.32;
			break;
		case 'b':
			evaluation += -0.34;
			break;
		case 'q':
			evaluation += -0.9;
			break;
		case 'k':
			evaluation += -20.0;
			break;
		}
	}

	if (cr.AttackedSquare(thc::e5, true))
		evaluation += 0.01;
	if (cr.AttackedSquare(thc::e4, true))
		evaluation += 0.01;
	if (cr.AttackedSquare(thc::d4, true))
		evaluation += 0.01;
	if (cr.AttackedSquare(thc::d5, true))
		evaluation += 0.01;

	if (cr.AttackedSquare(thc::e5, false))
		evaluation -= 0.01;
	if (cr.AttackedSquare(thc::e4, false))
		evaluation -= 0.01;
	if (cr.AttackedSquare(thc::d4, false))
		evaluation -= 0.01;
	if (cr.AttackedSquare(thc::d5, false))
	//cout << evaluation << endl;
    return evaluation;
}