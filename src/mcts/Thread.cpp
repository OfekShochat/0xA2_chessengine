#include "Thread.h"
#include "node.h"
#include <iostream>
#include <string>

SearchThread::SearchThread(Node* SelectedNode) {
    selected = SelectedNode;
}

void SearchThread::Start() {
    Node* AB = selected->select_AB();
    selected->update(selected->AB_evaluate());
    selected->expand();
}