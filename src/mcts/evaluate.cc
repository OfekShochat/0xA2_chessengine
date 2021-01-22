
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
#include "evaluate.h"
#include "chess/thc.h"

float evaluator::evaluate(string board) {
    thc::ChessRules cr;
    const char* c = board.c_str();
    cr.Forsyth(c);

    double evaluation = 0.0;

    string b = cr.ToDebugStr();
    for (auto piece = b.cbegin(); piece != b.cend(); ++piece) {
		switch (*piece) {
		case 'P': 
			evaluation += 1;
			break;
		case 'R':
            evaluation += 5;
			break;
		case 'N':
			evaluation += 3.2;
			break;
		case 'B':
			evaluation += 3.4;
			break;
		case 'Q':
			evaluation += 9;
			break;
		case 'K':
			evaluation += 200;
			break;

		case 'p': 
			evaluation += -1;
			break;
		case 'r':
			evaluation += -5;
			break;
		case 'n':
			evaluation += -3.2;
			break;
		case 'b':
			evaluation += -3.4;
			break;
		case 'q':
			evaluation += -9;
			break;
		case 'k':
			evaluation += -200;
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
		evaluation -= 0.01;
	//cout << evaluation << endl;
    return evaluation;
}