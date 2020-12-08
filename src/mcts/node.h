#pragma once
#include <iostream>
#include <string>
#include <list>
using namespace std;

class Node {
    /* Node class. contains: the mean value(q), Number of visits value (n), total value(w) and the functions to update them.
        param b:
            board fen.
        param m:
            move uci.
            defaults to "" for root node
        param p:
            parent node.
            defaults to NULL for root node
    */

public:
    Node(string b, string m = "", Node* p = NULL);
    Node* mParent;
    string mMove;
    string mBoard;

    int n = 0;
    float q = 0.5;
    float w = 0;
    bool turn;
    // default values for first seen search
    bool is_expanded = false;
    list<Node*> children = {};

    // function definition
    void update(float result);
    void expand();
    Node* select_AB();
    Node* getbest();
    float AB_evaluate();

private:
    double ucb1();
    Node* select();
    float AB(string fen, float alpha, float beta, int depth);
};