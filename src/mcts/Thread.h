#pragma once
#include <iostream>
#include <string>
#include <list>
#include "node.h"

class SearchThread {
public:
    SearchThread(Node* SelectedNode);
    Node* selected;

    void Start();
};