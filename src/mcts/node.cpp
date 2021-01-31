
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
#include <math.h>
#include "chess/thc.h"
#include "mcts/node.h"
#include <vector>
#include <mutex>
#include "evaluate.h"

using namespace std;

Node::Node(thc::ChessRules b, string m, Node* p) { // with policy: , float pl 
    mBoard = b;
    mMove = m;
    mParent = p;
}
Node::~Node() {
    for (Node* const& child : children) {
        delete child;
    }
}

int Node::root_depth() {
    int d_ = 0;
    lock_guard<mutex> lock(mChildrenMutex);
    for (auto const& i : children) {
        d_ += i->depth();
    }
    return (int)(d_/children.size()+1);
}

int Node::depth() {
    Node* current = this;
    int i = 0;
    while (true) {
        i += 1;
        if (!current)
            break;
        if (!current->is_expanded)
            break;
        current = current->getbest();
    }
    return i;
}

void Node::update(float result) {
    // preforms backpropagation. updates the visit count (n), total value (w) and mean value (q).
    Node* current = this;
    int turn_factor = 1;
    if (!turn)
        turn_factor = -1;
    while (!current->ThreadMaster) {
        current->n += 1;
        current->w += result * turn_factor;
        current->q = w / n;
        current = current->mParent;
        turn_factor *= -1;
    }
    current->w += result;
    current->q = w / n;
}

Node* Node::select() {
    // returns the node with the max of the ucb1 values of children.
    double max = -99999.0;
    Node* selected{};
    double ucbv = -99999.0;
    lock_guard<mutex> lock(mChildrenMutex);
    for (Node*& child : children) {
        ucbv = child->ucb1();
        
        if (ucbv == 100000.00) {
            selected = child;
            max = ucbv;
            break;
        }
        if (ucbv > max) {
            selected = child;
            max = ucbv;
            //cout << selected->mMove << " " << max << endl;
        }
    }
    if (!selected) {
        cout << "the heck is going on: " << children.size() << " " << mBoard.ForsythPublish() << endl;
        cout << ucbv << endl;
        cout << max << endl;
    }
    return selected;
}

double Node::ucb1() {
    // ucb1 function used for policy.
    //cout << log(mParent->n) << "\n";
    if (n == 0) {
        return 100000.00;
    }
    //cout << "log parent: " << log(mParent->n) << " sqrt log parent: " << sqrt(log(mParent->n)) << " parent: " << mParent->n << endl;
    // with policy (unmodified) : Q + policy + factor * sqrt(log(parent.n) / n)
    // with policy (modified) : Q + factor * sqrt(policy / (n + 1));
    return q + 1.675 * sqrt(log(mParent->n) / (double(n) + 1.00));
}

Node* Node::select_AB() {
    // selects leaf node from root
    Node* current = this;
    Node* previous = NULL;
    int d = 0;
    while (current->is_expanded && !current->children.empty()) {
        d++;
        //cout << "selecting\n";
        previous = current;
        current = current->select();
        if (!current) {
            cout << previous->mBoard.ForsythPublish() << endl;
            cout << previous->children.size() << endl;
            cout << "none oof" << endl;
            cout << previous->is_expanded << endl;
            
        }
    }
    return current;
}

void Node::expand() {
    if (is_expanded)
        return;

    is_expanded = true;

    // converting string fen (mBoard) to char and loading the chess position.
    //cout << "loading pos\n";
    turn = mBoard.WhiteToPlay();

    //cout << "movegen\n";
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    std::vector<thc::Move> moves;
    mBoard.GenLegalMoveList(moves, check, mate, stalemate);
    //cout << "expanding\n";
    // with policy: int i = 0;
    for (auto& move : moves) {
        mBoard.PlayMove(move);
        Node* thisNode = new Node(mBoard, move.TerseOut(), this); // with policy: policy=policies[i]
        {
            lock_guard<mutex> lock(mChildrenMutex);
            children.push_back(thisNode);
        }
        
        mBoard.PopMove(move);
        // with policy: i++;
    }
}

Node* Node::getbest() {
    // returns the node with the max of the ucb1 values of children. poop
    lock_guard<mutex> lock(mChildrenMutex);

    double max = -999.0;
    Node* selected{};
    
    for (auto& child : children) {
        if (child->n == max) {
            if (child->q>selected->q) {
                selected = child;
            }
        }
        if (child->n > max) {
            selected = child;
            max = child->n;
        }
    }
    //cout << selected << "\n";
    return selected;
}

/*
float Node::AB_evaluate() {
    // temp values for beta and search depth. this should be customizable.
    float alpha = -1000.0;
    float beta = 1000.0;
    int color;
    thc::ChessRules cr;
    const char* c = mBoard.c_str();
    cr.Forsyth(c);
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    std::vector<thc::Move> moves;
    cr.GenLegalMoveList(moves, check, mate, stalemate);
    if (cr.WhiteToPlay()) 
        color = -1;
    else
        color = 1;
    for (auto& move : moves) {
        ABn++;
        cr.PlayMove(move);
        float score = AB(cr.ForsythPublish(), alpha * color, beta * color, 15, color) * color;
        //cout << score << endl;
        cr.PopMove(move);
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}

float Node::AB(string fen, float alpha, float beta, int depth, int color) {
    ABn++;
    if (depth == 0) {
        return eval->evaluate(fen) * turn; // evaluate position with network here
    }
    float score;
    thc::ChessRules cr;
    const char* c = mBoard.c_str();
    cr.Forsyth(c);
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    std::vector<thc::Move> moves;
    cr.GenLegalMoveList(moves, check, mate, stalemate);
    for (auto& move : moves) {
        cr.PlayMove(move);
        score = -AB(cr.ForsythPublish(), -alpha, -beta, depth - 1, -color);
        cr.PopMove(move);
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}
*/