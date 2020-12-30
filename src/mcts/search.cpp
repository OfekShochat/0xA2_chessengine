#include <iostream>
#include <list>
#include <string>
#include "mcts/search.h"
#include "utils/preprocess_utils.h"
//#include "../neural/tensorflow/use.h"
#include <chrono>
#include "evaluate.h"

using namespace std;
using namespace chrono;

Search::Search(string b, int d, int n, int tt) {
    mBoard = b;
    depth = d;
    nodes = n;
    t = tt;
}

Node* Search::go() {
    auto st = high_resolution_clock::now();

    evaluator* eval = new evaluator();

    int t_depth;
    int p_depth = 0; 

    while (true) {
        root->n += 1;
        n += 1;

        Node* AB = root->select_AB();
        float score = AB->AB_evaluate();

        AB->expand();

        t_depth = root->root_depth();
        if (t_depth > p_depth) {
            p_depth = t_depth;
            cout << "info depth " << t_depth << " score " << score << endl;
        }

        /*while (true) {
            auto current = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(current - st);
            if (duration.count() > 0.005) {
                break;
            }
        }*/
        AB->update(score);
        DBG_ALWAYS(cout << "done, starting over\n");

        DBG_ALWAYS(cout << "best: " << root->getbest()->mMove << " q: " << root->getbest()->q << endl);

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
        if (n > nodes) {
            auto current = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(current - st);
            cout << duration.count() << " " << n << endl;
            if (duration.count() == 0)
                cout << "0" << endl;
            else 
                cout << n/(duration.count())*1000 << endl;
            break;
        }
    }
    return root;
}