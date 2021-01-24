
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
#include <chrono>
#include "evaluate.h"
#include "Thread.h"
#include <thread>
#include "chess/thc.h"
#include "utils/ThreadPool.h"

using namespace std;
using namespace chrono;

Search::Search(string b, int d, int n, int tt, bool stimemm) {
    mBoard = b;
    depth = d;
    nodes = n;
    t = tt;
    timemanage = stimemm;
}

Node* Search::go() {

    root->mBoard = mBoard;

    cout << "ddd: " << root->mBoard << endl;

    auto st = high_resolution_clock::now();

    int managedtime = timem();

    int t_depth;
    int p_depth = 0; 

    ThreadPool pool(thread::hardware_concurrency()/2);
    root->n += 1;
    root->expand();
    while (true) {
        Node* selected = root->select();
        if (!selected)
            cout << root->select() << endl;
        selected->ThreadMaster = true;
        selected->InUse = true;
        selected->n += 1;
        pool.enqueue(SearchThread::Start, selected);
        
        t_depth = root->root_depth();
        if (t_depth > p_depth) {
            p_depth = t_depth;
            auto current = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(current - st);
            cout << "info depth " << t_depth << " score " << root->getbest()->q << " nodes " << root->n << " nps " << int(root->n/(duration.count()/1000+0.01)) << " time " << duration.count() << " pv " << root->getbest()->mMove << endl;
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
        if (t != 0) {
            if (timemanage) {
                auto current = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(current - st);
                if (duration.count() > managedtime)
                    break;
            }
            else {
                auto current = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(current - st);
                if (duration.count() > t) {
                    //cout << n;
                    break;
                }
            }
        }
        
        if (nodes != 0) {
            if (root->n >= nodes) {
                auto current = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(current - st);
            
                break;
            }
        }
    }
    pool.~ThreadPool();
    for (auto const& i : root->children) {
        cout << i->mMove << " " << i->n << " " << i->q << endl;
    }
    
    return root;
}

int Search::timem() {
    thc::ChessRules cr;
    const char* c = mBoard.c_str();
    cr.Forsyth(c);

    double nMoves;
    double material = 0.0;

    string b = cr.ToDebugStr();
    for (auto piece = b.cbegin(); piece != b.cend(); ++piece) {
		switch (*piece) {
		case 'P': 
			material += 1.0;
			break;
		case 'R':
            material += 5.0;
			break;
		case 'N':
			material += 3.0;
			break;
		case 'B':
			material += 3.0;
			break;
		case 'Q':
			material += 9.0;
			break;

		case 'p': 
			material += 1.0;
			break;
		case 'r':
			material += 5.0;
			break;
		case 'n':
			material += 3.0;
			break;
		case 'b':
			material += 3.0;
			break;
		case 'q':
			material += 9.0;
			break;
		}
	}
    if (material < 20)
        nMoves = material + 10.0;
    else if (20 <= material && material <= 60)
        nMoves = 0.375*material + 28.0;
    else if (material > 60) {
        nMoves = 1.25*material - 30.0;
    }
        
    
    return (int)t/nMoves;
}
