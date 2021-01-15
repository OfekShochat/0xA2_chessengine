
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
#include <list>
#include <string>
#include "mcts/search.h"
#include "utils/preprocess_utils.h"
//#include "../neural/tensorflow/use.h"
#include <chrono>
#include "evaluate.h"
#include "Thread.h"
#include <thread>
#include "chess/thc.h"
#include <future>

using namespace std;
using namespace chrono;

Search::Search(string b, int d, int n, int tt, bool timemm) {
    mBoard = b;
    depth = d;
    nodes = n;
    t = tt;
    timemanage = timemm;
}

Node* Search::go() {
    auto st = high_resolution_clock::now();
    list<std::thread*> threads = {};
    evaluator* eval = new evaluator();

    //int managedtime = timem();        

    int t_depth;
    int p_depth = 0; 
    root->expand();
    while (true) {
        root->n += 1;
        n += 1;
        this_thread::sleep_for(std::chrono::milliseconds(100));
        if (4) { // activeThreads constant 4 < thread::hardware_concurrency()
            Node* selected = root->select();
            selected->ThreadMaster = true;
            selected->n += 1;
            thread* t1 = new thread(&SearchThread::Start, selected);
            threads.push_back(t1);
        }
        
        t_depth = root->root_depth();
        if (t_depth > p_depth) {
            p_depth = t_depth;
            cout << "info depth " << t_depth << " score " << root->getbest()->q << endl;
        }
        
        /*while (true) {
            auto current = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(current - st);
            if (duration.count() > 0.005) {
                break;
            }
        }*/
        DBG_ALWAYS(cout << "done, starting over\n");

        //cout << "best: " << root->getbest()->mMove << " q: " << root->getbest()->q << endl;

        /*if (!t == 0) {
            auto current = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(current - st);
            if (duration.count() > t) {
                //cout << n;
                break;
            }
        }
        if (!nodes == 0) {
            cout << n << "\n";
            if (n > nodes)
                break;
        }
        */
        /*if (t != 0) {
            if (timemanage) {
                auto current = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(current - st);
                if (duration.count() > managedtime)
                    break;
            }
        }
        */
        if (nodes != 0) {
            if (n >= nodes) {
                auto current = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(current - st);
            
                break;
            }
        }
    }
    for (auto const& i : root->children) {
        cout << i->mMove << " " << i->n << endl;
    }
    return root;
}
/*
int Search::timem() {
    thc::ChessRules cr;
    const char* c = mBoard.c_str();
    cr.Forsyth(c);

    int nMoves;
    int material = 0;

    string b = cr.ToDebugStr();
    for (auto piece = b.cbegin(); piece != b.cend(); ++piece) {
		switch (*piece) {
		case 'P': 
			material += 1;
			break;
		case 'R':
            material += 5;
			break;
		case 'N':
			material += 3;
			break;
		case 'B':
			material += 3;
			break;
		case 'Q':
			material += 9;
			break;

		case 'p': 
			material += 1;
			break;
		case 'r':
			material += 5;
			break;
		case 'n':
			material += 3;
			break;
		case 'b':
			material += 3;
			break;
		case 'q':
			material += 9;
			break;
		}
	}
    if (material < 20)
        nMoves = material + 10;
    else if (20 <= material <= 60)
        nMoves = 0.375*material + 22;
    else if (material > 60)
        nMoves = 1.25*material - 30;
    
    return (int)t/nMoves;
}
*/