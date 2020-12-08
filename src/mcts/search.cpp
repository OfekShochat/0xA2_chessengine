#include <iostream>
#include <list>
#include <string>
#include "mcts/search.h"
#include "utils/preprocess_utils.h"
#include "../neural/tensorflow/use.h"
#include <chrono>

using namespace std;
using namespace chrono;

Search::Search(string b, Net* nn, int d, int n, int tt) {
    mBoard = b;
    depth = d;
    nodes = n;
    t = tt;
    net = nn;
}

Node* Search::go() {
    auto st = high_resolution_clock::now();
    int n = 0;
    while (true) {
        root->n += 1;
        n += 1;
        DBG_ALWAYS(cout << n << "\n");
        DBG_ALWAYS(cout << "select_leaf\n");
        Node* AB = root->select_AB();
        float score = AB->AB_evaluate();
        DBG_ALWAYS(cout << "done\n");
        DBG_ALWAYS(cout << "expand\n");
        AB->expand();
        DBG_ALWAYS(cout << "done\n");
        DBG_ALWAYS(cout << "update\n");
        DBG_ALWAYS(cout << "r: " << d << endl);
        /*while (true) {
            auto current = high_resolution_clock::now();
            auto duration = duration_cast<seconds>(current - st);
            if (duration.count() > 0.005) {
                break;
            }
        }*/
        //L->update(net->predict());
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
        if (n > nodes)
            break;
    }
    return root;
}