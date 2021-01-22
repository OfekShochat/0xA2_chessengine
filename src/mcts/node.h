
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
#pragma once
#include <iostream>
#include <string>
#include <list>
#include "evaluate.h"
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
    float q = 0;
    float w = 0;
    bool turn = false;
    bool ThreadMaster = false;
    // default values for first seen search
    bool is_expanded = false;
    list<Node*> children = {};

    // function definition
    void update(float result);
    void expand();
    Node* select_AB();
    Node* getbest();
    //float AB_evaluate();
    int depth();
    int root_depth();
    int ABn = 0;
    Node* select();
    void delTree();
    void root_delTree();
private:
    double ucb1();
    //float AB(string fen, float alpha, float beta, int depth, int turn);
};