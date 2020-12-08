#include <iostream>
#include <list>
#include <math.h>
#include "chess/thc.h"
#include "mcts/node.h"
#include <vector>
using namespace std;

Node::Node(string b, string m, Node* p) {
    mBoard = b;
    mMove = m;
    mParent = p;
}

void Node::update(float result) {
    // preforms backpropagation. updates the visit count (n), total value (w) and mean value (q).
    Node* current = this;
    if (!turn)
        result = 1 - result;
    while (current->mParent != NULL) {
        current->n += 1;
        current->w += result;
        current->q = w / n;
        current = current->mParent;
        result = 1 - result;
    }
}

Node* Node::select() {
    // returns the node with the max of the ucb1 values of children.
    double max = -999.0;
    Node* selected{};
    for (auto& child : children) {
        double ucbv = child->ucb1();
        //cout << ucbv << "\n";
        if (ucbv == 100000.00) {
            selected = child;
            break;
        }
        if (ucbv > max) {
            selected = child;
            double max = ucbv;
        }
    }
    //cout << selected << "\n";
    return selected;
}

double Node::ucb1() {
    // ucb1 function used for policy.
    //cout << log(mParent->n) << "\n";
    if (mParent && mParent == 0) {
        return 100000.00;
    }
    //cout << "log parent: " << log(mParent->n) << " sqrt log parent: " << sqrt(log(mParent->n)) << " parent: " << mParent->n << endl;
    return q + sqrt(log(mParent->n) / (double(n) + 1.00));


}

Node* Node::select_AB() {
    // selects leaf node from root
    Node* current = this;
    while (true) {
        if (!current->is_expanded) {
            //cout << current->mBoard << " is not expanded\n";
            break;
        }
        //cout << "selecting\n";
        current = current->select();
    }
    return current;
}

void Node::expand() {
    is_expanded = true;
    thc::ChessRules cr;

    // converting string fen (mBoard) to char and loading the chess position.
    //cout << "loading pos\n";
    const char* c = mBoard.c_str();
    cr.Forsyth(c);
    turn = cr.WhiteToPlay();

    //cout << "movegen\n";
    std::vector<bool> check;
    std::vector<bool> mate;
    std::vector<bool> stalemate;
    std::vector<thc::Move> moves;
    cr.GenLegalMoveList(moves, check, mate, stalemate);
    //cout << "expanding\n";
    for (auto& move : moves) {
        cr.PlayMove(move);
        Node* thisNode = new Node(cr.ForsythPublish(), move.TerseOut(), this);
        children.push_back(thisNode);
        cr.PopMove(move);
    }
}

Node* Node::getbest() {
    // returns the node with the max of the ucb1 values of children.
    double max = -999.0;
    Node* selected{};
    for (auto& child : children) {
        //cout << ucbv << "\n";
        if (child->n > max) {
            selected = child;
            double max = child->n;
        }
    }
    //cout << selected << "\n";
    return selected;
}

float Node::AB_evaluate() {
    // temp values for beta and search depth. this should be customizable.
    float alpha = -1.0;
    float beta = 1.0;

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
        float score = -AB(cr.ForsythPublish(), -alpha, -beta, 5);
        cr.PopMove(move);
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}

float Node::AB(string fen, float alpha, float beta, int depth) {
    if (depth == 0)
        return (float)0.9; // evaluate position with network here
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
        float score = 1-AB(cr.ForsythPublish(), -alpha, -beta, depth - 1);
        cr.PopMove(move);
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}